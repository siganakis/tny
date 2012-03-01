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


#include <stdio.h>
#include <stdlib.h>
#include "tnyDB_tword.h"
#include "tnyDB_mem.h"

static const TWORD k1 = 0x5555555555555555; /*  -1/3   */
static const TWORD k2 = 0x3333333333333333; /*  -1/5   */
static const TWORD k4 = 0x0f0f0f0f0f0f0f0f; /*  -1/17  */
static const TWORD kf = 0x0101010101010101; /*  -1/255 */
static const int TWORD_SIZE_BITS = 64;
static const TWORD ONE_64BIT = 1;

void tnyDB_tword_print(TWORD *r, int length) {
	printf(" (%i) ", length);

	if (r == NULL) {
		printf("NULL");
	} else {
		for (int i = 0; i < length; i++) {
			if (i % 8 == 0 && i != 0) {
				printf(" ");

			}
			if (tnyDB_tword_bit_is_set(r, i)) {
				printf("1");
			} else {
				printf("0");
			}
		}

	}
}

void tnyDB_tword_print_offset(TWORD *r, int start, int length) {


	if (r == NULL) {
		printf("NULL");
	} else {
		for (int i = start; i < start+length; i++) {
			if (i % 8 == 0 && i != 0) {
				printf(" ");

			}
			if (tnyDB_tword_bit_is_set(r, i)) {
				printf("1");
			} else {
				printf("0");
			}
		}


	}

	printf(" (s:%i, l:%i)", start, length);
	//printf("\n");

}

int tnyDB_tword_bit_is_set(TWORD *r, int position) {

	int wordNumber = position / TWORD_SIZE_BITS;
	int bitPosition = position % TWORD_SIZE_BITS;

	if ((r[wordNumber] & (ONE_64BIT << bitPosition)) != 0) {
		return 1;
	} else {
		return 0;
	}

}

void tnyDB_tword_set(TWORD *r, int position) {
	int wordNumber = position / TWORD_SIZE_BITS;
	int bitPosition = position % TWORD_SIZE_BITS;

	r[wordNumber] = r[wordNumber] | (ONE_64BIT << bitPosition);
}

int tnyDB_tword_last_set_index(TWORD val) {
	int result = 0;
	if (val >= 0x100000000) {
		result += 32;
		val >>= 32;
	}
	if (val >= 0x10000) {
		result += 16;
		val >>= 16;
	}
	if (val >= 0x100) {
		result += 8;
		val >>= 8;
	}
	if (val >= 0x10) {
		result += 4;
		val >>= 4;
	}
	if (val >= 0x4) {
		result += 2;
		val >>= 2;
	}
	if (val >= 0x2) {
		result += 1;
		val >>= 1;
	}
	return result + (int) val;

}

TWORD *tnyDB_tword_copy_words(TWORD *old, int bit_length) {

	int wordLength = (bit_length / TWORD_SIZE_BITS) + 1;
	TWORD *new = tnyDB_malloc_data(sizeof(TWORD) * wordLength, "tnyDB_vector_copy->data");

	for (int i = 0; i < wordLength; i++) {
		new[i] = old[i];
	}

	return new;
}

void tnyDB_tword_and(TWORD *a, TWORD *b, int bit_length) {
	int wordLength = (bit_length / TWORD_SIZE_BITS) + 1;
	for (int i = 0; i < wordLength; i++) {
		a[i] &= b[i];
	}
}

void tnyDB_tword_and_not(TWORD *a, TWORD *b, int bit_length) {

	int wordLength = (bit_length / TWORD_SIZE_BITS) + 1;
	for (int i = 0; i < wordLength; i++) {
		a[i] &= ~(b[i]);
	}

}

void tnyDB_tword_nand(TWORD *a, TWORD *b, int bit_length) {

	int wordLength = (bit_length / TWORD_SIZE_BITS) + 1;
	for (int i = 0; i < wordLength; i++) {
		a[i] = (~(a[i])) & b[i];
	}

}

void tnyDB_tword_inverse(TWORD *a, int bit_length) {
	int wordLength = (bit_length / TWORD_SIZE_BITS) + 1;
	for (int i = 0; i < wordLength; i++) {
		a[i] = ~(a[i]);
	}

}


