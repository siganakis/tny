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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "tnyDB_list.h"
#include "tnyDB_wtree.h"

tnyDB_list * _get_keys(int *data, int dataLength) {
	tnyDB_list *keys = tnyDB_list_create();

	for (int i = 0; i < dataLength; i++) {
		int m = tnyDB_list_find(keys, data[i]);
		if (m < 0) {
			tnyDB_list_insert(keys, ~m, data[i]);
		}
	}
	return keys;
}

static inline int _log2(unsigned int v) {
	const unsigned int b[] = { 0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
	const unsigned int S[] = { 1, 2, 4, 8, 16 };
	int i;
	register unsigned int r = 0; // result of log2(v) will go here
	for (i = 4; i >= 0; i--) // unroll for speed...
			{
		if (v & b[i]) {
			v >>= S[i];
			r |= S[i];
		}
	}
	return r;
}

static inline int _row_word_length(tnyDB_wtree *tree) {
	return (tree->value_count / 64) + 1;
}


void _write_tree(tnyDB_wtree * tree, int *data, int dataLen, int rankIndex, int dataLeft, int keyLeft, int keyRight,
		int depth) {

	int keyMiddle = (keyLeft + keyRight) / 2;

	TWORD *row = tree->data + (depth * tree->row_length);
//	printf("Working at depth: %i (%i)\n", depth, depth * tree->row_length);

	int* leftValues = tnyDB_calloc_data(sizeof(int), dataLen, "_write_tree Left");
	int* rightValues = tnyDB_calloc_data(sizeof(int), dataLen, "_write_tree Right");
	int lCount = 0, rCount = 0;

	int i = 0;
	while (i < dataLen) {

		if (data[i] <= keyMiddle) {
			leftValues[lCount] = data[i];
			lCount++;
		} else {
			tnyDB_tword_set(row, dataLeft + i);
			rightValues[rCount] = data[i];
			rCount++;
		}
		i++;
	}

	tnyDB_list_set(tree->ranks, rankIndex - 1, lCount);

	// Go LEFT
	if (keyLeft < keyMiddle) {
		_write_tree(tree, leftValues, lCount, rankIndex * 2, dataLeft, keyLeft, keyMiddle, depth + 1);
	}

	tnyDB_free_data(leftValues, sizeof(int) * dataLen, "_write_tree Left");

	// Go RIGHT
	if (keyMiddle + 1 < keyRight) {
		_write_tree(tree, rightValues, rCount, (rankIndex * 2) + 1, dataLeft + lCount, keyMiddle + 1, keyRight,
				depth + 1);

	}
	tnyDB_free_data(rightValues, sizeof(int) * dataLen, "_write_tree Right");
}


// Given a bitmap vector, return the actual values at each set bits index
tnyDB_list* tnyDB_wtree_scan(tnyDB_wtree *tree, TWORD *scan_list, int scan_list_length) {
	tnyDB_list *list = tnyDB_list_create();

	// Now we are going to grab these ints in reverse order,
	// as its faster to read the TWORDs that way

	int words = (scan_list_length / 64) + 1;
//	int finalWordBitLength = vector->bit_length%64;
	int idx;
	for (int i = words - 1; i >= 0; i--) {

		TWORD v = scan_list[i];
		while (v > 0) {
			TWORD t = v;

			v &= v - 1;

			idx = i * 64 + tnyDB_tword_last_set_index(t - v) - 1;

			int val = tnyDB_wtree_access(tree, idx);
			tnyDB_list_push(list, val);
		}

	}

	return list;

}

TWORD * tnyDB_wtree_seek(tnyDB_wtree *tree, int value) {
	int rowBitLength = tree->value_count;
	int rowWordLength = (rowBitLength / 64) + 1;

	int l = 0;
	int r = tree->keys->length - 1;
	int m = (l + r) / 2;

	int rankIndex = 1;
	int vectorStart = 0, vectorLength = rowBitLength;
	TWORD *results = tnyDB_tword_copy_words(tree->data, rowBitLength);
	TWORD *row = tree->data;

//	printf("Seek: %i:\t", value);
	for (int i = 0; i < tree->depth; i++) {
		int zeroCount = tree->ranks->values[rankIndex - 1];

//		printf("\n----------------------\n(l:%i, m:%i, r:%i)\n:\t", l, m, r);
//		tnyDB_tword_print_offset(results, 0, rowBitLength);

		if (value > tree->keys->values[m]) {
			// BIGGER / RIGHT
			if (i == 0) {

			} else {
				tnyDB_tword_refine(results, rowBitLength, row, vectorStart, vectorLength, 0);
			}
//			printf("\n>R:\t");
//			tnyDB_tword_print_offset(row, vectorStart, vectorLength);
//			printf("\n");

			vectorStart += zeroCount;
			vectorLength = vectorLength - zeroCount;
			rankIndex = (rankIndex * 2) + 1;
			l = m + 1;

		} else {
			// SMALLER / LEFT
			if (i == 0) {
				tnyDB_tword_inverse(results, rowBitLength);
			} else {
				tnyDB_tword_refine(results, rowBitLength, row, vectorStart, vectorLength, 1);
			}
//			printf("\n> L:\t");
//			tnyDB_tword_print_offset(row, vectorStart, vectorLength);
//			printf("\n");

			vectorLength = zeroCount;
			rankIndex = (rankIndex * 2);
			r = m;
		}

		if (l == r) {
			break;

		}
		m = (l + r) / 2;
		// Next row please
		row += rowWordLength;
		zeroCount = tree->ranks->values[rankIndex - 1];

	}

//	printf("\t");
//	tnyDB_tword_print_offset(results, 0, rowBitLength);
//	printf("\n");
	return results;

}

// Finds the value at the specified index
int tnyDB_wtree_access(tnyDB_wtree *wtree, int rowIndex) {
//	printf("Access: %i ", rowIndex);
	int vectorStart = 0;

	int l = 0, r = wtree->keys->length - 1, m;

	int index = rowIndex;
	int rankIndex = 1;

	TWORD *ptr = wtree->data;

	for (int i = 0; i < wtree->depth; i++) {
		if (l == r) {

			break;
		}
		m = (l + r) / 2;

		// zeroCount tells us where the "fold" for the next row is (e.g. where 0's end and 1's begin)
		int zeroCount = wtree->ranks->values[rankIndex - 1];
		if (zeroCount < 0) {
			printf("Negative zeroCount... What the fuck?\n");
		}
		int pop = tnyDB_tword_population(ptr, vectorStart, vectorStart + index);

		if (!tnyDB_tword_bit_is_set(ptr, vectorStart + index)) {
//			printf("L");
			r = m;

			// Any "1" we see up to here can be removed from my index
			// as it will be placed after the "fold"
			index -= (pop);
			rankIndex = rankIndex * 2;

		} else {
//			printf("R");
			l = m + 1;

			// Move the start of the vector up to the
			vectorStart += zeroCount;
			// The index is after all the 0's (zeroCount) PLUS the number of 1's before my index
			index = pop - 1;

			rankIndex = (rankIndex * 2) + 1;
		}

		// Move the pointer to the next row down...
		ptr += wtree->row_length;
	}

	m = (l + r) / 2;
//	printf(" m=%i \n", m);
	return wtree->keys->values[m];

}

// Creates a new wtree from the integer array passed in
tnyDB_wtree *tnyDB_wtree_create(int *data, int dataLength) {

	tnyDB_wtree *tree = tnyDB_malloc_data(sizeof(tnyDB_wtree), "tnyDB_tree_create.wtree");

	tree->value_count = dataLength;
	tree->keys = _get_keys(data, dataLength);
	tree->depth = _log2(tree->keys->length) + 1;
	tree->row_length = _row_word_length(tree);

	// Build a new list "translated" which will contain all the
	// distinct values within the data array.  We then use the
	// bit vector stored in Data to handle references to this array
	int *translated = malloc(sizeof(int) * dataLength);
	for (int i = 0; i < dataLength; i++) {
		translated[i] = tnyDB_list_find(tree->keys, data[i]);
	}

	// Allocate enough words for each "row" times "depth"...
	tree->data = tnyDB_calloc_data(sizeof(TWORD), _row_word_length(tree) * tree->depth, "tnyDB_wtree_create.data");

	tree->ranks = tnyDB_list_create();
	_write_tree(tree, translated, dataLength, 1, 0, 0, tree->keys->length - 1, 0);

	free(translated);

//	for (int i = 0; i < tree->depth; i++) {
//		tnyDB_tword_print(tree->data + (i * tree->row_length), tree->value_count);
//		printf("\n");
//	}

	return tree;
}

