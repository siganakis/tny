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

#include "tnyDB_tmap.h"
#include "tnyDB_wtree.h"
#include "tnyDB_tarray.h"
#include "tnyDB_tword.h"

void tnyDB_tmap_access_test(int * values, int length) {
	tnyDB_mem_init();

	double elapsed; // in milliseconds
	clock_t start, end;
	start = clock();

	printf("Building TMAP... ");
	tnyDB_tmap *tree = tnyDB_tmap_create(values, length);

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Done (took %fms)\n", elapsed);

	tnyDB_mem_statistics memStats = tnyDB_mem_get_statistics();

//	printf("\n====== TMAP Mem Stats ======\n");
	printf("Bytes current: %i\n", memStats.bytes_current);
//	printf("Bytes total: %i\n", memStats.bytes_total);
//	printf("Allocations: %i\n", memStats.allocations);
//	printf("Frees: %i\n", memStats.frees);
//	printf("====== TMAP Mem Stats ======\n\n");

	printf("Testing TMAP access performance...");
	start = clock();

	for (int i = 0; i < length; i++) { //
		if (values[i] != tnyDB_tmap_access(tree, i)) {
			end = clock();
			elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
			fprintf(stderr, "\nmissmatch at: %i, found %i, expected %i (%f)\n", i, tnyDB_tmap_access(tree, i),
					values[i], elapsed);

			exit(-1);
		}
	}

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("TMAP Passed test in %fms\n\n", elapsed);
}


void tnyDB_wtree_access_test(int * values, int length) {
	tnyDB_mem_init();

	double elapsed; // in milliseconds
	clock_t start, end;
	start = clock();

	printf("Building WTREE... ");
	tnyDB_wtree *tree = tnyDB_wtree_create(values, length);

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Done (took %fms)\n", elapsed);

	tnyDB_mem_statistics memStats = tnyDB_mem_get_statistics();

//	printf("\n====== WTREE Mem Stats ======\n");
	printf("Bytes current: %i\n", memStats.bytes_current);
//	printf("Bytes total: %i\n", memStats.bytes_total);
//	printf("Allocations: %i\n", memStats.allocations);
//	printf("Frees: %i\n", memStats.frees);
//	printf("====== WTREE Mem Stats ======\n\n");

	printf("Testing wTree access performance...");
	start = clock();

	for (int i = 0; i < length; i++) { //
		if (values[i] != tnyDB_wtree_access(tree, i)) {
			end = clock();
			elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
			fprintf(stderr, "\nmissmatch at: %i, found %i, expected %i (%f)\n", i, tnyDB_wtree_access(tree, i),
					values[i], elapsed);

			exit(-1);
		}
	}

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("wtree Passed test in %fms\n\n", elapsed);
}

void tnyDB_tarray_access_test(int * values, int length) {
	tnyDB_mem_init();

	double elapsed; // in milliseconds
	clock_t start, end;
	start = clock();

	printf("Building ARRAY... ");
	tnyDB_tarray *tree = tnyDB_tarray_create(values, length);

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Done (took %fms)\n", elapsed);

	tnyDB_mem_statistics memStats = tnyDB_mem_get_statistics();

//	printf("\n====== array Mem Stats ======\n");
	printf("Bytes current: %i\n", memStats.bytes_current);
//	printf("Bytes total: %i\n", memStats.bytes_total);
//	printf("Allocations: %i\n", memStats.allocations);
//	printf("Frees: %i\n", memStats.frees);
//	printf("====== array Mem Stats ======\n\n");

	printf("Testing ARRAY access performance...");
	start = clock();

	for (int i = 0; i < length; i++) { //
		if (values[i] != tnyDB_tarray_access(tree, i)) {
			end = clock();
			elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
			fprintf(stderr, "\nmissmatch at: %i, found %i, expected %i (%f)\n", i, tnyDB_tarray_access(tree, i),
					values[i], elapsed);

			exit(-1);
		}
	}

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("ARRAY Passed test in %fms\n\n", elapsed);
}

int tnyDB_access_test(int keys, int length) {
	srand(time(NULL));

	int *values = malloc(length * sizeof(int));

	printf("Generating data...");
	for (int i = 0; i < length; i++) {
		values[i] = rand() % keys;
	}
	printf("Done\nh");

	tnyDB_tarray_access_test(values, length);
	tnyDB_tmap_access_test(values, length);
	tnyDB_wtree_access_test(values, length);

	free(values);

	return 1;
}

