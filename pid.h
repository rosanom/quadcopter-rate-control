#ifndef __PID__H
#define __PID__H

class Pid{

public:
	Pid(float delta_t);
    Pid(float kp, float ki, float kd, float saturation, float delta_t);
    float evaluate(float input);

	float get_kp() {
		return m_kp;
	};
    float get_ki() {
		return m_ki;
	};
    float get_kd() {
		return m_kd;
	};
	float get_saturation() {
		return m_saturation;
	}
    void set_kp(float v) {
		m_kp = v;
	};
    void set_ki(float v) {
		m_ki = v;
	};
    void set_kd(float v) {
		m_kd = v;
	};
	void set_saturation(float v) {
		m_saturation = v;
	};

	void set_all_params(float kp, float ki, float kd, float saturation) {
		m_kp = kp;
		m_ki = ki;
		m_kd = kd;
		m_saturation = saturation;
	};
	void reset();

 protected:
	int m_saturation_flag;
  float m_kp, m_ki, m_kd, m_integral_factor, m_saturation, m_delta_t, m_prev_input; // deleted m_out_i
};


#endif
