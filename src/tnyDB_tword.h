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

#ifndef TNYDB_TWORD_H_
#define TNYDB_TWORD_H_

#include "tnyDB_mem.h"



typedef unsigned long long TWORD;


void tnyDB_tword_print(TWORD *r, int length);
int tnyDB_tword_bit_is_set(TWORD *r, int position) ;
void tnyDB_tword_set(TWORD *r, int position);
int tnyDB_tword_last_set_index(TWORD val) ;

TWORD *tnyDB_tword_copy_words(TWORD *old, int bit_length) ;

void tnyDB_tword_nand(TWORD *a, TWORD *b, int bit_length) ;
void tnyDB_tword_and(TWORD *a, TWORD *b, int bit_length);
void tnyDB_tword_and_not(TWORD *a, TWORD *b, int bit_length);
void tnyDB_tword_inverse(TWORD *a, int bit_length);
void tnyDB_tword_combine(TWORD *m, int mOffset, int mBitLen, TWORD *c, int cOffset, int cBitLen) ;
int tnyDB_tword_population(TWORD *mask, int start, int end);


void tnyDB_tword_refine(TWORD *m, int mBitLen, TWORD *c, int cOffset, int cBitLen, int isLeft);
void tnyDB_tword_print_offset(TWORD *r, int start, int length) ;


void tnyDB_tword_combine_test();

#endif /* TNYDB_TWORD_H_ */
