/*
 * API_debounce.h
 *
 *  Created on: Jul 23, 2023
 *      Author: Nicolas Iriarte
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h" // bool_t

// Define the callback function signature
typedef void (*Callback)(void);

/**
 * SetUp the initial state for the FSM.
 */
void debounceFSM_init();

/**
 * This function must be called continuously. It will check the button state and the
 * exposed button state. It will update the state if needed.
 */
void debounceFSM_update();

/**
 * Return if the button is pressed or not.
 */
bool_t readKey();

void setPressedCallback(Callback callback);
void setReleasedCallback(Callback callback);

#endif /* API_INC_API_DEBOUNCE_H_ */
