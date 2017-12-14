/*
 * controller.cpp
 */

#include "controller.h"
#include "time_defines.h"
#include <iostream>
#include <time.h>

using namespace std;


Controller::Controller()
  :PeriodicTask("controller", CONTROLLER_PERIOD, TIME_UNIT, CONTROLLER_JITTER)
{
  joystick = new cJoystick();
}

void Controller::setRollJoystick(float m_roll_speed_joystick){
  roll_speed_joystick = m_roll_speed_joystick;
}

void Controller::setYawJoystick(float m_yaw_speed_joystick){
  yaw_speed_joystick = m_yaw_speed_joystick;
}

void Controller::setPitchJoystick(float m_pitch_speed_joystick){
  pitch_speed_joystick = m_pitch_speed_joystick;
}

void Controller::setThrustJoystick(float m_thrust_joystick){
  thrust_joystick = m_thrust_joystick;
}

float Controller::getRollJoystick(){
  return roll_speed_joystick;
}

float Controller::getYawJoystick(){
  return yaw_speed_joystick;
}

float Controller::getPitchJoystick(){
  return pitch_speed_joystick;
}

float Controller::getThrustJoystick(){
  return thrust_joystick;
}


void Controller::off(bool stop_pwm)
{
    PeriodicTask::off();
    if (stop_pwm) {
        setRollJoystick(0);
        setYawJoystick(0);
        setPitchJoystick(0);
        setThrustJoystick(0);
    }
}

void Controller::run()
{
    //std::cout << "SONO  IL CONTROLLER" << std::endl;
    clock_t start = clock();
    setRollJoystick((-ROLL_MAXIMUM_ANGULAR_SPEED)*joystick->joystickPosition(1).x);
    setPitchJoystick((-PITCH_MAXIMUM_ANGULAR_SPEED)*joystick->joystickPosition(1).y);
    setYawJoystick((-YAW_MAXIMUM_ANGULAR_SPEED)*joystick->joystickPosition(0).x);
    setThrustJoystick(((joystick->joystickPosition(0).y+1)*100/(2)));
    clock_t end = clock();
    cout << (end-start) << " micros joystick" << endl;
    std::cout << "Thrust: " << getThrustJoystick() << " Pitch: " << getPitchJoystick() << " Yaw: " << getYawJoystick() << " Roll: " << getRollJoystick() << std::endl;

}
