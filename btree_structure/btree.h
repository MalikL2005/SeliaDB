#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>




#define MIN_CHILDREN 5
#define MAX_CHILDREN 10 // MAX_CHILDREN = 2 * MIN_CHILDREN
#define MIN_KEYS 4 // MIN_KEYS = MIN_CHILDREN - 1
#define MAX_KEYS 9 // MAX_KEYS = 2 * MIN_CHILDREN - 1


typedef struct entry_t {
    int key;
    int value;
} entry_t;


// Struct that is used as nodes for the tree.
typedef struct node {
	entry_t entries [MAX_KEYS];
	struct node * children [MAX_CHILDREN];
} node;


typedef enum type_t {
    INTEGER,
    FLOAT,
    VARCHAR,
    BOOL
} type_t;


typedef struct column_t {
    type_t type;
    int size;
} column_t;


typedef struct table_metadata_t {
    int num_of_columns;
    column_t * columns;
} table_metadata_t;


typedef struct table_t {
    table_metadata_t metadata;
    char * name;
} table_t;


typedef struct database_t {
    char * name;
    int num_of_tables;
    table_t * tables;
} database_t;


// Declaration of root node
// The node * root is essential, as it points to the root of the currently selected btree.
struct node * root;

#define MAX_BTREE_NAME_LENGTH 32


void traverse(node *current);
node * findValue(int id, node * current);
int getNodeIndex(node * current, int id);
int getIndexGreatestValue(node * current);



typedef struct btree {
	char * name;
	struct node * root;
} btree;

#endif
