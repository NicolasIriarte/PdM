/*
 * API_delay.c
 *
 *  Created on: Jul 6, 2023
 *      Author: Nicolas Iriarte
 */

#include "API_delay.h"
#include "stm32f4xx_hal.h" // For HAL_GetTick()
#include <assert.h>        // For assert
#include <stddef.h>        // For NULL

void delayInit(delay_t *delay, tick_t duration_ms) {
  assert(delay != NULL);
  delay->duration = duration_ms;
  delay->running = false;
}

bool_t delayRead(delay_t *delay) {
  assert(delay != NULL);
  if (!delay->running) {
    delay->startTime = HAL_GetTick();
    delay->running = true;
  }

  bool_t finished = (HAL_GetTick() - delay->startTime) >= delay->duration;
  if (finished) {
    delay->running = false;
    return true;
  }

  return false;
}

void delayWrite(delay_t *delay, tick_t duration_ms) {
  assert(delay != NULL);
  delay->duration = duration_ms;
}
