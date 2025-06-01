/*
 * song.c
 *
 *  Created on: Jun 1, 2025
 *      Author: HTSANG
 */


#include "song.h"
#include "main.h"

uint8_t song_index;
uint8_t song_name[SONG_NAME_SIZE];
uint8_t song_data[SONG_DATA_SIZE];

void song_erase(uint8_t song_index) {
    if (song_index >= SONG_COUNT) return;

    for (uint32_t addr = SONG_NAME_START[song_index]; addr < SONG_NAME_START[song_index] + SONG_BLOCK; addr += 4096) {
        GD25Q16E_EraseSector(&flash, addr);
    }
}

void song_save_index(GD25Q16E_Dev_t *flash, uint8_t index) {
    GD25Q16E_EraseSector(flash, INDEX_STORAGE_START);
    GD25Q16E_Write(flash, INDEX_STORAGE_START, (uint8_t *)&index, sizeof(index));
}

uint8_t song_read_index(GD25Q16E_Dev_t *flash) {
	uint8_t index;
    GD25Q16E_Read(flash, INDEX_STORAGE_START, (uint8_t *)&index, sizeof(index));
    if (index >= SONG_COUNT || index == 0xFF) {
        return 0;
    }
    return index;
}

void song_write(GD25Q16E_Dev_t *flash, uint8_t song_index) {
    if (song_index >= SONG_COUNT) return;

    song_erase(song_index);

    GD25Q16E_Write(flash, SONG_NAME_START[song_index], song_name, SONG_NAME_SIZE);

    GD25Q16E_Write(flash, SONG_DATA_START[song_index], song_data, SONG_DATA_SIZE);
}



