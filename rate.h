#ifndef __RATE__H
#define __RATE__H

class Rate {
	public:
		Rate();
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

	private:
		float m_curr_roll_speed, m_curr_pitch_speed, m_curr_yaw_speed, m_curr_thrust;
};


#endif
