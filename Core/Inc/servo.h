/*
 * servo.h
 *
 *  Created on: Jul 10, 2023
 *      Author: tuantran
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include "pwm.h"

struct ServoInstance {
	PWMInstance* pwm;
	uint32_t offset;
	uint32_t range;

	ServoInstance(PWMInstance* pwm,uint32_t offset,uint32_t range) {
		this->pwm = pwm;
		this->offset = offset;
		this->range = range;
	}

	void set_position_degree(uint32_t pos) {
		uint32_t w = ((pos * this->range) / 180) + this->offset;
		this->pwm->set_pulse_width(w);
	}
};



#endif /* INC_SERVO_H_ */
