#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "stm32f1xx_hal.h"

struct PWMInstance {
	TIM_HandleTypeDef* timer;
	uint32_t channel;
	uint32_t cycle;
	uint32_t w;

	PWMInstance(TIM_HandleTypeDef* timer, uint32_t channel, uint32_t cycle) {
		this->timer = timer;
		this->channel = channel;
		this->cycle = cycle;
		this->w = 0;
	}

	bool start(void) {
		bool ok = (HAL_TIM_PWM_Start(this->timer, this->channel) == HAL_OK);
		if (!ok) {
			return false;
		}
		__HAL_TIM_SET_COMPARE(this->timer, this->channel, 0);
		return ok;
	}

	bool stop(void) {
		__HAL_TIM_SET_COMPARE(this->timer, this->channel, 0);
		return (HAL_TIM_PWM_Stop(this->timer, this->channel) == HAL_OK);
	}

	void set_duty_cycle(uint32_t percentage) {
		uint32_t w = (percentage * this->cycle) / 100;
		if (w > this->cycle) {
			w = this->cycle;
		}
		this->w = w;
		__HAL_TIM_SET_COMPARE(this->timer, this->channel, w);
	}

	uint32_t get_duty_cycle(void) {
		return (this->w * 100)/this->cycle;
	}

	uint32_t get_pulse_width(void) {
		return this->w;
	}

	void set_pulse_width(uint32_t w) {
		__HAL_TIM_SET_COMPARE(this->timer, this->channel, w);
	}
};

#endif /* INC_PWM_H_ */
