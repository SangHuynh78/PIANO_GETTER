/*
 * gd25q16e.c
 *
 *  Created on: May 9, 2025
 *      Author: HTSANG
 */


#include "gd25q16e.h"

static void CS_LOW(GD25Q16E_Dev_t *flash) {
    LL_GPIO_ResetOutputPin(flash->cs_port, flash->cs_pin);
}

static void CS_HIGH(GD25Q16E_Dev_t *flash) {
    LL_GPIO_SetOutputPin(flash->cs_port, flash->cs_pin);
}

static void SPI_Send(GD25Q16E_Dev_t *flash, uint8_t data) {
    while (!LL_SPI_IsActiveFlag_TXE(flash->spi));
    LL_SPI_TransmitData8(flash->spi, data);
    while (!LL_SPI_IsActiveFlag_RXNE(flash->spi));
    (void)LL_SPI_ReceiveData8(flash->spi);
}

static uint8_t SPI_Transceive(GD25Q16E_Dev_t *flash, uint8_t data) {
    while (!LL_SPI_IsActiveFlag_TXE(flash->spi));
    LL_SPI_TransmitData8(flash->spi, data);
    while (!LL_SPI_IsActiveFlag_RXNE(flash->spi));
    return LL_SPI_ReceiveData8(flash->spi);
}

static void SPI_ReadBuffer(GD25Q16E_Dev_t *flash, uint8_t *buf, uint32_t len) {
    while (len--) {
        *buf++ = SPI_Transceive(flash, 0xFF);
    }
}

static void SPI_WriteBuffer(GD25Q16E_Dev_t *flash, const uint8_t *buf, uint32_t len) {
    while (len--) {
        SPI_Transceive(flash, *buf++);
    }
}

static void GD25Q16E_WriteEnable(GD25Q16E_Dev_t *flash) {
    CS_LOW(flash);
    SPI_Send(flash, GD25Q16E_CMD_WRITE_ENABLE);
    CS_HIGH(flash);
}

static uint8_t GD25Q16E_ReadStatus(GD25Q16E_Dev_t *flash) {
    CS_LOW(flash);
    SPI_Send(flash, GD25Q16E_CMD_READ_STATUS);
    uint8_t status = SPI_Transceive(flash, 0xFF);
    CS_HIGH(flash);
    return status;
}

static void GD25Q16E_WaitBusy(GD25Q16E_Dev_t *flash) {
    while (GD25Q16E_ReadStatus(flash) & 0x01);
}

void GD25Q16E_Init(GD25Q16E_Dev_t *flash) {
    CS_HIGH(flash);
}

uint32_t GD25Q16E_ReadID(GD25Q16E_Dev_t *flash) {
    CS_LOW(flash);
    SPI_Send(flash, GD25Q16E_CMD_READ_JEDEC_ID);
    uint8_t id1 = SPI_Transceive(flash, 0xFF);
    uint8_t id2 = SPI_Transceive(flash, 0xFF);
    uint8_t id3 = SPI_Transceive(flash, 0xFF);
    CS_HIGH(flash);
    return (id1 << 16) | (id2 << 8) | id3;
}

void GD25Q16E_Read(GD25Q16E_Dev_t *flash, uint32_t addr, uint8_t *data, uint32_t len) {
    CS_LOW(flash);
    SPI_Send(flash, GD25Q16E_CMD_READ_DATA);
    SPI_Send(flash, (addr >> 16) & 0xFF);
    SPI_Send(flash, (addr >> 8) & 0xFF);
    SPI_Send(flash, addr & 0xFF);
    SPI_ReadBuffer(flash, data, len);
    CS_HIGH(flash);
}

void GD25Q16E_Write(GD25Q16E_Dev_t *flash, uint32_t addr, const uint8_t *data, uint32_t len) {
    while (len > 0) {
        uint32_t page_offset = addr % GD25Q16E_PAGE_SIZE;
        uint32_t write_size = GD25Q16E_PAGE_SIZE - page_offset;
        if (write_size > len) write_size = len;

        GD25Q16E_WriteEnable(flash);

        CS_LOW(flash);
        SPI_Send(flash, GD25Q16E_CMD_PAGE_PROGRAM);
        SPI_Send(flash, (addr >> 16) & 0xFF);
        SPI_Send(flash, (addr >> 8) & 0xFF);
        SPI_Send(flash, addr & 0xFF);
        SPI_WriteBuffer(flash, data, write_size);
        CS_HIGH(flash);

        GD25Q16E_WaitBusy(flash);

        addr += write_size;
        data += write_size;
        len -= write_size;
    }
}

void GD25Q16E_EraseSector(GD25Q16E_Dev_t *flash, uint32_t addr) {
    GD25Q16E_WriteEnable(flash);
    CS_LOW(flash);
    SPI_Send(flash, GD25Q16E_CMD_SECTOR_ERASE);
    SPI_Send(flash, (addr >> 16) & 0xFF);
    SPI_Send(flash, (addr >> 8) & 0xFF);
    SPI_Send(flash, addr & 0xFF);
    CS_HIGH(flash);
    GD25Q16E_WaitBusy(flash);
}

void GD25Q16E_ChipErase(GD25Q16E_Dev_t *flash) {
    GD25Q16E_WriteEnable(flash);
    CS_LOW(flash);
    SPI_Send(flash, GD25Q16E_CMD_CHIP_ERASE);
    CS_HIGH(flash);
    GD25Q16E_WaitBusy(flash);
}
