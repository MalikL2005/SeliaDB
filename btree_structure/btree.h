#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "database.h"






// Declaration of root node
// The node * root is essential, as it points to the root of the currently selected btree.
/*struct node * root;*/

#define MAX_BTREE_NAME_LENGTH 32


void traverse(node * current, table_t * tb);
node * findValue(int id, node * current);
int getNodeIndex(node * current, int id);
int getIndexGreatestValue(node * current);



typedef struct btree {
	char * name;
	struct node * root;
} btree;

#endif
