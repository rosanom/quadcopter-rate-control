/*
 * main.cpp
 */

#include "joystick.h"
#include "speed_control.h"
#include <iostream>
#include <unistd.h>
#include "time_defines.h"
#include "time.h"

using namespace std;


int main(int argc, char **argv)
{

    //Controller *c = new Controller();
    SpeedControl *sc = new SpeedControl();
    int err;
    
    clock_t start, end;

    sc->on();
    for (;;) {
        //sc->run();
        start = clock();
        run_all_tasks();
        end = clock();
        err = usleep( ( ( TIME_UNIT*1000 ) ) - ( ( ( end-start ) / (CLOCKS_PER_SEC/1000) ) * 1000) ) ; //funzione usleep prende il tempo in microsecondi, per questo si moltiplica per 1000
        
        //cout << "Start time: " << (start)/(CLOCKS_PER_SEC/1000) << "ms\n" << "End time: " << (end)/(CLOCKS_PER_SEC/1000) << "ms\n" << (end-start)/(CLOCKS_PER_SEC/1000) << "ms" << endl;
        //cout <<  ( ( TIME_UNIT*1000 ) )-( ( ( end-start ) / (CLOCKS_PER_SEC/1000) ) * 1000) << "micros" << endl;

    }

    return 0;

}
