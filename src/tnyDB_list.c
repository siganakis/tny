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
#include <string.h>
#include "tnyDB_list.h"

int tnyDB_list_check_resize(tnyDB_list *list, int newLength) {

	if (newLength >= list->allocated_length) {
		int oldLength = list->allocated_length;
		int increaseBy = newLength * 0.2;
		if (increaseBy < 3) {
			increaseBy = 3;
		}
		int oldAllocated = list->allocated_length;
		list->allocated_length = newLength + increaseBy;

		if (oldLength != list->allocated_length) {
			// We need to create a new array I reckon...
			int * new = tnyDB_malloc_data(list->allocated_length * sizeof(int), "tnyDB_list_check_resize.new");
			for (int i = 0; i < oldAllocated; i++) {
				new[i] = list->values[i];
			}

			if (list->values != NULL) {
				tnyDB_free_data(list->values, oldAllocated * sizeof(int), "tnyDB_list_check_resize");
			}
			list->values = new;

//			printf("tnyDB_list resized to: %i\n", list->allocated_length);
		}
	}

	return 0;
}

void tnyDB_list_push(tnyDB_list *list, int value) {
	tnyDB_list_check_resize(list, list->length + 1);

	if (list->length >= list->allocated_length) {
		printf("Buffer overrun in tnyDB_list_push: list->length >= list->allocated_length (%i >= %i)\nExiting!\n",
				list->length, list->allocated_length);
		exit(-1);
	}

	list->values[list->length] = value;
	list->length++;
}

void tnyDB_list_insert(tnyDB_list *list, int index, int value) {

	int biggest = list->length + 1 > index + 1 ? list->length + 1 : index + 1;

	tnyDB_list_check_resize(list, biggest);

	if (index >= list->allocated_length) {
		printf("Buffer overrun in tnyDB_list_push: list->length >= list->allocated_length (%i >= %i)\nExiting!\n",
				list->length, list->allocated_length);
		exit(-1);
	}

	for (int i = list->length - 1; i >= index; i--) {
		list->values[i + 1] = list->values[i];
	}

	list->values[index] = value;

	list->length++;

}

void tnyDB_list_set(tnyDB_list *list, int index, int value) {

	int biggest = list->length + 1 > index + 1 ? list->length + 1 : index + 1;

	tnyDB_list_check_resize(list, biggest);

	if (index >= list->allocated_length) {
		printf("Buffer overrun in tnyDB_list_push: list->length >= list->allocated_length (%i >= %i)\nExiting!\n",
				list->length, list->allocated_length);
		exit(-1);
	}

	list->values[index] = value;

	if (index < list->length) {
		list->length++;
	} else {
		list->length = index + 1;
	}

}

int tnyDB_list_get(tnyDB_list *list, int index) {
	if (index > list->length) {
		fprintf(stderr, "ERROR: Couldn't realloc memory!\n");
		return (-1);
	}

	return list->values[index];
}

int tnyDB_list_binary_find(tnyDB_list *list, int seeking) {
	int l = 0;
	int r = list->length;
	int m = 0;

	if (r == 0) {
		return ~0;
	}

	while (seeking != list->values[m] && l <= r) {
		m = (l + r) / 2;

		if (m >= list->length)
			break;

		if (seeking < list->values[m])
			r = m - 1;
		if (seeking > list->values[m])
			l = m + 1;
	}

	if (l <= r && m < list->length) {
		//printf("tnyDB_list_binary_find: Found (Seeking: %i, l: %i, m: %i,r: %i, Length: %i)\n", seeking, l, m, r, list->length);
		return m;
	} else {
		//printf("tnyDB_list_binary_find: Not found (Seeking: %i, l: %i, m: %i,r: %i, Length: %i)\n", seeking, l, m, r, list->length);
		return ~l;
	}

}

int tnyDB_list_find(tnyDB_list *list, int seeking) {
	return tnyDB_list_binary_find(list, seeking);
}
tnyDB_list *tnyDB_list_create_allocated(int size) {

	tnyDB_list *list = tnyDB_malloc_data(sizeof(tnyDB_list), "tnyDB_list_create_allocated.list");
	list->values = tnyDB_malloc_data(sizeof(int), "tnyDB_list_create_allocated.list->values");
	list->allocated_length = size;
	list->length = 0;

	return list;
}
tnyDB_list *tnyDB_list_create() {
	tnyDB_list *list = tnyDB_malloc_data(sizeof(tnyDB_list), "tnyDB_list_create");
	list->allocated_length = 0;
	list->length = 0;
	list->values = NULL;
	return list;
}

