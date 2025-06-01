/*
 * song_map.h
 *
 *  Created on: Jun 1, 2025
 *      Author: HTSANG
 *
 * Header file defining start addresses for song names and sample data for 50 songs.
 * Songs 0-4 (2 minutes): 512 bytes name + 12,000 bytes data, aligned to 0x4000 (16,384 bytes).
 * Songs 5-49 (1 minute): 512 bytes name + 6,000 bytes data, aligned to 0x2000 (8,192 bytes).
 * Sample data offset: 0x200 (512 bytes) from name start.
 * Total storage: 450,560 bytes.
 */

#ifndef SONG_SONG_MAP_H_
#define SONG_SONG_MAP_H_

#define SONG_COUNT          	50
#define SONG_NAME_SIZE      	512   		// Bytes for song name
#define SONG_DATA_SIZE_LONG 	12000 		// Bytes for sample data (2-minute songs, 0-4)
#define SONG_DATA_SIZE_SHORT 	6000 		// Bytes for sample data (1-minute songs, 5-49)
#define SONG_BLOCK_LONG     	16384 		// Aligned block for 2-minute songs (0x4000)
#define SONG_BLOCK_SHORT    	8192  		// Aligned block for 1-minute songs (0x2000)
#define SONG_DATA_OFFSET    	0x200 		// Offset for data from name start

#include "stdint.h"

// Array of start addresses for song names
static const uint32_t SONG_NAME_START[SONG_COUNT] = {
    0x000000, // Song 0 name (2 min)
    0x004000, // Song 1 name (2 min)
    0x008000, // Song 2 name (2 min)
    0x00C000, // Song 3 name (2 min)
    0x010000, // Song 4 name (2 min)
    0x014000, // Song 5 name (1 min)
    0x016000, // Song 6 name
    0x018000, // Song 7 name
    0x01A000, // Song 8 name
    0x01C000, // Song 9 name
    0x01E000, // Song 10 name
    0x020000, // Song 11 name
    0x022000, // Song 12 name
    0x024000, // Song 13 name
    0x026000, // Song 14 name
    0x028000, // Song 15 name
    0x02A000, // Song 16 name
    0x02C000, // Song 17 name
    0x02E000, // Song 18 name
    0x030000, // Song 19 name
    0x032000, // Song 20 name
    0x034000, // Song 21 name
    0x036000, // Song 22 name
    0x038000, // Song 23 name
    0x03A000, // Song 24 name
    0x03C000, // Song 25 name
    0x03E000, // Song 26 name
    0x040000, // Song 27 name
    0x042000, // Song 28 name
    0x044000, // Song 29 name
    0x046000, // Song 30 name
    0x048000, // Song 31 name
    0x04A000, // Song 32 name
    0x04C000, // Song 33 name
    0x04E000, // Song 34 name
    0x050000, // Song 35 name
    0x052000, // Song 36 name
    0x054000, // Song 37 name
    0x056000, // Song 38 name
    0x058000, // Song 39 name
    0x05A000, // Song 40 name
    0x05C000, // Song 41 name
    0x05E000, // Song 42 name
    0x060000, // Song 43 name
    0x062000, // Song 44 name
    0x064000, // Song 45 name
    0x066000, // Song 46 name
    0x068000, // Song 47 name
    0x06A000, // Song 48 name
    0x06C000 // Song 49 name
};

// Array of start addresses for song sample data
static const uint32_t SONG_DATA_START[SONG_COUNT] = {
    0x000200, // Song 0 data (2 min)
    0x004200, // Song 1 data (2 min)
    0x008200, // Song 2 data (2 min)
    0x00C200, // Song 3 data (2 min)
    0x010200, // Song 4 data (2 min)
    0x014200, // Song 5 data (1 min)
    0x016200, // Song 6 data
    0x018200, // Song 7 data
    0x01A200, // Song 8 data
    0x01C200, // Song 9 data
    0x01E200, // Song 10 data
    0x020200, // Song 11 data
    0x022200, // Song 12 data
    0x024200, // Song 13 data
    0x026200, // Song 14 data
    0x028200, // Song 15 data
    0x02A200, // Song 16 data
    0x02C200, // Song 17 data
    0x02E200, // Song 18 data
    0x030200, // Song 19 data
    0x032200, // Song 20 data
    0x034200, // Song 21 data
    0x036200, // Song 22 data
    0x038200, // Song 23 data
    0x03A200, // Song 24 data
    0x03C200, // Song 25 data
    0x03E200, // Song 26 data
    0x040200, // Song 27 data
    0x042200, // Song 28 data
    0x044200, // Song 29 data
    0x046200, // Song 30 data
    0x048200, // Song 31 data
    0x04A200, // Song 32 data
    0x04C200, // Song 33 data
    0x04E200, // Song 34 data
    0x050200, // Song 35 data
    0x052200, // Song 36 data
    0x054200, // Song 37 data
    0x056200, // Song 38 data
    0x058200, // Song 39 data
    0x05A200, // Song 40 data
    0x05C200, // Song 41 data
    0x05E200, // Song 42 data
    0x060200, // Song 43 data
    0x062200, // Song 44 data
    0x064200, // Song 45 data
    0x066200, // Song 46 data
    0x068200, // Song 47 data
    0x06A200, // Song 48 data
    0x06C200 // Song 49 data
};

#endif /* SONG_SONG_MAP_H_ */
