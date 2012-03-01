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

#ifndef TNYDB_MEM_H_
#define TNYDB_MEM_H_


#include <stdlib.h>
#include <stdio.h>


typedef struct  {
	int allocations;
	int frees;

	int bytes_total;
	int bytes_current;

} tnyDB_mem_statistics;




void *tnyDB_malloc(int amount, char *tag);
void tnyDB_free(void *ptr, char *tag);


void tnyDB_free_data(void *ptr, int bytes, char* tag);
void *tnyDB_malloc_data(int bytes, char* tag);
void *tnyDB_realloc_data(void *ptr, int new_bytes, int old_bytes, char* tag);
void *tnyDB_calloc_data(int item_size, int item_count, char* tag);

void tnyDB_mem_init();
tnyDB_mem_statistics tnyDB_mem_get_statistics();

#endif /* SPRDB_MEM_H_ */
