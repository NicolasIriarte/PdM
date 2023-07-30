/*
 * API_uart.h
 *
 *  Created on: Jul 28, 2023
 *      Author: nico
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdbool.h>
#include <stdint.h>

typedef bool bool_t;

/**
 * Perform all UART initialization and print through the serial terminal a message
 * with its configuration parameters.
 *
 * The function returns:
 * - true: if the initialization is successful.
 * - false: if the initialization is not successful.
 */
bool_t uartInit();

/**
 * Receives a pointer to a string to be sent over the entire UART (up to the
 * character '\0'). It use the HAL function HAL_UART_Transmit(...) behind the
 * escene to transmit the string.
 */
void uartSendString(uint8_t *pstring);

/**
 * Receives a pointer to a string to be sent via the UART and an integer with the
 * number of characters to send. The function uses HAL_UART_Transmit(...) behind
 * the escene to transmit the string.
 */
void uartSendStringSize(uint8_t *pstring, uint16_t size);

/**
 * Receives a string from the UART of size `size` and store it on `pString`. The
 * function uses HAL_UART_Receive(...) under the hood.
 *
 */
void uartReceiveStringSize(uint8_t *pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
