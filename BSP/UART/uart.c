/*
 * uart.c
 *
 *  Created on: May 9, 2025
 *      Author: HTSANG
 */


#include "uart.h"

void UART1_LL_SendChar(uint8_t c) {
    while (!LL_USART_IsActiveFlag_TXE(USART1));
    LL_USART_TransmitData8(USART1, c);
}

void UART1_LL_SendString(const char* str) {
    while (*str) {
        UART1_LL_SendChar(*str++);
    }
}

uint8_t UART1_LL_ReceiveChar(void) {
    while (!LL_USART_IsActiveFlag_RXNE(USART1));
    return LL_USART_ReceiveData8(USART1);
}

void UART_Transmit(uint8_t *data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, data[i]);
    }
}
