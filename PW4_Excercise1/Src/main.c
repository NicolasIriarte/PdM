/**
 ******************************************************************************
 * @file    UART/UART_Printf/Src/main.c
 * @author  MCD Application Team
 * @brief   This example shows how to retarget the C library printf function
 *          to the UART.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "main.h"
#include "API_delay.h"

/**
 * Define the debounce time for the FSM, the button state will be checked at this
 * time interval.
 */
#define DEBOUNCE_TIME_MS 40

UART_HandleTypeDef UartHandle;

/**
 * SetUp the initial state for the FSM.
 */
void debounceFSM_init();

/**
 * This function must be called continuosly. It will check the button state and the
 * exposed button state. It will update the state if needed.
 */
void debounceFSM_update();

/**
 * Both these functions will toggle the led LED1 and LED3 if the button is
 * pressed/released respectively.
 */
void buttonPressed();
void buttonReleased();

/**
 * Possible state of the button.
 */
typedef enum {
	BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING,
} debounceState_t;

/**
 * Global state of the button.
 */
static debounceState_t current_state;

// Forward declarations
static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void) {
	HAL_Init(); // Initialize HAL
	SystemClock_Config();

	// Configure LED pin (GPIOG Pin 13) as output
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED3);

	// Configure button pin (GPIOA Pin 0) as input with interrupt
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	// Leds delays
	delay_t FSM_delay;
	delay_t button_pressed_delay;
	delay_t button_released_delay;

	// Init delays
	delayInit(&FSM_delay, 0);
	delayInit(&button_pressed_delay, 0);
	delayInit(&button_released_delay, 0);

	debounceFSM_init();

	/* Infinite loop */
	while (1) {

		if (delayRead(&FSM_delay)) {
			delayWrite(&FSM_delay, DEBOUNCE_TIME_MS);
			debounceFSM_update();
		}

		if (delayRead(&button_pressed_delay)) {
			delayWrite(&button_pressed_delay, 200);
			buttonPressed();
		}

		if (delayRead(&button_released_delay)) {
			delayWrite(&button_released_delay, 200);
			buttonReleased();
		}
	}
}

void debounceFSM_init() {
	current_state = BUTTON_UP;
}

void debounceFSM_update() {
	// Check if the button is pressed to update the FSM if necessary.
	uint32_t is_button_pressed = BSP_PB_GetState(BUTTON_USER);

	/**
	 * Update the global state as described on the exercise.
	 */
	switch (current_state) {
	case BUTTON_UP:
		if (is_button_pressed) {
			current_state = BUTTON_FALLING;
		}
		break;
	case BUTTON_FALLING:
		if (is_button_pressed) {
			current_state = BUTTON_DOWN;
		} else {
			current_state = BUTTON_UP;
		}
		break;
	case BUTTON_DOWN:
		if (!is_button_pressed) {
			current_state = BUTTON_RAISING;
		}
		break;
	case BUTTON_RAISING:
		if (!is_button_pressed) {
			current_state = BUTTON_UP;
		} else {
			current_state = BUTTON_DOWN;
		}
		break;
	}
}

void buttonPressed() {
	if (current_state == BUTTON_DOWN) {
		BSP_LED_Toggle(LED1);
	} else if (current_state == BUTTON_UP) {
		// Catch the edge case where the LED stays turned on when the release sequesce is
		// active.
		BSP_LED_Off(LED1);
	}
}

void buttonReleased() {
	if (current_state == BUTTON_UP) {
		BSP_LED_Toggle(LED3);
	} else if (current_state == BUTTON_DOWN) {
		// Catch the edge case where the LED stays turned on when the pressed sequesce is
		// active.
		BSP_LED_Off(LED3);
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1) {
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
