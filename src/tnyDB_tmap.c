/* 	Copyright (c) 2011 Terence Siganakis.

 This file is part of TnyDB.

 TnyDB is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TnyDB is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TnyDB.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Contact: Terence Siganakis <terence@siganakis.com> */

#include "tnyDB_tmap.h"

tnyDB_list * _tmap_get_keys(int *data, int dataLength) {
	tnyDB_list *keys = tnyDB_list_create();
	for (int i = 0; i < dataLength; i++) {
		int m = tnyDB_list_find(keys, data[i]);
		if (m < 0) {
			tnyDB_list_insert(keys, ~m, data[i]);
		}
	}
	return keys;
}

static inline int _tmap_log2(unsigned int v) {
	const unsigned int b[] = { 0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
	const unsigned int S[] = { 1, 2, 4, 8, 16 };
	int i;
	unsigned int r = 0; // result of log2(v) will go here
	for (i = 4; i >= 0; i--) // unroll for speed...
			{
		if (v & b[i]) {
			v >>= S[i];
			r |= S[i];
		}
	}
	return r;
}

static inline int _tmap_row_word_length(tnyDB_tmap *map) {
	return (map->length / 64) + 1;

}


tnyDB_tmap *tnyDB_tmap_create(int *data, int dataLength) {
	tnyDB_tmap *map = tnyDB_malloc_data(sizeof(tnyDB_tmap), "tnyDB_tmap_create.tmap");

	map->length = dataLength;
	// map->keys = _tmap_get_keys(data, dataLength);
	map->keys = tnyDB_list_sorted_distinct(data, dataLength);


	// tnyDB_list * list1 = _tmap_get_keys(data, dataLength);
	// tnyDB_list * list2 = tnyDB_list_sorted_distinct(data, dataLength);

	// for (int i =0; i < list1->length || i < list2->length; i++){
	// 	printf("%i %i\n", list1->values[i], list2->values[i]);
	// }
	// exit(0);


	map->depth = _tmap_log2(map->keys->length) + 1;
	map->data = tnyDB_calloc_data(sizeof(TWORD*), map->depth, "tnyDB_tmap_create.tmap->data[]");

	int wordLength =  _tmap_row_word_length(map);
	for (int d =0; d < map->depth; d++){
		map->data[d] = tnyDB_calloc_data(sizeof(TWORD), wordLength, "tnyDB_tmap_create.tmap->data[i]");
	}

	// Build a new list "translated" which will contain all the
	// distinct values within the data array.  We then use the
	// bit vector stored in Data to handle references to this array

	int *translated = malloc(sizeof(int) * dataLength);
	for (int i = 0; i < dataLength; i++) {
		translated[i] = tnyDB_list_find(map->keys, data[i]);
	}


	for (int d = 0; d < map->depth; d++) {
		for (int w = 0; w < wordLength; w++){
			TWORD tmp=0;
			for (int i = 0; i < 64; i++){
				if ((translated[(w*64) + i] & (1 << d)) != 0) {
					tmp |= 1ull << i;
				}
			}
			map->data[d][w] = tmp;

		}

	}
	free(translated);

	return map;
}

int tnyDB_tmap_access(tnyDB_tmap *map, int rowIndex) {
	int keyIndex = 0;
	for (int d = 0; d < map->depth; d++) {
		if (tnyDB_tword_bit_is_set(map->data[d], rowIndex)) {
			keyIndex |= 1 << d;
		}
	}
	return map->keys->values[keyIndex];

}

TWORD * tnyDB_tmap_seek(tnyDB_tmap *map, int value) {
	TWORD * result = tnyDB_calloc_data(sizeof(TWORD), _tmap_row_word_length(map), "tnyDB_tmap_seek.result");
	int keyIndex = tnyDB_list_find(map->keys, value);

	// Make a little cache...
	int keyBitMask[map->depth];
	for (int j = 0; j < map->depth; j++) {
		keyBitMask[j] = (keyIndex & (1 << j));
	}

	int rowWordLength = _tmap_row_word_length(map);

	// Do we have it in our key index?
	if (keyIndex >= 0) {
		// Go through the data one word at a time...
		for (int w = 0; w < rowWordLength; w++) {

			TWORD wordMatches = keyBitMask[0] == 0 ? ~map->data[0][w] : map->data[0][w];
			if (wordMatches != 0) {
				for (int d = 1; d < map->depth; d++) {
					if (keyBitMask[d] == 0) {
						wordMatches &= ~(map->data[d][w]);
					} else {
						wordMatches &= (map->data[d][w]);
					}
					if (wordMatches == 0)
						break;
				}
				result[w] = wordMatches;
			}
		}

	}

	return result;
}

