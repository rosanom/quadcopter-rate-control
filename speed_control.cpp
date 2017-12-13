#include "speed_control.h"
#include "time_defines.h"
#include "controller.h"
#include <fstream>

SpeedControl::SpeedControl()
    : PeriodicTask("speed_control", SPEED_PERIOD, TIME_UNIT, SPEED_JITTER),
      /*
	  m_kinematics(kin),
      m_pwm_left(0),
      m_pwm_right(0),
      */
      m_target_roll(0), m_target_pitch(0), m_target_yaw(0), m_target_thrust(0),
      m_curr_roll_speed(0), m_curr_pitch_speed(0), m_curr_yaw_speed(0),
	    m_curr_thrust(0),
      m_roll_pid(m_real_time_period), // m_real_time_period from periodic_task.h
      m_pitch_pid(m_real_time_period),
      m_yaw_pid(m_real_time_period),
      m_thrust_pid(m_real_time_period),
      flight_started(0)

{
                        // Kp, Ki, Kd, saturation
  m_roll_pid.set_all_params(0.1, 0.5, 0, (-ROLL_MAXIMUM_ANGULAR_SPEED)); //SIGN IS USED BECAUSE ROLL_MAXIMUM_ANGULAR_SPEED IS NEGATIVE
	m_pitch_pid.set_all_params(0.1, 0.5, 0, (-PITCH_MAXIMUM_ANGULAR_SPEED)); //SIGN IS USED BECAUSE PITCH_MAXIMUM_ANGULAR_SPEED IS NEGATIVE
	m_yaw_pid.set_all_params(0.1, 0.5, 0, (-YAW_MAXIMUM_ANGULAR_SPEED)); //SIGN IS USED BECAUSE YAW_MAXIMUM_ANGULAR_SPEED IS NEGATIVE
	m_thrust_pid.set_all_params(0.1, 0.5, 0, 10);

  m_controller = new Controller();
  m_simulator = new server_socket();

  m_controller->on(); // it's here because in main file doesn't work
  m_simulator->on();

  roll_speed_simulator.open("roll_sp_sim.txt");
  pitch_speed_simulator.open("pitch_sp_sim.txt");
  yaw_speed_simulator.open("yaw_sp_sim.txt");
  engines_speed_simulator.open("engines_speed_sim.txt");

  roll_speed_joypad.open("roll_sp_joy.txt");
  pitch_speed_joypad.open("pitch_sp_joy.txt");
  yaw_speed_joypad.open("yaw_sp_joy.txt");
  thrust_speed_joypad.open("thrust_sp_joy.txt");


}

// read the target values from joypad
float SpeedControl::calc_max(float f1, float f2, float f3, float f4){
  float max=0;
  if( f1 > max)   max = f1;
  if( f2 > max)   max = f2;
  if( f3 > max)   max = f3;
  if( f4 > max)   max = f4;
  return max;
}
float SpeedControl::calc_min(float f1, float f2, float f3, float f4){
  float min=9999;
  if( f1 < min)   min = f1;
  if( f2 < min)   min = f2;
  if( f3 < min)   min = f3;
  if( f4 < min)   min = f4;
  return min;
}

void SpeedControl::get_targets_from_joypad() {
	m_target_roll = m_controller->getRollJoystick();
	m_target_pitch = m_controller->getPitchJoystick();
	m_target_yaw = m_controller->getYawJoystick();
	m_target_thrust = m_controller->getThrustJoystick();

	//on();
}

void SpeedControl::setEnginesSpeed(float roll, float pitch, float yaw, float thrust){
  //engines clockwise from top left
  float engine1, engine2, engine3, engine4, max, min;
  //float pwm1, pwm2, pwm3, pwm4;
  engine1 = roll - pitch + yaw + thrust;
  engine2 = -roll - pitch - yaw + thrust;
  engine3 = -roll + pitch + yaw + thrust;
  engine4 = roll + pitch - yaw + thrust;
  // adjusted as follow: (MAXVOLTS*engine1Speed)/100 es. thrust to max -> 9v (MAX)
  engine1 = (MAXVOLTS*engine1)/100;
  engine2 = (MAXVOLTS*engine2)/100;
  engine3 = (MAXVOLTS*engine3)/100;
  engine4 = (MAXVOLTS*engine4)/100;

  // check if the drone took off
  if( flight_started==0 ){
    if( engine1>0 || engine2>0 || engine3>0 || engine4>0 )
      flight_started=1;
  }

  // managing min and max PWM outputs
  max = calc_max(engine1, engine2, engine3, engine4);
  min = calc_min(engine1, engine2, engine3, engine4);
  if( max > MAXVOLTS ){
    float diff = max - MAXVOLTS;
    engine1 -=  diff; //scale each engine by the diff value
    engine2 -=  diff;
    engine3 -=  diff;
    engine4 -=  diff;
    max = MAXVOLTS; //update max value
    min = min-diff; //update min value substracting the diff from old min
  }
  if( max-min > (MAXVOLTS-MINVOLTS) ){ // be sure min is not negative, otherwise adjust its value
    min = max - (MAXVOLTS-MINVOLTS);
  }
  else if( flight_started!=0 ){ // the drone already took off, we can set the min to MINVOLTS
    min = MINVOLTS;
  }
  if( engine1 < min ) engine1 = min; // engines output can't be less then min
  if( engine2 < min ) engine2 = min;
  if( engine3 < min ) engine3 = min;
  if( engine4 < min ) engine4 = min;

  m_simulator->set_curr_engine_speed1(engine1);
  m_simulator->set_curr_engine_speed2(engine2);
  m_simulator->set_curr_engine_speed3(engine3);
  m_simulator->set_curr_engine_speed4(engine4);
  //std::cout << engine1Speed << " " << engine2Speed << engine3Speed << " " << engine4Speed << std::endl;
}

