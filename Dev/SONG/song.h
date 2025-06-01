/*
 * song.h
 *
 *  Created on: Jun 1, 2025
 *      Author: HTSANG
 */

#ifndef SONG_SONG_H_
#define SONG_SONG_H_

#include "song_map.h"
#include "gd25q16e.h"

extern uint8_t song_index;
extern uint8_t song_name[SONG_NAME_SIZE];
extern uint8_t song_data[SONG_DATA_SIZE];

void song_erase(uint8_t song_index);
void song_save_index(GD25Q16E_Dev_t *flash, uint8_t index);
uint8_t song_read_index(GD25Q16E_Dev_t *flash);
void song_write(GD25Q16E_Dev_t *flash, uint8_t song_index);


#endif /* SONG_SONG_H_ */
