#ifndef INC_INIT_H_
#define INC_INIT_H_
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef* global_huart1;
extern I2C_HandleTypeDef* global_hi2c1;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void bring_up(void);
bool init_system_clock(void);
void init_gpio(void);
bool init_timer_1(TIM_HandleTypeDef* htim1);
bool init_uart_1(UART_HandleTypeDef* huart1);
bool init_i2c_1(I2C_HandleTypeDef* hi2c1);

#endif /* INC_INIT_H_ */
