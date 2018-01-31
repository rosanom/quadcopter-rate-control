/*
 * controller.h
 */

 #ifndef __CONTROLLER_H
 #define __CONTROLLER_H

 #include "periodic_task.h"
 #include "joystick.h"
 #include <stdbool.h>

 #define YAW_MAXIMUM_ANGULAR_SPEED -0.5
 #define ROLL_MAXIMUM_ANGULAR_SPEED -0.5
 #define PITCH_MAXIMUM_ANGULAR_SPEED -0.5
 #define THRUST_MIN_OUTPUT 3.0
 #define THRUST_MAX_OUTPUT 6.0


 class Controller : public PeriodicTask {

  public:
    virtual void run();
    virtual void off(bool stop_pwm = true);
    Controller();
    void setRollJoystick(float m_roll_speed_joystick);
    void setYawJoystick(float m_yaw_speed_joystick);
    void setPitchJoystick(float m_pitch_speed_joystick);
    void setThrustJoystick(float m_thrust_joystick);
    float getRollJoystick();
    float getYawJoystick();
    float getPitchJoystick();
    float getThrustJoystick();


  protected:
    cJoystick *joystick;
    float roll_speed_joystick;
    float yaw_speed_joystick;
    float pitch_speed_joystick;
    float thrust_joystick;

 };

 #endif
