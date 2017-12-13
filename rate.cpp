
#include "rate.h"
#include "time_defines.h"

Rate::Rate()
{
	set_roll_speed(0);
	set_pitch_speed(0);
	set_yaw_speed(0);
	set_thrust(0);
	//m_curr_roll_speed(0), m_curr_pitch_speed(0), m_curr_yaw_speed(0), m_curr_thrust(0)
}
