#define MAX_TABLE_LENGTH 32
#define MAX_TABLES 10
#define MAX_DB_NAME_LENGTH 32
#define MAX_TABLE_NAME_LENGTH 32
#define MAX_COLUMN_NAME_LENGTH 32
#define MAX_INDEX_NAME_LENGTH 32
#define MAX_COLUMNS 100
#define MAX_DATABASES 5
#define MAX_ROWS 200

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
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
    table_row *rows[MAX_ROWS];
    FILE *table_file;
} table; 


typedef struct {
    char name[32];
    table *tables[MAX_TABLES];
} database;


void check_database_files(database *db_ptr){
    printf("Checking DB-files...\n");
    database *test_db = malloc(sizeof(database));
    strcpy(test_db->name, "test worked");
    db_ptr = test_db;
}


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
    char current_dir_path[] = "/home/malik/git_projects/DB/"; // Get current path (-> pwd?)
    char *dir_name = malloc(strlen(name) + strlen(current_dir_path));
    strcpy(dir_name, current_dir_path);
    strcat(dir_name, name);
    printf("New_dir: %s\n", dir_name);
    int result = mkdir(dir_name, 0777);
    database *mydb = malloc(sizeof(database));
    strcpy(mydb->name, name);
    return mydb;
}


table *create_table(database **pDb, char *table_name, int number_of_columns){
    // Create table-file in current_DB dir 
    

    // Create new table 
    table *new_table = (table *) malloc(sizeof(table)); 
    strcpy(new_table->name, table_name);
    new_table->number_of_columns = number_of_columns;
    // Add table to current DB 
    for (int i=0; i<MAX_TABLES; i++){
        if (!(*pDb)->tables[i]){
            (*pDb)->tables[i] = new_table;

            // Make table file
            char *file_path = malloc(sizeof((*pDb)->name) + sizeof(table_name) + 5);
            strcpy(file_path, (*pDb)->name);
            strcat(file_path, "/");
            strcat(file_path, table_name);
            strcat(file_path, ".txt");
            printf("%s", file_path);
            (*pDb)->tables[i]->table_file = fopen(file_path, "w"); // fopen ("<db-dirname>/<table-filename>", "w")
            fprintf((*pDb)->tables[i]->table_file, "Col1\tCol2\tCol3\n");

            printf("\t%d\n", i);
            break;
        }
    }


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
