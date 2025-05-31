/*
 * uart.h
 *
 *  Created on: May 9, 2025
 *      Author: HTSANG
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "stdio.h"
#include "stdint.h"
#include "stm32f1xx_ll_usart.h"

void UART1_LL_SendChar(uint8_t c);
void UART1_LL_SendString(const char* str);
uint8_t UART1_LL_ReceiveChar(void);

#endif /* UART_UART_H_ */
