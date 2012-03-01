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

#ifndef TNYDB_TARRAY_H_
#define TNYDB_TARRAY_H_

#include "tnyDB_tword.h"

typedef struct {
	int* data;
	int length;
} tnyDB_tarray;

TWORD * tnyDB_tarray_seek(tnyDB_tarray *tree, int value);
int tnyDB_tarray_access(tnyDB_tarray *wtree, int rowIndex);
tnyDB_tarray *tnyDB_tarray_create(int *data, int dataLength);

#endif /* TNYDB_TARRAY_H_ */
