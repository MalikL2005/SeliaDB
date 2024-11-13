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
#include <dirent.h>


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
    column *columns[MAX_COLUMNS]; // *columns[number_of_columns]
    table_row *rows[MAX_ROWS];
    table_row *last_row; 
    FILE *table_file;
} table; 


typedef struct {
    char name[32];
    table *tables[MAX_TABLES];
} database;


void check_for_init_folder(){
    DIR* dir = opendir("./DB_init");
    if (dir){
        printf("DB_init-dir exists\n");
    } else {
        printf("Creating DB_init-dir\n");
        mkdir("DB_init", 0777);
    }
}


void * check_database_files(database *db_ptr){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

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
    char current_dir_path[] = "/home/malik/git_projects/DB/DB_init/"; // Get current path (-> pwd?)
    char *dir_name = malloc(strlen(name) + strlen(current_dir_path));
    strcpy(dir_name, current_dir_path);
    strcat(dir_name, name);
    printf("New_dir: %s\n", dir_name);
    // Check if dir already exists
    struct stat s = {0};
    if (!stat(dir_name, &s)){
        printf("Dir %s already exists.", dir_name);
        return NULL;
    }

    int result = mkdir(dir_name, 0777);
    database *mydb = malloc(sizeof(database));
    strcpy(mydb->name, name);
    return mydb;
}


table *create_table(database **pDb, char *table_name, int number_of_columns){

    // Create table-file in current_DB dir 
    FILE *fileptr;
    char * new_table_file = malloc(sizeof(table_name) + sizeof("DB_init/") + sizeof((*pDb)->name) + 5);
    strcpy(new_table_file, "DB_init/");
    strcat(new_table_file, (*pDb)->name);
    strcat(new_table_file, "/");
    strcat(new_table_file, table_name);
    strcat(new_table_file, ".bin");
    printf("Full file path: %s\n", new_table_file);
    fileptr = fopen(new_table_file, "wb");
    if (fileptr == NULL){
        return NULL; 
    }
    // Writing to file is still buggy 
    fwrite("Hi", sizeof("Hi"), 1, fileptr);

    // Create new table 
    table *new_table = (table *) malloc(sizeof(table));
    strcpy(new_table->name, table_name);
    new_table->number_of_columns = number_of_columns;
    new_table->table_file = fileptr;
    return new_table;
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
    (*new_row->values) = *values;

    // append row to table-file 
    fprintf(tb->table_file, *new_row->values);

    // append *new_row to table->rows
    tb->last_row = new_row;
}