/*
void tnyDB_quick_sort(int *arr, int elements) {

  #define  MAX_LEVELS  300

  int  piv, beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R, swap ;

  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L];
      while (L<R) {
        while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
        while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
      arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L;
      if (end[i]-beg[i]>end[i-1]-beg[i-1]) {
        swap=beg[i]; beg[i]=beg[i-1]; beg[i-1]=swap;
        swap=end[i]; end[i]=end[i-1]; end[i-1]=swap; 
    	}
	}
    else {
      i--; 
  }
  }
}
*/

#define MIN_MERGESORT_LIST_SIZE    32
void mergesort_array(int a[], int size, int temp[]) {
      int i1, i2, tempi;
      if (size < MIN_MERGESORT_LIST_SIZE) {
          /* Use insertion sort */
          int i;
          for (i=0; i < size; i++) {
             int j, v = a[i];
              for (j = i - 1; j >= 0; j--) {
                 if (a[j] <= v) break;
                  a[j + 1] = a[j];
              }
              a[j + 1] = v;
          }
          return;
      }
  
      mergesort_array(a, size/2, temp);
      mergesort_array(a + size/2, size - size/2, temp);
      i1 = 0;
      i2 = size/2;
      tempi = 0;
      while (i1 < size/2 && i2 < size) {
          if (a[i1] <= a[i2]) {
              temp[tempi] = a[i1];
              i1++;
         } else {
             temp[tempi] = a[i2];
              i2++;
          }
          tempi++;
      }
  
      while (i1 < size/2) {
          temp[tempi] = a[i1];
          i1++;
          tempi++;
      }
      while (i2 < size) {
          temp[tempi] = a[i2];
         i2++;
          tempi++;
      }
  
      memcpy(a, temp, size*sizeof(int));
  }
  
void tnyDB_swap(int *a, int *b)
{
  int t=*a; *a=*b; *b=t;
}
void tnyDB_quick_sort(int arr[], int beg, int end)
{
  if (end > beg + 1)
  {
    int piv = arr[beg], l = beg + 1, r = end;
    while (l < r)
    {
      if (arr[l] <= piv)
        l++;
      else
        tnyDB_swap(&arr[l], &arr[--r]);
    }
    tnyDB_swap(&arr[--l], &arr[beg]);
    tnyDB_quick_sort(arr, beg, l);
    tnyDB_quick_sort(arr, r, end);
  }
}


tnyDB_list *tnyDB_list_sorted_distinct(int *list, int length){
	int *sorted = tnyDB_malloc_data(sizeof(int) * length, "tnyDB_list_sorted_distinct.sorted");
	int *tmpSorted = tnyDB_malloc_data(sizeof(int) * length, "tnyDB_list_sorted_distinct.tmpSorted");
	memcpy(sorted, list, length* sizeof(int));


	// Sort it
	// tnyDB_quick_sort(sorted, 0, length);
	mergesort_array(sorted, length, tmpSorted);
	tnyDB_free_data(tmpSorted, sizeof(int) * length, "tnyDB_list_sorted_distinct.tmpSorted");

	// Extract only the unique values 
	int ci=1, last=sorted[0];


	for (int i =1; i < length; i++){
		if (sorted[i] != last){
			last = sorted[i];
			sorted[ci] = sorted[i];
			ci++;
		}
	}

	// Resize the array
	sorted = tnyDB_realloc_data(sorted, sizeof(int) * (ci), sizeof(int) * length, "tnyDB_list_sorted_distinct.shrink");


	tnyDB_list *result = tnyDB_malloc_data(sizeof(tnyDB_list), "tnyDB_list_sorted_distinct.list");
	result->values = sorted;
	result->allocated_length = ci+1;
	result->length = ci;

	return result;
}

void tnyDB_list_free(tnyDB_list * toFree) {
	tnyDB_free_data(toFree->values, toFree->allocated_length * sizeof(int), "tnyDB_list_free (values)");
	tnyDB_free_data(toFree, sizeof(tnyDB_list), "tnyDB_list_free");
}

