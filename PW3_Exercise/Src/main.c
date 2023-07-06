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

UART_HandleTypeDef UartHandle;

// Forward declarations
static void SystemClock_Config(void);
static void Error_Handler(void);

/// This is a joke! Lets program a scheduler. Uncomment this line if you want that implementation.
// #define NEI_SCHEDULER

#ifdef NEI_SCHEDULER

// Define a MAX number of task supported by the scheduler.
// Defining this variable makes the size of the scheduler defined at
// compile-time, preventing unexpected mallocs or infinite tasks, which makes
// debbuging difficult
#define MAX_TASKS 64

// Helper type to keep track of the added event. Not used by now, but it may be
// useful if you want to make a change on a task (ex: removing it from the
// scheduler).
typedef uint32_t event_id;

// A task is a struct with a callback and a delay for repetitions.
// delay_t should be a more complex struct if we want to handle scenarios of
// finite time tasks.
typedef struct {
	void (*callback)(delay_t *delay);
	delay_t delay;
} task_t;

typedef struct {
	task_t tasks[MAX_TASKS]; // Fixed size to prevent mallocs
	// Keep track of how many tasks are active on the scheduler.
	event_id active_tasks;
} nei_scheduler_t;

// Init the scheduler, setting the number of active_tasks to zero, and all the
// defaults pointers to NULL. Also initialize the delay of all tasks to zero.

// IMPORTANT: This function must be called a single time.
void nei_schedulerInit(nei_scheduler_t *scheduler) {
	for (int i = 0; i < MAX_TASKS; ++i) {
		scheduler->tasks[i].callback = NULL; // Be sure to be NULL
		delayInit(&scheduler->tasks[i].delay, 0); // Grants running = false
	}

	scheduler->active_tasks = 0;
}

// Add a periodic event to the scheduler. The callback will be executed each
// given repeat_interval_ms.
// returns: 0 if the scheduler is full of tasks and fails to add, or a unique
//          event_id which identifies the task on the scheduler.
static event_id nei_schedulerAddPeriodicEvent(nei_scheduler_t *scheduler,
		void (*callback)(delay_t *delay), tick_t repeat_interval_ms) {

	if (scheduler->active_tasks == MAX_TASKS) {
		return 0; // Scheduler is full
	}

	task_t *task = &scheduler->tasks[scheduler->active_tasks];

	task->callback = callback;
	delayInit(&task->delay, repeat_interval_ms);

	++scheduler->active_tasks; // New task were added
	return scheduler->active_tasks; // return a unique id of the added task
}

// Check if any task needs to be executed and do that. This function should be
// called on every tick of the run. Waiting for tasks is a non-cloking operation
static int nei_schedulerTick(nei_scheduler_t *scheduler) {
	int executed_tasks = 0;
	for (int i = 0; i < scheduler->active_tasks; ++i) {
		task_t *task = &scheduler->tasks[i];
		if (delayRead(&task->delay)) {
			task->callback(&task->delay);
			++executed_tasks;
		}
	}
	return executed_tasks;
}

// Define some function to be emplaced on the scheduler. As the exercise is very
// simple all three functions seems almost equals. On a real-world application
// the would look very different.

static void toggleLed1(delay_t *_) {
	BSP_LED_Toggle(LED1);
}

static void toggleLed2(delay_t *_) {
	BSP_LED_Toggle(LED2);
}

static void toggleLed3(delay_t *_) {
	BSP_LED_Toggle(LED3);
}

// TODO(Nico): This implementation needs a way to remove events from the
//             scheduler.  But as is almost a joke, I will add the feature when
//             I need it.

// Cheers!
#endif

int main(void) {
	HAL_Init();  // Initialize HAL
	SystemClock_Config();

// Configure LED pin (GPIOG Pin 13) as output
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);

#ifndef NEI_SCHEDULER
// Leds delays
	delay_t leds_delay[LEDn];

// Init delays
	delayInit(&leds_delay[0], 100);
	delayInit(&leds_delay[1], 500);
	delayInit(&leds_delay[2], 1000);

	/* Infinite loop */
	while (1) {
		for (int led_id = 0; led_id < LEDn; ++led_id) {
			if (delayRead(&leds_delay[led_id])) {
				BSP_LED_Toggle(led_id);
			}
		}
	}
#else
	nei_scheduler_t scheduler;
	nei_schedulerInit(&scheduler);

	nei_schedulerAddPeriodicEvent(&scheduler, toggleLed1, 100);
	nei_schedulerAddPeriodicEvent(&scheduler, toggleLed2, 500);
	nei_schedulerAddPeriodicEvent(&scheduler, toggleLed3, 1000);

	/* Infinite loop */
	while (1) {
		nei_schedulerTick(&scheduler);
	}
#endif
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
