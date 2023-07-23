/**
 ******************************************************************************
 * @file    UART/UART_Printf/Inc/main.h
 * @author  MCD Application Team
 * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"        /* <- HAL include */
#include "stm32f4xx_nucleo_144.h" /* <- BSP include */
#include <stdio.h>

#include <stdbool.h> // for bool_t
#include <stdint.h>  // for uint32_t

// What library must be included for this to compile?
typedef uint32_t tick_t;

// What library must be included for this to compile?
typedef bool bool_t;

/**
 * This struct represent a delay. It is intended to be used by the functions
 * provided on this file.
 */
typedef struct {
  tick_t startTime;
  tick_t duration; // Duration on ms
  bool_t running;
} delay_t;

/**
 * delayInit will load the delay duration value into the structure, in the
 * corresponding field.
 *
 * It will not start the delay count-down.
 * It will initialize the running flag to `false´.
 */
void delayInit(delay_t *delay, tick_t duration_ms);

/**
 * delayRead will check the status of the running flag.  If it is false, it will
 * take a timestamp and change running to 'true'.
 *
 * If it is true, it will calculate whether the delay time has been reached or
 * not.
 *
 * It will return a boolean value indicating whether the time has been reached
 * or not.  When the time is reached, it will change the running flag to false.
 */
bool_t delayRead(delay_t *delay);

/**
 * delayWrite will allow changing the duration time of an existing delay.
 */
void delayWrite(delay_t *delay, tick_t duration_ms);

/* User can use this section to tailor USARTx/UARTx instance used and associated
 resources */
/* Definition for USARTx clock resources */
#define USARTx USART3
#define USARTx_CLK_ENABLE() __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET() __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET() __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN GPIO_PIN_8
#define USARTx_TX_GPIO_PORT GPIOD
#define USARTx_TX_AF GPIO_AF7_USART3
#define USARTx_RX_PIN GPIO_PIN_9
#define USARTx_RX_GPIO_PORT GPIOD
#define USARTx_RX_AF GPIO_AF7_USART3

#endif /* __MAIN_H */
