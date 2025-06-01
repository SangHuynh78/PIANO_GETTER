/*
 * song.c
 *
 *  Created on: Jun 1, 2025
 *      Author: HTSANG
 */


#include "song.h"
#include "main.h"

uint32_t song_index = 0;
uint8_t song_data[SONG_DATA_SIZE_LONG];
uint8_t song_name[SONG_NAME_SIZE] = "New Song";

void song_erase(uint8_t song_index) {
	uint32_t block_size = song_index < 5 ? SONG_BLOCK_LONG : SONG_BLOCK_SHORT;
	for (uint32_t addr = SONG_NAME_START[song_index]; addr < SONG_NAME_START[song_index] + block_size; addr += 4096) {
	    GD25Q16E_EraseSector(&flash, addr);
	}
}

void song_write(GD25Q16E_Dev_t *flash, uint32_t song_index) {
    if (song_index >= SONG_COUNT) return;

    song_erase(song_index);

    GD25Q16E_Write(flash, SONG_NAME_START[song_index], song_name, SONG_NAME_SIZE);

    uint32_t data_size = song_index < 5 ? SONG_DATA_SIZE_LONG : SONG_DATA_SIZE_SHORT;
    GD25Q16E_Write(flash, SONG_DATA_START[song_index], song_data, data_size);
}