void tnyDB_tword_refine(TWORD *m, int mBitLen, TWORD *c, int cOffset, int cBitLen, int isLeft) {




	// Move through the compressed word to the correct word (word position)
	c += cOffset/64;

	// The bit position in the current Compressed WORD
	int cPosInWord = cOffset % 64;

	// The bit position in the current MASK word
	int mPosInWord = 0;

	// How many bits in M we have visited
	int mCount = 0;

	for (int ci = 0; ci < cBitLen; ci++) {

		TWORD isSet = ( (*c) & (1ul << cPosInWord));
		if (isLeft == 1){
			isSet = isSet == 0 ? 1 : 0;
		}

		// Go through the MASK until we find the next 1 bit
		while ((*m & (1ul << mPosInWord)) == 0) {
			mPosInWord++;
			mCount++;
			if (mPosInWord == 64) {
				mPosInWord = 0;
				m++;
			}

			if (mCount > mBitLen) {
				printf("Error in tnyDB_tword_refine: mCount > mBitLen (%i > %i)\n", mCount, mBitLen);

				printf("Mask:\t");
				tnyDB_tword_print_offset(m, 0, mBitLen);
				printf("\nModifier:\t");
				tnyDB_tword_print_offset(c, cOffset, cBitLen);
				printf("\n");
				exit(-1);
			}
		}
		if (isSet == 0) {
			// Reset the bit in M corresponding to mPosInWord...
			*m &= ~(1ul << mPosInWord);
		}

		mPosInWord++;
		cPosInWord++;

		mCount++;

		if (cPosInWord == 64) {
			cPosInWord = 0;
			c++;
		}
		if (mPosInWord == 64) {
			mPosInWord = 0;
			m++;
		}
	}
}

// Expand
// c = "0101" using
// m = "0100110100", to come up with
// r = "0*001*0100" (where a * is a 1 reset to a 0)
void tnyDB_tword_combine(TWORD *m, int mOffset, int mBitLen, TWORD *c, int cOffset, int cBitLen) {

	int mCount = 0;
	int cPosInWord = cOffset; // The position in the current Compressed WORD
	int mPosInWord = mOffset; // The position in the current MASK word
	TWORD mStart = *m;
	TWORD * cStart = c;

	printf("tnyDB_tword_combine\n---------------------\nM: ");
	tnyDB_tword_print_offset(m, mOffset, mBitLen);

	printf("\nC: ");
	tnyDB_tword_print_offset(c, cOffset, cBitLen);
	printf("\n");

	for (int ci = 0; ci < cBitLen; ci++) {

		TWORD isSet = (*c & (1ul << cPosInWord));

		// Go through the MASK until we find the next 1 bit
		while ((*m & (1ul << mPosInWord)) == 0) {
			mPosInWord++;
			mCount++;
			if (mPosInWord == 64) {
				mPosInWord = 0;
				m++;
			}
			if (mCount > mBitLen) {
				printf("Error in tnyDB_tword_combine: mCount > mBitLen (%i > %i)\n M:", mCount, mBitLen);

				tnyDB_tword_print(&mStart, mBitLen + mOffset);
				printf("\nC:");
				tnyDB_tword_print(cStart, cBitLen);
				printf("\n");
				exit(-1);
			}
		}

		if (isSet == 0) {
			// Reset it...
			*m &= ~(1ul << mPosInWord);
		}

		mPosInWord++;
		cPosInWord++;

		mCount++;




		if (ci == 64) {
			cPosInWord = 0;
			c++;
		}
	}

}

void tnyDB_tword_combine_test() {

	TWORD mask = 0;

	mask |= 1ul << 0;
	mask |= 1ul << 2;
	mask |= 1ul << 4;
	mask |= 1ul << 6;
	mask |= 1ul << 8;
	mask |= 1ul << 10;

	TWORD compressed = 0;
	compressed |= 1ul << 0;
	compressed |= 1ul << 1;
	compressed |= 1ul << 2;

	printf("M: ");
	tnyDB_tword_print(&mask, 10);
	printf("\nC: ");
	tnyDB_tword_print(&compressed, 10);
	printf("\nR: ");
	tnyDB_tword_combine(&mask, 0, 10, &compressed, 0, 7);
	tnyDB_tword_print(&mask, 10);

	printf("\n");

}

int tnyDB_tword_population(TWORD *mask, int start, int end) {
	int startWord = start / TWORD_SIZE_BITS;
	int startIndex = start % TWORD_SIZE_BITS;

	int endWord = end / TWORD_SIZE_BITS;
	int endIndex = end % TWORD_SIZE_BITS;

	int startEndIndex = startWord == endWord ? endIndex : TWORD_SIZE_BITS - 1;

	int pop = 0;

	TWORD first = *(mask + startWord);
	// Lets do this the slow and steady way....
	for (int i = startIndex; i <= startEndIndex; i++) {
		if ((first & (ONE_64BIT << i)) != 0) {
			pop++;
		}
	}

	// Now we can do it Word at a time...
	for (int j = startWord + 1; j < endWord; j++) {
		TWORD x = *(mask + j);
		x = x - ((x >> 1) & k1); /* put count of each 2 bits into those 2 bits */
		x = (x & k2) + ((x >> 2) & k2); /* put count of each 4 bits into those 4 bits */
		x = (x + (x >> 4)) & k4; /* put count of each 8 bits into those 8 bits */
		x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */

		pop += (int) x;
	}

	if (startWord != endWord) {
		TWORD last = *(mask + endWord);
		// Lets do this the slow and steady way....
		for (int k = 0; k <= endIndex; k++) {
			if ((last & (ONE_64BIT << k)) != 0) {
				pop++;
			}
		}
	}
	return pop;

}
