/*
	https://github.com/psr2016/psr/blob/master/firmware/MotionControl_v6/speed_control.h
*/

#ifndef __SPEED_CONTROL_H
#define __SPEED_CONTROL_H

#include <stdbool.h>
#include "pid.h"
#include "periodic_task.h"
#include "controller.h"
#include "server_socket.h"
#include <fstream>

#define MAXVOLTS 6 //Max x volts as input to the engines
#define MINVOLTS 3

class SpeedControl : public PeriodicTask {
 public:
 	SpeedControl();
    //SpeedControl(Kinematics & kin);
    void run();
    void off();
    void get_targets_from_joypad();
    void setEnginesSpeed(float roll, float pitch, float yaw, float thrust);
    std::ofstream roll_speed_simulator, pitch_speed_simulator, yaw_speed_simulator, engines_speed_simulator,
    roll_speed_joypad, pitch_speed_joypad, yaw_speed_joypad, thrust_speed_joypad;

 protected:
    //Kinematics & m_kinematics;
    //int m_pwm_left, m_pwm_right;

	float m_target_roll, m_target_pitch, m_target_yaw, m_target_thrust,
	m_curr_roll_speed, m_curr_pitch_speed, m_curr_yaw_speed, m_curr_thrust,
  m_pid_roll_output,
  m_pid_yaw_output, m_pid_pitch_output, m_pid_thrust_output;
  Pid m_roll_pid, m_pitch_pid, m_yaw_pid, m_thrust_pid;

  Controller *m_controller;
  server_socket *m_simulator;

 private:
   float calc_max(float f1, float f2, float f3, float f4);
   float calc_min(float f1, float f2, float f3, float f4);
   int flight_started;
};

#endif
