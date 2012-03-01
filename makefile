
# All Target
all: 
	gcc src/main.c src/tnyDB_mem.c src/tnyDB_list.c src/tnyDB_tword.c src/tnyDB_tmap.c src/tnyDB_tarray.c -o "tnyDB" -std=c99 -O3 -Wall
