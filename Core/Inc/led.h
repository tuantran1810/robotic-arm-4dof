/*
 * gpio.h
 *
 *  Created on: Jul 11, 2023
 *      Author: tuantran
 */

#ifndef INC_LED_H_
#define INC_LED_H_

struct LedInstance {
	GPIO_TypeDef* gpio;
	uint16_t pin;

	LedInstance(GPIO_TypeDef* gpio, uint16_t pin) {
		this->gpio = gpio;
		this->pin = pin;
		this->reset();
	}

	void set() {
		HAL_GPIO_WritePin(this->gpio, this->pin, GPIO_PIN_SET);
	}

	void reset() {
		HAL_GPIO_WritePin(this->gpio, this->pin, GPIO_PIN_RESET);
	}

	void toggle() {
		HAL_GPIO_TogglePin(this->gpio, this->pin);
	}
};



#endif /* INC_LED_H_ */
