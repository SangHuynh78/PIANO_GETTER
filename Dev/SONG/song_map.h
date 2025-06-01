/*
 * song_map.h
 *
 *  Created on: Jun 1, 2025
 *      Author: HTSANG
 *
 * Header file defining start addresses for song names and sample data for 50 songs.
 * All songs (2 minutes): 512 bytes name + 12,000 bytes data, aligned to 0x5000 (20,480 bytes).
 * Sample data offset: 0x1000 (4,096 bytes) from name start.
 * First 2 sectors (0x000000-0x001FFF, 8,192 bytes) reserved for song_index.
 * Total storage: 1,032,192 bytes (2 sectors + 50 songs).
 */

#ifndef SONG_SONG_MAP_H_
#define SONG_SONG_MAP_H_

#define SONG_COUNT          50
#define SONG_NAME_SIZE      256    		// Bytes for song name
#define SONG_DATA_SIZE      12000  		// Bytes for sample data (2-minute songs)
#define SONG_BLOCK          20480  		// Aligned block for songs (0x5000)
#define SONG_DATA_OFFSET    0x1000 		// Offset for data from name start (4,096 bytes)
#define INDEX_STORAGE_START 0x000000 	// Start address for song_index
#define INDEX_STORAGE_SIZE  8192   		// Size of reserved area for song_index (2 sectors, 0x2000)

#include "stdint.h"

// Array of start addresses for song names
static const uint32_t SONG_NAME_START[SONG_COUNT] = {
    0x002000, // Song 0 name
    0x007000, // Song 1 name
    0x00C000, // Song 2 name
    0x011000, // Song 3 name
    0x016000, // Song 4 name
    0x01B000, // Song 5 name
    0x020000, // Song 6 name
    0x025000, // Song 7 name
    0x02A000, // Song 8 name
    0x02F000, // Song 9 name
    0x034000, // Song 10 name
    0x039000, // Song 11 name
    0x03E000, // Song 12 name
    0x043000, // Song 13 name
    0x048000, // Song 14 name
    0x04D000, // Song 15 name
    0x052000, // Song 16 name
    0x057000, // Song 17 name
    0x05C000, // Song 18 name
    0x061000, // Song 19 name
    0x066000, // Song 20 name
    0x06B000, // Song 21 name
    0x070000, // Song 22 name
    0x075000, // Song 23 name
    0x07A000, // Song 24 name
    0x07F000, // Song 25 name
    0x084000, // Song 26 name
    0x089000, // Song 27 name
    0x08E000, // Song 28 name
    0x093000, // Song 29 name
    0x098000, // Song 30 name
    0x09D000, // Song 31 name
    0x0A2000, // Song 32 name
    0x0A7000, // Song 33 name
    0x0AC000, // Song 34 name
    0x0B1000, // Song 35 name
    0x0B6000, // Song 36 name
    0x0BB000, // Song 37 name
    0x0C0000, // Song 38 name
    0x0C5000, // Song 39 name
    0x0CA000, // Song 40 name
    0x0CF000, // Song 41 name
    0x0D4000, // Song 42 name
    0x0D9000, // Song 43 name
    0x0DE000, // Song 44 name
    0x0E3000, // Song 45 name
    0x0E8000, // Song 46 name
    0x0ED000, // Song 47 name
    0x0F2000, // Song 48 name
    0x0F7000  // Song 49 name
};

// Array of start addresses for song sample data
static const uint32_t SONG_DATA_START[SONG_COUNT] = {
    0x003000, // Song 0 data
    0x008000, // Song 1 data
    0x00D000, // Song 2 data
    0x012000, // Song 3 data
    0x017000, // Song 4 data
    0x01C000, // Song 5 data
    0x021000, // Song 6 data
    0x026000, // Song 7 data
    0x02B000, // Song 8 data
    0x030000, // Song 9 data
    0x035000, // Song 10 data
    0x03A000, // Song 11 data
    0x03F000, // Song 12 data
    0x044000, // Song 13 data
    0x049000, // Song 14 data
    0x04E000, // Song 15 data
    0x053000, // Song 16 data
    0x058000, // Song 17 data
    0x05D000, // Song 18 data
    0x062000, // Song 19 data
    0x067000, // Song 20 data
    0x06C000, // Song 21 data
    0x071000, // Song 22 data
    0x076000, // Song 23 data
    0x07B000, // Song 24 data
    0x080000, // Song 25 data
    0x085000, // Song 26 data
    0x08A000, // Song 27 data
    0x08F000, // Song 28 data
    0x094000, // Song 29 data
    0x099000, // Song 30 data
    0x09E000, // Song 31 data
    0x0A3000, // Song 32 data
    0x0A8000, // Song 33 data
    0x0AD000, // Song 34 data
    0x0B2000, // Song 35 data
    0x0B7000, // Song 36 data
    0x0BC000, // Song 37 data
    0x0C1000, // Song 38 data
    0x0C6000, // Song 39 data
    0x0CB000, // Song 40 data
    0x0D0000, // Song 41 data
    0x0D5000, // Song 42 data
    0x0DA000, // Song 43 data
    0x0DF000, // Song 44 data
    0x0E4000, // Song 45 data
    0x0E9000, // Song 46 data
    0x0EE000, // Song 47 data
    0x0F3000, // Song 48 data
    0x0F8000  // Song 49 data
};

#endif /* SONG_SONG_MAP_H_ */
