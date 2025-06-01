/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gd25q16e.h"
#include "ttp229.h"
#include "uart.h"
#include "song.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum {
    STATE_IDLE,
    STATE_WAIT_SW2_RELEASE,
    STATE_RECORD,
    STATE_UART_WAIT_SEND,
    STATE_UART_SENDING,
    STATE_WAIT_ERASE
} State_t;

struct GD25Q16E_Dev flash = {
	.spi = SPI1,
	.cs_port = FL_CS_GPIO_Port,
	.cs_pin = FL_CS_Pin
};

#define FLASH_END_ADDR				0x06DFFF
#define UART_CHUNK_SIZE				100
#define LED_TOGGLE_TICKS			50
#define DEBOUNCE_TICKS				5

static State_t current_state = STATE_IDLE;
static uint32_t sample_count = 0;
static uint32_t sw1_hold_time = 0;
static uint32_t sw2_hold_time = 0;
static uint32_t sw1_debounce = 0;
static uint32_t sw2_debounce = 0;
static bool sw1_state = false;
static bool sw2_state = false;
static uint32_t uart_wait_delay = 0;
static uint32_t erase_wait_delay = 0;
volatile uint32_t timer_count = 0;
static uint32_t flash_addr = 0;
static uint32_t led_toggle_count = 0;


void UART_Transmit_Flash(uint32_t start_addr, uint32_t len) {
    uint8_t buffer[UART_CHUNK_SIZE];
    uint32_t remaining = len - (flash_addr - start_addr);
    uint32_t chunk = (remaining > UART_CHUNK_SIZE) ? UART_CHUNK_SIZE : remaining;

    if (chunk > 0) {
        GD25Q16E_Read(&flash, flash_addr, buffer, chunk);
        for (uint32_t i = 0; i < chunk; i++) {
            while (!LL_USART_IsActiveFlag_TXE(USART1));
            LL_USART_TransmitData8(USART1, buffer[i]);
        }
        flash_addr += chunk;
    }
}

void EraseAllFlash(void) {
    for (uint32_t addr = 0x000000; addr <= FLASH_END_ADDR; addr += 4096) {
        GD25Q16E_EraseSector(&flash, addr);
        // Chớp LED đỏ
        LL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
        LL_mDelay(50); // Đợi ngắn để chớp LED
    }
}

void Timer2_10ms_handle(void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
        LL_TIM_ClearFlag_UPDATE(TIM2);
        timer_count++;
        led_toggle_count++;

        // Debouncing SW1
        bool sw1_current = !LL_GPIO_IsInputPinSet(SW1_GPIO_Port, SW1_Pin);
        if (sw1_current == sw1_state) {
            sw1_debounce = 0;
        } else {
            sw1_debounce++;
            if (sw1_debounce >= DEBOUNCE_TICKS) {
                sw1_state = sw1_current;
                sw1_debounce = 0;
                if (!sw1_state) {
                    sw1_hold_time = 0;
                }
            }
        }
        if (sw1_state) {
            sw1_hold_time++;
        }

        // Debouncing SW2
        bool sw2_current = !LL_GPIO_IsInputPinSet(SW2_GPIO_Port, SW2_Pin);
        if (sw2_current == sw2_state) {
            sw2_debounce = 0;
        } else {
            sw2_debounce++;
            if (sw2_debounce >= DEBOUNCE_TICKS) {
                sw2_state = sw2_current;
                sw2_debounce = 0;
                if (!sw2_state) {
                    sw2_hold_time = 0;
                }
            }
        }
        if (sw2_state) {
            sw2_hold_time++;
        }

        // Nhấp nháy LED trong UART_SENDING và WAIT_ERASE
        if ((current_state == STATE_UART_SENDING || current_state == STATE_WAIT_ERASE) && led_toggle_count >= LED_TOGGLE_TICKS) {
            if (current_state == STATE_UART_SENDING) {
                LL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
            } else {
                LL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
                LL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
                LL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
            }
            led_toggle_count = 0;
        }

        // Xử lý trạng thái
        switch (current_state) {
            case STATE_IDLE:
                LL_GPIO_SetOutputPin(LED_R_GPIO_Port, LED_R_Pin);
                LL_GPIO_ResetOutputPin(LED_G_GPIO_Port, LED_G_Pin);
                LL_GPIO_ResetOutputPin(LED_B_GPIO_Port, LED_B_Pin);
                if (sw2_hold_time == 1) {
                    current_state = STATE_WAIT_SW2_RELEASE;
                    sw2_debounce = 0;
                    sw2_state = true;
                } else if (sw1_hold_time >= 500) { // 5 giây
                    current_state = STATE_WAIT_ERASE;
                	LL_GPIO_ResetOutputPin(LED_R_GPIO_Port, LED_R_Pin);
                    sw1_hold_time = 0;
                    erase_wait_delay = 0;
                }
                break;

            case STATE_WAIT_SW2_RELEASE:
                if (sw2_hold_time >= 300) {
                    current_state = STATE_UART_WAIT_SEND;
                    sw2_hold_time = 0;
                    uart_wait_delay = 0;
                } else if (sw2_hold_time == 0) {
                    current_state = STATE_RECORD;
                    sample_count = 0;
                }
                break;

            case STATE_RECORD:
                LL_GPIO_ResetOutputPin(LED_R_GPIO_Port, LED_R_Pin);
                LL_GPIO_SetOutputPin(LED_G_GPIO_Port, LED_G_Pin);
                LL_GPIO_ResetOutputPin(LED_B_GPIO_Port, LED_B_Pin);
                if (sw1_hold_time == 1 || sample_count >= SONG_DATA_SIZE_LONG) {
                    char name[16];
                    memset(song_name, 0xFF, SONG_NAME_SIZE);
                    snprintf(name, sizeof(name), "Bai so %lu", song_index + 1);
                    memcpy(song_name, name, strlen(name) + 1);
                    song_write(&flash, song_index);
                    song_index = (song_index + 1) % SONG_COUNT;
                    current_state = STATE_IDLE;
                } else {
                    if (sample_count < SONG_DATA_SIZE_LONG) {
                        song_data[sample_count] = TTP229_Read_8Keys();
                        sample_count++;
                    }
                }
                break;

            case STATE_UART_WAIT_SEND:
                LL_GPIO_ResetOutputPin(LED_R_GPIO_Port, LED_R_Pin);
                LL_GPIO_ResetOutputPin(LED_G_GPIO_Port, LED_G_Pin);
                LL_GPIO_SetOutputPin(LED_B_GPIO_Port, LED_B_Pin);
                uart_wait_delay++;
                if (uart_wait_delay >= 50 && sw2_hold_time == 1) {
                    current_state = STATE_UART_SENDING;
                    flash_addr = 0x000000;
                    led_toggle_count = 0;
                } else if (uart_wait_delay >= 50 && sw1_hold_time == 1) {
                	current_state = STATE_IDLE;
                }

                break;

            case STATE_UART_SENDING:
                UART_Transmit_Flash(0x000000, FLASH_END_ADDR + 1);
                if (flash_addr > FLASH_END_ADDR) {
                    current_state = STATE_IDLE;
                }
                break;

            case STATE_WAIT_ERASE:
                erase_wait_delay++;
                if (erase_wait_delay >= 50 && sw2_state && sw2_hold_time == 1) {
                    EraseAllFlash();
                    current_state = STATE_IDLE;
                } else if (erase_wait_delay >= 50 && sw1_state && sw1_hold_time == 1) {
                    current_state = STATE_IDLE;
                }
                break;
        }
    }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  GD25Q16E_Init(&flash);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  uint8_t key_state = 0;