void SpeedControl::off() {
  PeriodicTask::off();
  m_roll_pid.reset();
	m_pitch_pid.reset();
	m_yaw_pid.reset();
	m_thrust_pid.reset();
}
void SpeedControl::run() {

  //std::cout << "SONO LO SPEEDCONTROL" << std::endl;

  //m_controller->run();
	// read current rate from Rate class
	m_curr_roll_speed = m_simulator->get_curr_roll_speed();
	m_curr_pitch_speed = m_simulator->get_curr_pitch_speed();
	m_curr_yaw_speed = m_simulator->get_curr_yaw_speed();
	m_curr_thrust = m_simulator->get_curr_thrust();
  //std::cout << "Current Roll: " << m_curr_roll_speed << std::endl;

  // get rate targets from joypad
	m_target_roll = m_controller->getRollJoystick();
	m_target_pitch = m_controller->getPitchJoystick();
	m_target_yaw = m_controller->getYawJoystick();
	m_target_thrust = m_controller->getThrustJoystick();
  //std::cout << "Target Roll: " << m_target_roll << std::endl;
  //std::cout << "Differenza: " << (m_target_roll-m_curr_roll_speed) << std::endl;

  // pids output
	m_pid_roll_output = m_roll_pid.evaluate( m_target_roll-m_curr_roll_speed );
	m_pid_pitch_output = m_pitch_pid.evaluate( m_target_pitch-m_curr_pitch_speed );
	m_pid_yaw_output = m_yaw_pid.evaluate( m_target_yaw-m_curr_yaw_speed );
	m_pid_thrust_output = m_thrust_pid.evaluate( m_target_thrust-m_curr_thrust );

  //std::cout << "PID Roll: " << m_pid_roll_output << "\n" << std::endl;

  //roll_file << "Target roll speed: " << m_target_roll << ", Current roll speed: " << m_simulator->get_curr_roll_speed() << ", PID roll output: "<< m_pid_roll_output << std::endl;
  //pitch_file << "Target pitch speed: " << m_target_pitch << ", Current pitch speed: " << m_simulator->get_curr_pitch_speed() << ", PID pitch output: "<< m_pid_pitch_output << std::endl;
  //yaw_file << "Target yaw speed: " << m_target_yaw << ", Current yaw speed: " << m_simulator->get_curr_yaw_speed() << ", PID yaw output: "<< m_pid_yaw_output << std::endl;
  //thrust_file << "Target thrust speed: " << m_target_thrust << ", Current thrust speed: " << m_simulator->get_curr_thrust() << ", PID thrust output: "<< m_pid_thrust_output << std::endl;

	//new angular speeds
	m_simulator->set_roll_speed(m_pid_roll_output+m_curr_roll_speed); //old speed + pid adjustment value
	m_simulator->set_pitch_speed(m_pid_pitch_output+m_curr_pitch_speed);
	m_simulator->set_yaw_speed(m_pid_yaw_output+m_curr_yaw_speed);
	m_simulator->set_thrust(m_pid_thrust_output+m_curr_thrust);

  //std::cout << "Roll speed: " << m_simulator->get_curr_roll_speed() << "\n" << std::endl;

  // new engines speed (mixed) based on new current angular speeds
  setEnginesSpeed( m_simulator->get_curr_roll_speed(), m_simulator->get_curr_pitch_speed(),m_simulator->get_curr_yaw_speed(), m_simulator->get_curr_thrust() );

  roll_speed_simulator << m_simulator->get_curr_roll_speed() << std::endl;
  pitch_speed_simulator << m_simulator->get_curr_pitch_speed() << std::endl;
  yaw_speed_simulator << m_simulator->get_curr_yaw_speed() << std::endl;
  engines_speed_simulator << "Engine 1 speed:" << m_simulator->get_curr_engine_speed1() << ", engine 2 speed: " << m_simulator->get_curr_engine_speed2() << ", engine 3 speed: " << m_simulator->get_curr_engine_speed3() << ", engine 4 speed: " << m_simulator->get_curr_engine_speed4() << std::endl;

  roll_speed_joypad << m_controller->getRollJoystick() << std::endl;
  pitch_speed_joypad << m_controller->getPitchJoystick() << std::endl;
  yaw_speed_joypad << m_controller->getYawJoystick() << std::endl;
  thrust_speed_joypad << m_controller->getThrustJoystick() << std::endl;

  std::cout<<"target roll:"<<m_controller->getRollJoystick()<<",sim roll: "<<m_simulator->get_curr_roll_speed()<<std::endl;
  std::cout<<"target thrust:"<<m_controller->getThrustJoystick()<<", sim speeds:"<<m_simulator->get_curr_engine_speed1()<<", "<<m_simulator->get_curr_engine_speed2()<<", "<<m_simulator->get_curr_engine_speed3()<<", "<<m_simulator->get_curr_engine_speed4()<<std::endl;

  //engines_file << "Engine 1 speed:" << engine1Speed << ", engine 2 speed: " << engine2Speed << ", engine 3 speed: " << engine3Speed << ", engine 4 speed: " << engine4Speed << std::endl;
}
