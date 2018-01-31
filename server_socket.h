#ifndef __SERVER_SOCKET_H
#define __SERVER_SOCKET_H

#include "periodic_task.h"

#define PORTNUMBER 19923

class server_socket : public PeriodicTask {
	public:
		server_socket();
		void run();
    void off();

		float get_curr_roll_speed() {
			return m_curr_roll_speed;
		}
		float get_curr_pitch_speed() {
			return m_curr_pitch_speed;
		}
		float get_curr_yaw_speed() {
			return m_curr_yaw_speed;
		}
		float get_curr_thrust() {
			return m_curr_thrust;
		}

    float get_curr_engine_speed1() {
			return m_curr_engine_speed1;
		}
    float get_curr_engine_speed2() {
			return m_curr_engine_speed2;
		}
    float get_curr_engine_speed3() {
			return m_curr_engine_speed3;
		}
    float get_curr_engine_speed4() {
			return m_curr_engine_speed4;
		}

		void set_roll_speed(float curr_roll_speed) {
			m_curr_roll_speed=curr_roll_speed;
		}
		void set_pitch_speed(float curr_pitch_speed) {
			m_curr_pitch_speed=curr_pitch_speed;
		}
		void set_yaw_speed(float curr_yaw_speed) {
			m_curr_yaw_speed=curr_yaw_speed;
		}
		void set_thrust(float curr_thrust) {
			m_curr_thrust=curr_thrust;
		}
    void set_curr_engine_speed1(float curr_engine_speed1) {
			m_curr_engine_speed1 = curr_engine_speed1;
		}
    void set_curr_engine_speed2(float curr_engine_speed2) {
			m_curr_engine_speed2 = curr_engine_speed2;
		}
    void set_curr_engine_speed3(float curr_engine_speed3) {
			m_curr_engine_speed3 = curr_engine_speed3;
		}
    void set_curr_engine_speed4(float curr_engine_speed4) {
			m_curr_engine_speed4 = curr_engine_speed4;
		}

	private:
		float* receive();
		void send();

		float m_curr_roll_speed, m_curr_pitch_speed, m_curr_yaw_speed, m_curr_thrust, m_curr_engine_speed1, m_curr_engine_speed2, m_curr_engine_speed3, m_curr_engine_speed4;
		int clientSocket;
		char buffer[255];
};


#endif