void tnyDB_tarray_seek_test(int * values, int length, int seeks) {
	tnyDB_mem_init();

	double elapsed; // in milliseconds
	clock_t start, end;
	start = clock();

	printf("Building ARRAY... ");
	tnyDB_tarray *tree = tnyDB_tarray_create(values, length);

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Done (took %fms)\n", elapsed);

	tnyDB_mem_statistics memStats = tnyDB_mem_get_statistics();

	// printf("\n====== ARRAY Mem Stats ======\n");
	printf("Bytes current: %i\n", memStats.bytes_current);
	// printf("Bytes total: %i\n", memStats.bytes_total);
	// printf("Allocations: %i\n", memStats.allocations);
	// printf("Frees: %i\n", memStats.frees);

	printf("Testing ARRAY SEEK performance... ");
	start = clock();

	for (int i = 0; i < seeks; i++) { //
		int seeking = values[i];
		TWORD * results = tnyDB_tarray_seek(tree, seeking);

		// Now in the results, a bit at "i" should be set
		if (tnyDB_tword_bit_is_set(results, i) == 0) {
			end = clock();
			elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
			fprintf(stderr, "\n ARRAY SEEK: missmatch at: %i, BIT NOT SET\n", i);
			exit(-1);
		}

	}

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Passed test in %fms\n\n", elapsed);
}

void tnyDB_tmap_seek_test(int * values, int length, int seeks) {
	tnyDB_mem_init();

	double elapsed; // in milliseconds
	clock_t start, end;
	start = clock();

	printf("Building TMAP... ");
	tnyDB_tmap *tree = tnyDB_tmap_create(values, length);

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Done (took %fms)\n", elapsed);

	tnyDB_mem_statistics memStats = tnyDB_mem_get_statistics();

	// printf("\n====== TMAP Mem Stats ======\n");
	printf("Bytes current: %i\n", memStats.bytes_current);
	// printf("Bytes total: %i\n", memStats.bytes_total);
	// printf("Allocations: %i\n", memStats.allocations);
	// printf("Frees: %i\n", memStats.frees);
	// printf("====== TMAP Mem Stats ======\n\n");

	printf("Testing TMAP SEEK performance... ");
	start = clock();

	for (int i = 0; i < seeks; i++) { //
		int seeking = values[i];
		TWORD * results = tnyDB_tmap_seek(tree, seeking);

		// Now in the results, a bit at "i" should be set
		if (tnyDB_tword_bit_is_set(results, i) == 0) {
			end = clock();
			elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
			fprintf(stderr, "\n TMAP Seek: missmatch at: %i, BIT NOT SET\n", i);
			exit(-1);
		}

	}

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf(" Passed test in %fms\n\n", elapsed);
}


void tnyDB_wtree_seek_test(int * values, int length, int seeks) {
	tnyDB_mem_init();

	double elapsed; // in milliseconds
	clock_t start, end;
	start = clock();

	printf("Building WTREE... ");
	tnyDB_wtree *tree = tnyDB_wtree_create(values, length);

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf("Done (took %fms)\n", elapsed);

	tnyDB_mem_statistics memStats = tnyDB_mem_get_statistics();

	// printf("\n====== TMAP Mem Stats ======\n");
	printf("Bytes current: %i\n", memStats.bytes_current);
	// printf("Bytes total: %i\n", memStats.bytes_total);
	// printf("Allocations: %i\n", memStats.allocations);
	// printf("Frees: %i\n", memStats.frees);
	// printf("====== TMAP Mem Stats ======\n\n");

	printf("Testing WTREE SEEK performance... ");
	start = clock();

	for (int i = 0; i < seeks; i++) { //
		int seeking = values[i];
		TWORD * results = tnyDB_wtree_seek(tree, seeking);

		// Now in the results, a bit at "i" should be set
		if (tnyDB_tword_bit_is_set(results, i) == 0) {
			end = clock();
			elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
			fprintf(stderr, "\n WTREE Seek: missmatch at: %i, BIT NOT SET\n", i);
			exit(-1);
		}

	}

	end = clock();
	elapsed = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
	printf(" Passed test in %fms\n\n", elapsed);
}


int tnyDB_seek_test(int keys, int length, int seeks) {
	srand(time(NULL));

	int *values = malloc(length * sizeof(int));

	printf("Generating data...");
	for (int i = 0; i < length; i++) {
		values[i] = rand() % keys;
	}
	printf("Done\n\n");

	tnyDB_tarray_seek_test(values, length, seeks);
	tnyDB_tmap_seek_test(values, length, seeks);
	tnyDB_wtree_seek_test(values, length, seeks);

	free(values);

	return 1;
}

int main(int argc, char *argv[]) {

	if (argc < 4 || argc > 4){
		printf("Please supply 3 arguments:\n");
		printf("\t1.\t The number of values to generate\n");
		printf("\t2.\t The number of distinct values\n");
		printf("\t3.\t The number of times the structures should be seeked\n");
		printf("You supplied %i arguments\n", argc);
		return EXIT_SUCCESS;
	}
	int values= atoi(argv[1]);
	int keys  = atoi(argv[2]);
	int seeks = atoi(argv[3]);

	tnyDB_access_test(keys, values);
	tnyDB_seek_test(keys, values, seeks);

//	int values[10] = {1, 4, 7, 4, 5, 6, 7, 9, 1, 2};
//	tnyDB_tmap_seek_test(values, 10);

	return EXIT_SUCCESS;
}
