
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "database.h"
#include "types.h"


int create_database (database_t * db, table_t ** pTb, char * name, int num_of_tables, ...){
    // multiple table_t * to one table_t**
    va_list args;
    va_start(args, num_of_tables);
    pTb = malloc(sizeof(column_t*)*num_of_tables);
    for (int i=0; i<num_of_tables; i++){
        pTb[i] = va_arg(args, table_t *);
    }
    va_end(args);

    db->name = name;
    db->root = malloc(sizeof(node));
    db->num_of_tables = num_of_tables;
    db->tables = pTb;
    return 0;
}


/*
* num_of_columns needs to be passed as extra param (even though included in table_metadata_t)
* because of the nature of vadriatic functions in C
*/
entry_t * create_entry (entry_t * new_entry, buffer_t buffer, table_metadata_t * tb, int num_of_columns, ...){
    if (tb->num_of_columns != num_of_columns) {
        printf("warning: returning NULL entry\n");
        return NULL;
    }
    void ** vals = malloc(sizeof(int)* tb->num_of_columns);
    buffer = (buffer_t){malloc(sizeof(float)*num_of_columns), 0, malloc(sizeof(int)*num_of_columns), 0, malloc(sizeof(char *)*num_of_columns), 0};

    va_list args;
    va_start(args, num_of_columns);
    printf("Creating entry...\n");

    for (int i=0; i<num_of_columns; i++){
        /*printf("%d) %s: %s (%d)\n", i, tb->columns[i]->name, get_type_as_string(tb->columns[i]->type), tb->columns[i]->size);*/
        switch(tb->columns[i]->type){
            case INTEGER:
                buffer.int_b[buffer.num_of_ints] = va_arg(args, int);
                vals[i] = &buffer.int_b[buffer.num_of_ints++];
                printf("Got int %d\n", *(int *) vals[i]);
                break;
            case FLOAT:
                /*buffer_float[num_of_floats] = malloc(sizeof(float));*/
                buffer.float_b[buffer.num_of_floats] = va_arg(args, double);
                vals[i] = &buffer.float_b[buffer.num_of_floats++];
                printf("Got float %f\n", *(float *) vals[i]);
                break;
            case VARCHAR:
                buffer.char_b[buffer.num_of_pChars] = va_arg(args, char *);
                vals[i] = buffer.char_b[buffer.num_of_pChars++];
                printf("Got varchar %s\n", (char *) vals[i]);
                break;
            case BOOL:
                printf("Got bool\n");
                break;
            default: 
                printf("Warning: returning NULL entry for type %s\n", get_type_as_string(tb->columns[i]->type));
                free(buffer.char_b);
                free(buffer.float_b);
                free(buffer.int_b);
                return NULL;
        }
    }
    va_end(args);
    new_entry->values = vals;
    new_entry->key = tb->last_index;
    new_entry->value = tb->last_index * 2;
    tb->last_index ++;

    printf("create entry got %p\n", new_entry->values);
    printf("returning int %d\n", *(int*) new_entry->values[0]);

    free(buffer.char_b);
    free(buffer.float_b);
    free(buffer.int_b);

    return new_entry;
}



/*
* data -> * table_t (singular)
*/
int create_table (table_t * tb, char * tb_name, int num_of_columns, ...){
    va_list args;
    va_start(args, num_of_columns);
    column_t ** columns = malloc(sizeof(column_t *)*num_of_columns);
    for (int i=0; i<num_of_columns; i++){
        columns[i] = va_arg(args, column_t *);
    }
    va_end(args);

    tb->name = tb_name;
    table_metadata_t * metadata = malloc(sizeof(table_metadata_t));
    metadata->num_of_columns = num_of_columns;
    metadata->columns = columns;
    metadata->last_index = 1;
    tb->metadata = metadata;
    tb->root = malloc(sizeof(node));
    return 0;
}


/*
* Compares the string representation of a character with the enum
*/
bool compare_type(char * name, char * type_name){
    return strcmp(name, type_name) == 0;
}


type_t determine_type (char * type_name){
    if (compare_type(type_name, "INTEGER")) {
        return INTEGER;
    } else if (compare_type(type_name, "FLOAT")){
        return FLOAT;
    } else if (compare_type(type_name, "VARCHAR")){
        return VARCHAR;
    } else if (compare_type(type_name, "BOOL")){
        return BOOL;
    } 
    printf("Error: invalid Type '%s'. Check reference for types for more information.", type_name);
    return NONE;
}


int create_column (column_t * col, char * name, char * type, int varchar_size){
    col->name = name;
    // Todo make string uppercase
    type_t t = determine_type(type);
    switch(t){
        case INTEGER: 
            col->type = INTEGER;
            col->size = sizeof(int);
            break;
        case FLOAT: 
            col->type = FLOAT;
            col->size = sizeof(float);
            break;
        case VARCHAR:
            col->type = VARCHAR;
            col->size = varchar_size;
            break;
        case BOOL:
            col->type = BOOL;
            col->size = sizeof(bool);
            break;
        default: 
            printf("Error: Could not create column because of invalid type.");
            return -1;
    }
    return 0;
}


/*
* Returns a string representation of database-types
*/
const char * get_type_as_string (type_t tp){
    switch(tp){
        case INTEGER: return "INTEGER";
        case FLOAT: return "FLOAT";
        case BOOL: return "BOOL";
        case VARCHAR: return "VARCHAR";
        case NONE: return "NONE";
    }
    return "UNKNOWN TYPE";
}


void print_sep(){
    for (int i=0; i<30; i++) printf("-");
    printf("\n");
}


void display_column(column_t * col){
    printf("- %s (%s -> %d)\n", col->name, get_type_as_string(col->type), col->size);
}

void display_table(table_t *tb){
    printf("Name: %s\n", tb->name);
    printf("Num of cols: %d\n", tb->metadata->num_of_columns);
    for (int i=0; i<tb->metadata->num_of_columns; i++){
        display_column(tb->metadata->columns[i]);
    }
}

void display_database(database_t * db){
    printf("\nDisplay of %s\n", db->name);
    print_sep();
    for (int i=0; i<db->num_of_tables; i++){
        printf("%d)\n", i+1);
        display_table(db->tables[i]);
    }
}


int add_table (database_t * db, table_t * new_tb){
    if (db == NULL || new_tb == NULL){
        return -1;
    }
    db->tables[db->num_of_tables++] = new_tb;
    return 0;
}


int add_column (table_t * tb, column_t * new_col){
    if (tb == NULL || new_col == NULL){
        return -1;
    }
    tb->metadata->columns[tb->metadata->num_of_columns++] = new_col;
    return 0;
}


