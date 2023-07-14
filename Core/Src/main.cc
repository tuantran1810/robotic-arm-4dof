#include "init.h"
#include "pwm.h"
#include "servo.h"
#include "led.h"
#include "uart.h"

void Error_Handler(void);

struct Resources {
	TIM_HandleTypeDef* timer;
	UART_HandleTypeDef* uart;
	PWMInstance* pwm1;
	PWMInstance* pwm2;
	PWMInstance* pwm3;
	PWMInstance* pwm4;
	ServoInstance* servo1;
	ServoInstance* servo2;
	ServoInstance* servo3;
	ServoInstance* servo4;
	UartInstance* uart1;
	LedInstance* debug_led;

	bool start(void) {
		if (!pwm1 || !pwm2 || !pwm3 || !pwm4) {
			return false;
		}

		if (!pwm1->start() || !pwm2->start() || !pwm3->start() || !pwm4->start()) {
			return false;
		}

		return true;
	}

	bool stop(void) {
		if (!pwm1 || !pwm2 || !pwm3 || !pwm4) {
			return false;
		}

		if (!pwm1->stop() || !pwm2->stop() || !pwm3->stop() || !pwm4->stop()) {
			return false;
		}

		return true;
	}
};

int main(void)
{
	bring_up();
	if (!init_system_clock()) {
		Error_Handler();
	};
	init_gpio();
	TIM_HandleTypeDef timer1;
	if (!init_timer_1(&timer1)) {
		Error_Handler();
	}
	UART_HandleTypeDef u1;
	if (!init_uart_1(&u1)) {
		Error_Handler();
	}

	PWMInstance pwm1 = PWMInstance(&timer1, TIM_CHANNEL_1, 10000);
	PWMInstance pwm2 = PWMInstance(&timer1, TIM_CHANNEL_2, 10000);
	PWMInstance pwm3 = PWMInstance(&timer1, TIM_CHANNEL_3, 10000);
	PWMInstance pwm4 = PWMInstance(&timer1, TIM_CHANNEL_4, 10000);

	ServoInstance servo1 = ServoInstance(&pwm1, 250, 1000);
	ServoInstance servo2 = ServoInstance(&pwm2, 250, 1000);
	ServoInstance servo3 = ServoInstance(&pwm3, 250, 1000);
	ServoInstance servo4 = ServoInstance(&pwm4, 250, 1000);

	UartInstance uart1 = UartInstance(&u1);
	uart1.register_rx_callback();

	LedInstance debug_led = LedInstance(GPIOC, GPIO_PIN_13);

	Resources resources = Resources{
		timer: &timer1,
		pwm1: &pwm1,
		pwm2: &pwm2,
		pwm3: &pwm3,
		pwm4: &pwm4,
		servo1: &servo1,
		servo2: &servo2,
		servo3: &servo3,
		servo4: &servo4,
		uart1: &uart1,
		debug_led: &debug_led,
	};

	if (!resources.start()) {
		Error_Handler();
	}

	while (1){
//		for (int i = 0; i <= 180; i += 30) {
//			servo4.set_position_degree(i);
//			HAL_Delay(500);
//		}
////		debug_led.toggle();
//		for (int i = 180; i >= 0; i -= 30) {
//			servo4.set_position_degree(i);
//			HAL_Delay(500);
//		}
		uint8_t Test[20] = {'a','b','c','d','e','f','g','h','i','j',10,10,10,10,10,10,10,10,10,10}; //Data to send
		HAL_UART_Transmit(&u1,Test,sizeof(Test),10);// Sending in normal mode
		HAL_Delay(1000);
	}
}

void Error_Handler(void)
{
	__disable_irq();
	while (1){}
}
