#define MAX_TABLE_LENGTH 32
#define MAX_TABLES 10
#define MAX_DB_NAME_LENGTH 32
#define MAX_COLUMN_NAME_LENGTH 32
#define MAX_INDEX_NAME_LENGTH 32
#define MAX_COLUMNS 100
#define MAX_DATABASES 5

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


typedef struct {
    char name [MAX_COLUMN_NAME_LENGTH];
    char type[20];
} column; 


typedef struct {
    char ** values; 
    struct table_row *next_row; 
    struct table *table_meta_data;
} table_row;


typedef struct {
    char name [MAX_TABLE_LENGTH];
    int number_of_columns; 
    column *columns[MAX_COLUMNS];
    table_row *rows[];
} table; 


typedef struct {
    char name[32];
    table *tables[MAX_TABLES];
} database;


void show_tables(database *db){
    printf("\n%s\n", db->name);
    for (int i=0; i<MAX_TABLES; i++){
        if (db->tables[i] == NULL){
            return;
        }
        printf("|\n|---- %s\n", db->tables[i]->name);
    }
}


void show_columns(table *table){
    printf("\n%s\n", table->name);
    for (int i=0; i<(table->number_of_columns); i++){
        printf("|\n|---- %s\n", table->columns[i]->name);
    }
}

database *create_db(char *name){
    database *mydb = malloc(sizeof(database));
    strcpy(mydb->name, name);
    return mydb;
}


table *create_table(char *table_name, int number_of_columns, char *column_names[]){
    // Create new table 
    table *new_table = (table *) malloc(sizeof(table)); 
    strcpy(new_table->name, table_name);
    new_table->number_of_columns = number_of_columns;
    
    for (int i=0; i<number_of_columns; i++){
        
        // Create new column 
        column *new_col = (column *) malloc(sizeof(column));
        strcpy(new_col->name, column_names[i]);
        
        // Add new_col to new_table.columns
        new_table->columns[i] = new_col;
    }
    printf("%s\n", (new_table->columns[0]->name));
    return (table *) new_table;
}


int add_table_to_db(database *db, table *table){
    for (int i=0; i<MAX_TABLES; i++){
        if (db->tables[i] == NULL){
            db->tables[i] = table; 
            return 0;
        }
        printf("%s\n", db->tables[i]->name);
    }
    return 1;
}


void add_row (table *tb, int number_of_columns, char *values[]){
    table_row *new_row = malloc(sizeof(table_row));
    // append *new_row to table->rows
}
