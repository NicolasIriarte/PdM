/*
 * API_delay.c
 *
 *  Created on: Jul 23, 2023
 *      Author: Nicolas Iriarte
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "API_debounce.h"

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

/**
 * @brief Private global boolean variable that is set to true on a falling edge event
 *        and set to false when the function readKey() is called.
 *
 * This variable is used to track the occurrence of a falling edge event on a particular
 * input signal. When the falling edge event happens, the bool_t variable is set to true.
 * Subsequently, when the readKey() function is called, it resets the bool_t variable to false,
 * indicating that the falling edge event has been processed or acknowledged.
 *
 * @note This variable should only be accessed and modified within the scope of the relevant module
 *       or class to ensure proper encapsulation and prevent unintended interference from other
 *       parts of the code.
 */
static bool_t is_falling_edge_event = false;

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
			is_falling_edge_event = true;
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

bool_t readKey() {
	bool_t return_value = is_falling_edge_event;
	is_falling_edge_event = false;
	return return_value;
}

