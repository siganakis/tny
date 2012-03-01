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


#ifndef SPRDB_WTREE_H_
#define SPRDB_WTREE_H_

#include "tnyDB_list.h"
#include "tnyDB_tword.h"
#include "tnyDB_mem.h"

typedef struct TWORD* wtree_row;

typedef struct{
	int value_count;
	int depth;
	int row_length;

	tnyDB_list *keys;
	TWORD *data;
	tnyDB_list *ranks;

	int vector_length;

} tnyDB_wtree;



tnyDB_wtree *tnyDB_wtree_create(int *data, int dataLength);
TWORD * tnyDB_wtree_seek(tnyDB_wtree *tree, int value);
int tnyDB_wtree_access(tnyDB_wtree *wtree, int rowIndex);
tnyDB_list* tnyDB_wtree_scan(tnyDB_wtree *tree, TWORD *scan_list, int scan_list_length);

void tnyDB_wtree_free(tnyDB_wtree *wtree);



#endif /* SPRDB_TREE_H_ */
