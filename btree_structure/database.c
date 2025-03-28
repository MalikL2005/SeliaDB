
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "database.h"
#include "types.h"


database_t * create_database (char * name, int num_of_tables, table_t * tables){
    database_t * db = malloc(sizeof(database_t));
    db->name = name;
    db->root = malloc(sizeof(node));
    db->num_of_tables = num_of_tables;
    db->tables = tables;
    return db;
}


table_t * create_table (char * tb_name, int num_of_columns, column_t * columns){
    table_t * tb = malloc(sizeof(table_t));
    tb->name = tb_name;
    table_metadata_t * metadata = malloc(sizeof(table_metadata_t));
    metadata->num_of_columns = num_of_columns;
    metadata->columns = columns;
    tb->metadata = *metadata;
    return tb;
}


/*
* Compares a 
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


void print_sep(){
    for (int i=0; i<30; i++) printf("-");
    printf("\n");
}


void display_column(column_t col){
    char * tp;
    switch(col.type){
        case INTEGER: tp="INTEGER"; break;
        case FLOAT: tp="FLOAT"; break;
        case VARCHAR: tp="VARCHAR"; break;
        case BOOL: tp="BOOL"; break;
        case NONE: tp="NONE";
    }
    printf("- %s (%s -> %d)\n", col.name, tp, col.size);
}

void display_table(table_t tb){
    printf("Name: %s\n", tb.name);
    printf("Num of cols: %d\n", tb.metadata.num_of_columns);
    for (int i=0; i<tb.metadata.num_of_columns; i++){
        printf("%d)\n", i);
        display_column(tb.metadata.columns[i]);
    }
}

void display_database(database_t * db){
    printf("Display of %s\n", db->name);
    print_sep();
    for (int i=0; i<db->num_of_tables; i++){
        printf("%d)\n", i+1);
        display_table(db->tables[i]);
    }
}