//  char buffer[100];
  while (1)
  {

//	  key_state = TTP229_Read_8Keys();
//	  LL_mDelay(10);
//
//	  if (key_state != 0) {
//		  snprintf(buffer, sizeof(buffer), "0x%02x  ", key_state);
//		  UART1_LL_SendString(buffer);
//	  }
//
//	  LL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
//	  LL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
//	  LL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
//
//	  if (!LL_GPIO_IsInputPinSet(SW1_GPIO_Port, SW1_Pin)) {
//		  uint32_t Flash_ID = GD25Q16E_ReadID(&flash);
//
//		  snprintf(buffer, sizeof(buffer), "Flash ID: 0x%06X \r\n", (unsigned int)Flash_ID);
//		  UART1_LL_SendString(buffer);
//	  }
//	  if (!LL_GPIO_IsInputPinSet(SW2_GPIO_Port, SW2_Pin)) {
//		  /* Write data to flash */
//		  uint8_t write_data[] = "Hello, Sang!";
//		  uint32_t write_addr = 0x000000; // Start at address 0
//		  snprintf(buffer, sizeof(buffer), "Writing data to address 0x%06X: %s\r\n", (unsigned int)write_addr, write_data);
//		  UART1_LL_SendString(buffer);
//		  GD25Q16E_Write(&flash, write_addr, write_data, strlen((char*)write_data) + 1);
//
//		  //song_erase(0);
//
//		  /* Read data back */
//		  uint8_t read_data[32] = {0};
//		  snprintf(buffer, sizeof(buffer), "Reading data from address 0x%06X...\r\n", (unsigned int)write_addr);
//		  UART1_LL_SendString(buffer);
//		  GD25Q16E_Read(&flash, write_addr, read_data, strlen((char*)write_data) + 1);
//		  snprintf(buffer, sizeof(buffer), "Data read: %s\r\n", read_data);
//		  UART1_LL_SendString(buffer);
//
//	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_6);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(24000000);
  LL_SetSystemCoreClock(24000000);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = FL_SCK_Pin|FL_MOSI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FL_MISO_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(FL_MISO_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  /* USER CODE BEGIN SPI1_Init 2 */
  LL_SPI_Enable(SPI1);
  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 239;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  LL_TIM_EnableIT_UPDATE(TIM2); // Bật ngắt Update
  LL_TIM_EnableCounter(TIM2);   // Khởi động timer

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  TIM_InitStruct.Prescaler = 23;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM4, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM4);
  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM4);
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(FL_CS_GPIO_Port, FL_CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(TTP_CLK_GPIO_Port, TTP_CLK_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LED_B_Pin|LED_G_Pin|LED_R_Pin);

  /**/
  GPIO_InitStruct.Pin = FL_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(FL_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SW2_Pin|TTP_SDO_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = TTP_CLK_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(TTP_CLK_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_B_Pin|LED_G_Pin|LED_R_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = KQ0_Pin|KQ1_Pin|KQ2_Pin|KQ3_Pin
                          |KQ4_Pin|KQ5_Pin|KQ6_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
