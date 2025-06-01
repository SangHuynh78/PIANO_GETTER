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

extern uint32_t song_index;
extern uint8_t song_data[SONG_DATA_SIZE_LONG];
extern uint8_t song_name[SONG_NAME_SIZE];

void song_erase(uint8_t song_index);
void song_write(GD25Q16E_Dev_t *flash, uint32_t song_index);

#endif /* SONG_SONG_H_ */
