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

#include "tnyDB_tarray.h"

tnyDB_tarray *tnyDB_tarray_create(int *data, int dataLength) {
	tnyDB_tarray * result = tnyDB_malloc_data(sizeof(tnyDB_tarray), "tnyDB_wtree_create.result");
	result->length = dataLength;
	result->data = tnyDB_calloc_data(sizeof(int), dataLength, "tnyDB_wtree_create.result->data");

	for (int i =0; i < dataLength; i++){
		result->data[i] = data[i];
	}

	return result;
}

TWORD * tnyDB_tarray_seek(tnyDB_tarray *tree, int value){
	TWORD *result = calloc(sizeof(TWORD), (tree->length/sizeof(TWORD))+1 );

	for (int i =0; i < tree->length; i++){
		if (tree->data[i] == value){
			tnyDB_tword_set(result, i);
		}
	}

	return result;
}

int tnyDB_tarray_access(tnyDB_tarray *tree, int rowIndex){
	return tree->data[rowIndex];
}
