#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>




#define MIN_CHILDREN 4
#define MAX_CHILDREN 8 // MAX_CHILDREN = 2 * MIN_CHILDREN
#define MIN_KEYS 3 // MIN_KEYS = MIN_CHILDREN - 1
#define MAX_KEYS 7 // MAX_KEYS = 2 * MIN_CHILDREN - 1



// Struct that is used as nodes for the tree.
typedef struct node{
	int keys [MAX_KEYS];
	struct node * children [MAX_CHILDREN];
} node;


// Struct that is stored in file.
typedef struct page{
	int num_of_entries;
	int entry_size;
	int offset_children [MAX_CHILDREN];
	int keys[MAX_KEYS];


} page;


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
	node * root;
	char * filename;
	int * min_children;
} btree;

#endif
