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

#ifndef TNYDB_LIST_H_
#define TNYDB_LIST_H_

#include "tnyDB_mem.h"

typedef struct {
	int length;
	int allocated_length;
	int *values;
} tnyDB_list;


// Appends a value to the end of the list
void tnyDB_list_push(tnyDB_list *list, int value);

// Inserts a value into the list at the specified index,
// moving items that occur after the index to their index+i
void tnyDB_list_insert(tnyDB_list *list, int index, int value);

// Sets the value at index specified to the value specified
void tnyDB_list_set(tnyDB_list *list, int index, int value);

// Gets the value at the specified index
int tnyDB_list_get(tnyDB_list *list, int index);

// Located the first occurence of the specified value
// by doing a binary search. If no item is found, the not
// value (~) of where it would be found is returned.
// This functions reauires that the list is already sorted
int tnyDB_list_find(tnyDB_list *list, int value);


// Creates a list and returns its reference
tnyDB_list *tnyDB_list_create();


// Creates a list that is pre-allocated to the size specified
tnyDB_list *tnyDB_list_create_allocated(int size);


// Creates a list of the disctinct values from *list, sorted ascending
tnyDB_list *tnyDB_list_sorted_distinct(int *list, int length);


// Frees memory used by this list
void tnyDB_list_free(tnyDB_list * toFree) ;


#endif /* TNYDB_LIST_H_ */
