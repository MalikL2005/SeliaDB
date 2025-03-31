
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "database.h"
#include "types.h"


database_t * create_database (char * name, int num_of_tables, ...){
    // multiple table_t * to one table_t**
    va_list args;
    va_start(args, num_of_tables);
    table_t ** tables = malloc(sizeof(table_t *));
    for (int i=0; i<num_of_tables; i++){
        tables[i] = va_arg(args, table_t *);
    }
    va_end(args);

    database_t * db = malloc(sizeof(database_t));
    db->name = name;
    db->root = malloc(sizeof(node));
    db->num_of_tables = num_of_tables;
    db->tables = tables;
    return db;
}


/*
* num_of_columns needs to be passed as extra param (even though included in table_metadata_t)
* because of the nature of vadriatic functions in C
*/
entry_t * create_entry (table_metadata_t * tb, int num_of_columns, ...){
    if (tb->num_of_columns != num_of_columns) {
        return NULL;
    }
    void ** vals = malloc(sizeof(int)* tb->num_of_columns);
    entry_t * new_entry = malloc(sizeof(entry_t));

    va_list args;
    va_start(args, num_of_columns);
    printf("Creating entry...\n");
    float * buffer_float = malloc(sizeof(float) * tb->num_of_columns);
    int num_of_floats = 0;
    char ** buffer_varchar = malloc(sizeof(char *) * tb->num_of_columns);
    int num_of_pChars = 0;
    int * buffer_int = malloc(sizeof(int) * tb->num_of_columns);
    int num_of_ints = 0; 

    for (int i=0; i<num_of_columns; i++){
        printf("%d) %s: %s (%d)\n", i, tb->columns[i]->name, get_type_as_string(tb->columns[i]->type), tb->columns[i]->size);
        switch(tb->columns[i]->type){
            case INTEGER:
                buffer_int[num_of_ints] = va_arg(args, int);
                vals[i] = &buffer_int[num_of_ints++];
                printf("Got int %d\n", *(int *) vals[i]);
                break;
            case FLOAT:
                /*buffer_float[num_of_floats] = malloc(sizeof(float));*/
                buffer_float[num_of_floats] = va_arg(args, double);
                vals[i] = &buffer_float[num_of_floats++];
                printf("Got float %f\n", *(float *) vals[i]);
                break;
            case VARCHAR:
                buffer_varchar[num_of_pChars] = va_arg(args, char *);
                vals[i] = buffer_varchar[num_of_pChars++];
                free(buffer_varchar[num_of_pChars]);
                printf("Got varchar %s\n", (char *) vals[i]);
                break;
            default: return NULL;
        }
    }
    va_end(args);
    new_entry->values = vals;
    new_entry->key = tb->last_index;
    new_entry->value = tb->last_index * 2;
    tb->last_index ++;

    free(buffer_varchar);
    free(buffer_float);
    free(buffer_int);

    /*printf("create entry got %p\n", new_entry->values);*/
    /*printf("got %f\n", *(float*) new_entry->values[0]);*/
    /*printf("got addr %p\n", new_entry->values[0]);*/
    /*printf("got %s\n", (char*) new_entry->values[1]);*/

    return new_entry;
}



/*
* data -> * table_t (singular)
*/
table_t * create_table (char * tb_name, int num_of_columns, ...){
    va_list args;
    va_start(args, num_of_columns);
    column_t ** columns = malloc(sizeof(column_t *));
    for (int i=0; i<num_of_columns; i++){
        columns[i] = va_arg(args, column_t *);
    }
    va_end(args);

    table_t * tb = malloc(sizeof(table_t));
    tb->name = tb_name;
    table_metadata_t * metadata = malloc(sizeof(table_metadata_t));
    metadata->num_of_columns = num_of_columns;
    metadata->columns = columns;
    metadata->last_index = 1;
    tb->metadata = metadata;
    tb->root = malloc(sizeof(node));
    return tb;
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


column_t * create_column (char * name, char * type, int varchar_size){
    column_t * col = malloc(sizeof(column_t));
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
            col->size = sizeof(BOOL);
            break;
        default: 
            printf("Error: Could not create column because of invalid type.");
            return NULL;
    }
    return col;
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
    char * tp;
    switch(col->type){
        case INTEGER: tp="INTEGER"; break;
        case FLOAT: tp="FLOAT"; break;
        case VARCHAR: tp="VARCHAR"; break;
        case BOOL: tp="BOOL"; break;
        case NONE: tp="NONE";
    }
    printf("- %s (%s -> %d)\n", col->name, tp, col->size);
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


