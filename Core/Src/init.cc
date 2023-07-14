#include "stm32f1xx_hal.h"
#include "init.h"
#include "led.h"

UART_HandleTypeDef* global_huart1;
I2C_HandleTypeDef* global_hi2c1;

void bring_up(void) {
	HAL_Init();
}

bool init_system_clock(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		return false;
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		return false;
	}
	return true;
}

void init_gpio(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

bool init_timer_1(TIM_HandleTypeDef* htim1)
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	if (!htim1) {
		return false;
	}

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	htim1->Instance = TIM1;
	htim1->Init.Prescaler = 144;
	htim1->Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1->Init.Period = 10000-1;
	htim1->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1->Init.RepetitionCounter = 0;
	htim1->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(htim1) != HAL_OK) {
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(htim1, &sMasterConfig) != HAL_OK) {
		return false;
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		return false;
	}
	if (HAL_TIM_PWM_ConfigChannel(htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
		return false;
	}
	if (HAL_TIM_PWM_ConfigChannel(htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
		return false;
	}
	if (HAL_TIM_PWM_ConfigChannel(htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
		return false;
	}

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

	if (HAL_TIMEx_ConfigBreakDeadTime(htim1, &sBreakDeadTimeConfig) != HAL_OK) {
		return false;
	}

	HAL_TIM_MspPostInit(htim1);
	return true;
}

bool init_uart_1(UART_HandleTypeDef* huart1) {
	if (!huart1) {
		return false;
	}
	huart1->Instance = USART1;
	huart1->Init.BaudRate = 115200;
	huart1->Init.WordLength = UART_WORDLENGTH_8B;
	huart1->Init.StopBits = UART_STOPBITS_1;
	huart1->Init.Parity = UART_PARITY_NONE;
	huart1->Init.Mode = UART_MODE_TX_RX;
	huart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1->Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(huart1) != HAL_OK) {
		return false;
	}
	HAL_UART_MspInit(huart1);
	global_huart1 = huart1;
	return true;
}

bool init_i2c_1(I2C_HandleTypeDef* hi2c1) {
	hi2c1->Instance = I2C1;
	hi2c1->Init.ClockSpeed = 100000;
	hi2c1->Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1->Init.OwnAddress1 = 0;
	hi2c1->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1->Init.OwnAddress2 = 0;
	hi2c1->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(hi2c1) != HAL_OK) {
		return false;
	}
	return true;
}
