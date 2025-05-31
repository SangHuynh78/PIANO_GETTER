/*
 * tpp229.c
 *
 *  Created on: May 9, 2025
 *      Author: HTSANG
 */


#include "ttp229.h"

#define Set_GPIO_OUTPUT() 	LL_GPIO_SetPinMode(TTP_SDO_GPIO_Port, TTP_SDO_Pin, LL_GPIO_MODE_OUTPUT)
#define Set_GPIO_INPUT() 	LL_GPIO_SetPinMode(TTP_SDO_GPIO_Port, TTP_SDO_Pin, LL_GPIO_MODE_INPUT)

static void TTP229_Delay_us(uint32_t us) {
    // Set counter to 65535 - us (overflow after 'us' microseconds)
    LL_TIM_SetCounter(TIM4, 65535 - us);
    // Clear update flag
    LL_TIM_ClearFlag_UPDATE(TIM4);
    // Start the timer
    LL_TIM_EnableCounter(TIM4);
    // Wait for overflow (UIF set)
    while (!LL_TIM_IsActiveFlag_UPDATE(TIM4));
    // Stop the timer
    LL_TIM_DisableCounter(TIM4);
}

uint8_t TTP229_Read_8Keys(void) {
//    uint8_t key_state = 0;
//    Set_GPIO_OUTPUT();
//    LL_GPIO_SetOutputPin(TTP_SDO_GPIO_Port, TTP_SDO_Pin);
//    TTP229_Delay_us(100);
//    LL_GPIO_ResetOutputPin(TTP_SDO_GPIO_Port, TTP_SDO_Pin);
//    Set_GPIO_INPUT();
//    TTP229_Delay_us(15);
//
//    for (uint8_t i = 0; i < 7; i++)
//    {
//        LL_GPIO_SetOutputPin(TTP_CLK_GPIO_Port, TTP_CLK_Pin);
//        TTP229_Delay_us(40);
//        LL_GPIO_ResetOutputPin(TTP_CLK_GPIO_Port, TTP_CLK_Pin);
//        if (LL_GPIO_IsInputPinSet(TTP_SDO_GPIO_Port, TTP_SDO_Pin))
//        {
//            key_state |= (0x01 << i);
//        }
//        TTP229_Delay_us(50);
//    }
//    return key_state;

    uint8_t key_state = 0;
    if (!LL_GPIO_IsInputPinSet(KQ0_GPIO_Port, KQ0_Pin))
    	key_state |= 1;
    if (!LL_GPIO_IsInputPinSet(KQ1_GPIO_Port, KQ1_Pin))
		key_state |= 2;
    if (!LL_GPIO_IsInputPinSet(KQ2_GPIO_Port, KQ2_Pin))
		key_state |= 4;
    if (!LL_GPIO_IsInputPinSet(KQ3_GPIO_Port, KQ3_Pin))
		key_state |= 8;
    if (!LL_GPIO_IsInputPinSet(KQ4_GPIO_Port, KQ4_Pin))
		key_state |= 16;
    if (!LL_GPIO_IsInputPinSet(KQ5_GPIO_Port, KQ5_Pin))
		key_state |= 32;
    if (!LL_GPIO_IsInputPinSet(KQ6_GPIO_Port, KQ6_Pin))
		key_state |= 64;
    return key_state;
}
