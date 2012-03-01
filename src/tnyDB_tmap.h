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

#ifndef TNYDB_TMAP_H_
#define TNYDB_TMAP_H_

#include "tnyDB_tword.h"
#include "tnyDB_list.h"

typedef struct {
	TWORD** data;
	int length;
	int depth;
	tnyDB_list *keys;
} tnyDB_tmap;


TWORD * tnyDB_tmap_seek(tnyDB_tmap *tree, int value);
int tnyDB_tmap_access(tnyDB_tmap *wtree, int rowIndex);
tnyDB_tmap *tnyDB_tmap_create(int *data, int dataLength);


#endif /* TNYDB_TMAP_H_ */
