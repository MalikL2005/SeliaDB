#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>




#define MIN_CHILDREN 5
#define MAX_CHILDREN 10 // MAX_CHILDREN = 2 * MIN_CHILDREN
#define MIN_KEYS 4 // MIN_KEYS = MIN_CHILDREN - 1
#define MAX_KEYS 9 // MAX_KEYS = 2 * MIN_CHILDREN - 1



// Struct that is used as nodes for the tree.
typedef struct node{
	int keys [MAX_KEYS];
	struct node * children [MAX_CHILDREN];
} node;

// Declaration of root node
// The node * root is essential, as it points to the root of the currently selected btree.
struct node * root;

#define MAX_BTREE_NAME_LENGTH 32


void traverse(node *current);
node * findValue(int value, node * current);
int getNodeIndex(node * current, int value);
int getIndexGreatestValue(node * current);



typedef struct btree {
	char * name;
	struct node * root;
} btree;

#endif
