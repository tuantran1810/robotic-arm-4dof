/*
 * uart.h
 *
 *  Created on: Jul 11, 2023
 *      Author: tuantran
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f1xx_hal.h"
#include "ring_buffer.h"

uint8_t uart1_tmp[20];
uint8_t uart2_tmp[20];
uint8_t uart3_tmp[20];

RingBuffer256 uart1_buffer = RingBuffer256();
RingBuffer256 uart2_buffer = RingBuffer256();
RingBuffer256 uart3_buffer = RingBuffer256();

void __process_message(uint8_t* tmp_buf, RingBuffer256* buffer) {
	// last byte must be added padding bytes;
	uint8_t added_padding_bytes = tmp_buf[19];
	if (added_padding_bytes > 19) {
		return;
	}
	if (!buffer->push(tmp_buf, 20 - added_padding_bytes)) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart) {
	if (uart->Instance == USART1) {
		__process_message(uart1_tmp, &uart1_buffer);
		HAL_UART_Receive_IT(uart, uart1_tmp, 20);
	} else if (uart->Instance == USART2) {
		__process_message(uart2_tmp, &uart2_buffer);
		HAL_UART_Receive_IT(uart, uart2_tmp, 20);
	} else if (uart->Instance == USART3) {
		__process_message(uart3_tmp, &uart3_buffer);
		HAL_UART_Receive_IT(uart, uart3_tmp, 20);
	}
}

struct UartInstance {
	UART_HandleTypeDef* uart;
	RingBuffer256* rx_buffer;

	UartInstance(UART_HandleTypeDef* uart) {
		this->uart = uart;
		if (uart->Instance == USART1) {
			this->rx_buffer = &uart1_buffer;
		} else if (uart->Instance == USART2) {
			this->rx_buffer = &uart2_buffer;
		} else if (uart->Instance == USART3) {
			this->rx_buffer = &uart3_buffer;
		}
	}

	void register_rx_callback(void) {
		if (this->uart->Instance == USART1) {
			HAL_UART_Receive_IT(this->uart, uart1_tmp, 20);
		} else if (this->uart->Instance == USART2) {
			HAL_UART_Receive_IT(this->uart, uart2_tmp, 20);
		} else if (this->uart->Instance == USART3) {
			HAL_UART_Receive_IT(this->uart, uart3_tmp, 20);
		}
	}

	uint8_t fetch_byte(void) {
		return this->rx_buffer->pop();
	}
};



#endif /* INC_UART_H_ */
