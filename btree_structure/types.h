#ifndef TYPES_H
#define TYPES_H

#define MIN_CHILDREN 5
#define MAX_CHILDREN 10 // MAX_CHILDREN = 2 * MIN_CHILDREN
#define MIN_KEYS 4 // MIN_KEYS = MIN_CHILDREN - 1
#define MAX_KEYS 9 // MAX_KEYS = 2 * MIN_CHILDREN - 1





typedef struct varchar_entry_t {
    int length;
    char * value;
} varchar_entry_t;


typedef struct entry_t {
    int key;
    int value;
    void ** values;
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
    BOOL,
    NONE
} type_t;


typedef struct column_t {
    char * name;
    type_t type;
    int size;
} column_t;


typedef struct table_metadata_t {
    int num_of_columns;
    column_t ** columns;
    int last_index;
} table_metadata_t;


typedef struct table_t {
    table_metadata_t * metadata;
    char * name;
} table_t;


typedef struct database_t {
    char * name;
    int num_of_tables;
    table_t ** tables;
    node * root;
} database_t;


#endif
