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

#include "tnyDB_mem.h"

tnyDB_mem_statistics tnyDB_mem_stats;

void tnyDB_mem_init() {

	tnyDB_mem_stats.allocations = 0;
	tnyDB_mem_stats.frees = 0;
	tnyDB_mem_stats.bytes_total = 0;
	tnyDB_mem_stats.bytes_current = 0;
}

void *tnyDB_malloc(int amount, char* tag) {
	return malloc(amount);
}

void tnyDB_free(void *ptr, char* tag) {
	free(ptr);

}

tnyDB_mem_statistics tnyDB_mem_get_statistics() {
	return tnyDB_mem_stats;
}

void tnyDB_free_data(void *ptr, int bytes, char* tag) {

	tnyDB_mem_stats.frees++;
	tnyDB_mem_stats.bytes_current -= bytes;

	free(ptr);
}

void *tnyDB_realloc_data(void *ptr, int new_bytes, int old_bytes, char* tag) {
	int difference = new_bytes - old_bytes;
	tnyDB_mem_stats.frees++;
	tnyDB_mem_stats.allocations++;
	tnyDB_mem_stats.bytes_current += difference;

	void* newptr = realloc(ptr, new_bytes);
	if (newptr != NULL) {
		return newptr;
	} else {
		fprintf(stderr, "REALLOC Failed! {new_bytes: %i, old_bytes: %i}", new_bytes, old_bytes);
		exit(-1);
	}
}
void *tnyDB_calloc_data(int item_size, int item_count, char* tag) {
	tnyDB_mem_stats.allocations++;
	tnyDB_mem_stats.bytes_current += (item_size * item_count);
	tnyDB_mem_stats.bytes_total += (item_size * item_count);



	void *ptr = calloc(item_count, item_size);

	return ptr;
}

void *tnyDB_malloc_data(int bytes, char* tag) {
	tnyDB_mem_stats.allocations++;
	tnyDB_mem_stats.bytes_current += bytes;
	tnyDB_mem_stats.bytes_total += bytes;

	void * ptr = malloc(bytes);

	return ptr;
}
