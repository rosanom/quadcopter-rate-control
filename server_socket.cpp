#include "periodic_task.h"
#include "speed_control.h"
#include "time_defines.h"
#include "controller.h"
#include "server_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

using namespace std;

union byteToFl
{
   unsigned char byte[4];
   float number;
}byteToFl;

server_socket::server_socket()
    : PeriodicTask("server_socket", SIMULATOR_PERIOD, TIME_UNIT, SIMULATOR_JITTER),
    m_curr_roll_speed(0),
  	m_curr_pitch_speed(0),
  	m_curr_yaw_speed(0),
  	m_curr_thrust(0),
    m_curr_engine_speed1(0),
    m_curr_engine_speed2(0),
    m_curr_engine_speed3(0),
    m_curr_engine_speed4(0),
    clientSocket(-1)
{
  int sockfd, n;
  float *tr;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("ERROR opening socket");
    exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORTNUMBER);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    perror("ERROR on binding");
    exit(1);
  }

  listen(sockfd,1);
  clilen = sizeof(cli_addr);
  // blocking socket
  clientSocket = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

  tr=receive(); // at fitst communication we receive all 0s, we ignore them
  //send();
  printf("connessione effettuata\n");
}

float* server_socket::receive(){
  int n;
  static float values[7];
  n = read(clientSocket, buffer,255); // delta_t, x_pos, y_pos, z_pos, roll_speed, pitch_speed, yaw_speed
   if (n < 0) {
     perror("errore reading from socket");
     exit(1);
   }
   //printf("bytes letti: %d\n",n);

   // how to convert 4 bytes in 1 float?
   int shift=0;
   for(int i=0;i<7;i++){ // 7 floats
     byteToFl.byte[0]=buffer[i+shift];
     byteToFl.byte[1]=buffer[i+shift+1];
     byteToFl.byte[2]=buffer[i+shift+2];
     byteToFl.byte[3]=buffer[i+shift+3];
     values[i]=byteToFl.number;
     shift+=3;
   }
   shift=0;
/*
   printf("float letti:");
   for(int i=0;i<7;i++){
     printf("%f, ", values[i]);
   }
*/
  return values;
}

void server_socket::send(){
  float engineOutput[4];
  engineOutput[0]=get_curr_engine_speed1();
  engineOutput[1]=get_curr_engine_speed2();
  engineOutput[2]=get_curr_engine_speed3();
  engineOutput[3]=get_curr_engine_speed4();

  printf("dati inviati\n");

  write(clientSocket, engineOutput, 16); // 16 bytes

}

void server_socket::off() {
  PeriodicTask::off();
  close(clientSocket);
}

void server_socket::run(){
  //printf("run serversocket\n");
  clock_t start = clock();
  send();

  float *valuesFromSim;
  valuesFromSim=receive(); // read the values from simulator
  set_roll_speed( *(valuesFromSim+4) );
  set_pitch_speed( *(valuesFromSim+5) );
  set_yaw_speed( *(valuesFromSim+6) );
  set_thrust(*(valuesFromSim+3) ); //added
  clock_t end = clock();
  // cout << (end-start) << " micros server socket" << endl;
}
