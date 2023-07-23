/*
 * API_delay.h
 *
 *  Created on: Jul 6, 2023
 *      Author: Nicolas Iriarte
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

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

#endif /* API_INC_API_DELAY_H_ */
