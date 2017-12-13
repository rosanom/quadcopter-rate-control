/*
 * controller.h
 */

 #ifndef __CONTROLLER_H
 #define __CONTROLLER_H

 #include "periodic_task.h"
 #include "joystick.h"
 #include <stdbool.h>

 #define YAW_MAXIMUM_ANGULAR_SPEED -3.49066   //200 gradi al secondo (convertito in radianti). Trovato su internet
 #define ROLL_MAXIMUM_ANGULAR_SPEED -3.14159  //180 gradi al secondo (valore arbitrario, lo modificheremo con il professore in caso
 #define PITCH_MAXIMUM_ANGULAR_SPEED -3.14159 //180 gradi al secondo (valore arbitrario, lo modificheremo con il professore in caso


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
