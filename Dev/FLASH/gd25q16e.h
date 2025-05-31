/*
 * gd25q16e.h
 *
 *  Created on: May 9, 2025
 *      Author: HTSANG
 */

#ifndef FLASH_GD25Q16E_H_
#define FLASH_GD25Q16E_H_

#include "main.h"

// Các lệnh SPI
#define GD25Q16E_CMD_WRITE_ENABLE      0x06
#define GD25Q16E_CMD_READ_STATUS       0x05
#define GD25Q16E_CMD_READ_JEDEC_ID     0x9F
#define GD25Q16E_CMD_PAGE_PROGRAM      0x02
#define GD25Q16E_CMD_READ_DATA         0x03
#define GD25Q16E_CMD_SECTOR_ERASE      0x20
#define GD25Q16E_CMD_CHIP_ERASE        0xC7

// Thông số
#define GD25Q16E_PAGE_SIZE             256
#define GD25Q16E_SECTOR_SIZE           4096

// SPI & GPIO cấu hình
typedef struct GD25Q16E_Dev {
    SPI_TypeDef *spi;
    GPIO_TypeDef *cs_port;
    uint32_t cs_pin;
} GD25Q16E_Dev_t;

void GD25Q16E_Init(GD25Q16E_Dev_t *flash);
uint32_t GD25Q16E_ReadID(GD25Q16E_Dev_t *flash);
void GD25Q16E_Read(GD25Q16E_Dev_t *flash, uint32_t addr, uint8_t *data, uint32_t len);
void GD25Q16E_Write(GD25Q16E_Dev_t *flash, uint32_t addr, const uint8_t *data, uint32_t len);
void GD25Q16E_EraseSector(GD25Q16E_Dev_t *flash, uint32_t addr);
void GD25Q16E_ChipErase(GD25Q16E_Dev_t *flash);

#endif /* FLASH_GD25Q16E_H_ */
