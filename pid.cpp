/*
	https://github.com/psr2016/psr/blob/master/simul_pid/pid.c
	https://github.com/psr2016/psr/blob/master/firmware/libs/controllib/pid.cpp
*/

#include "pid.h"
#include <iostream>

using namespace std;

Pid::Pid(float kp, float ki, float kd, float saturation, float delta_t)
    : m_kp(kp), m_ki(ki), m_kd(kd), m_integral_factor(0), m_saturation(saturation), m_delta_t(delta_t),
	m_saturation_flag(0), m_prev_input(0) // deleted m_out_i(0)
{
}

Pid::Pid(float delta_t)
	: Pid(1, 1, 10, 100, delta_t)
{
}

float Pid::evaluate(float input) { //input is the error
	//proportional factor
	float out_P = (m_kp * input);

	//integrative factor
	float out_I;
	if(m_saturation_flag == 0){ //if not in saturation, calc. integraf factor
		out_I = m_integral_factor + (m_ki * ( (input+m_prev_input)/2) ) * m_delta_t; // error file psr/firmware/libs/controllib/pid.cpp line 93
		m_integral_factor = out_I;
	}
	else
		out_I = 0;

	//derivative factor
	float out_D = m_kd * (input-m_prev_input) / m_delta_t;

	float output = out_P + out_I + out_D;

	if(output >= m_saturation){
		output = m_saturation;
		m_saturation_flag = 1;
	}
	else if(output <= -m_saturation) {
    //std::cout << "Ci vado" << '\n';
		output = -m_saturation;
		m_saturation_flag = 1;
	}
	else {
		m_saturation_flag = 0;
	}

	m_prev_input = input;
    //float deriv = (input - m_prev_input) / m_delta_t;
    //m_out_i = m_out_i + m_ki * input * m_delta_t;
    //m_prev_input = input;
    //return input * m_kp + m_out_i + deriv * m_kd;
    return output;
}

void Pid::reset() {
	m_integral_factor = 0;
	m_prev_input = 0;
	m_saturation_flag = 0;
}
