#define MAX_TABLE_LENGTH 32
#define MAX_TABLES 10
#define MAX_DB_NAME_LENGTH 32
#define MAX_TABLE_NAME_LENGTH 10
#define MAX_COLUMN_NAME_LENGTH 32
#define MAX_INDEX_NAME_LENGTH 32
#define MAX_COLUMNS 10
#define MAX_DATABASES 5
#define MAX_ROWS 20
#define FILE_SIZE 2048
#define LOCATION_NUMBER_OF_COLUMNS MAX_TABLE_NAME_LENGTH
#define LOCATION_NUMBER_OF_ENTRIES MAX_TABLE_NAME_LENGTH + sizeof(int)
#define LOCATION_FIRST_INDEX_POINTER MAX_TABLE_NAME_LENGTH + sizeof(int)*2
#define LOCATION_LAST_INDEX_POINTER MAX_TABLE_NAME_LENGTH + sizeof(int)*3
#define LOCATION_LAST_ENTRY_POINTER MAX_TABLE_NAME_LENGTH + sizeof(int)*4

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "add_entry.c"


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
    int number_of_entries;
    table_row *rows[MAX_ROWS];
    table_row *last_row; 
    FILE *table_file;
    char *file_name; 
    int pLastEntry;
    int * pFirstIndex; 
    int * pLastIndex;
    int test_index; 
} table; 


typedef struct {
    char name[MAX_DB_NAME_LENGTH];
    table *tables[MAX_TABLES];
} database;


void check_for_init_folder(){
    DIR* dir = opendir("./DB_init.db");
    if (dir){
        printf("DB_init-dir exists\n");
    } else {
        printf("Creating DB_init-dir\n");
        mkdir("DB_init.db", 0777);
    }
}


void * check_database_files(database *db_ptr){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

}


void show_tables(database *db){
    //printf("\n%s\n", db->name);
    for (int i=0; i<MAX_TABLES; i++){
        if (db->tables[i] == NULL){
            return;
        }
        printf("|\n|---- %s\n", db->tables[i]->name);
    }
}


void show_columns(database *db, table *tb){
    printf("\n%s\n", db->name);
    for (int i=0; i<(tb->number_of_columns); i++){
        printf("|\n|---- %s\n", tb->columns[i]->name);
    }
}

database *create_db(char *name){
    char current_dir_path[] = "/home/malik/git_projects/DB/DB_init.db/"; // Get current path (-> pwd?)
    char *dir_name = malloc(strlen(name) + strlen(current_dir_path));
    strcpy(dir_name, current_dir_path);
    strcat(dir_name, name);
    printf("New_dir: %s\n", dir_name);
    // Check if dir already exists
    struct stat s = {0};
    if (!stat(dir_name, &s)){
        printf("Dir %s already exists.\n", dir_name);
        return NULL;
    }

    int result = mkdir(dir_name, 0777);
    database *mydb = malloc(sizeof(database));
    strcpy(mydb->name, name);
    return mydb;
}


table *create_table(database **pDb, char *table_name, int *number_of_columns, char *column_names[MAX_COLUMNS]){

    if (strlen(table_name) > MAX_TABLE_NAME_LENGTH){
        printf("Error: Table name is too long.\n");
        return NULL; 
    }

    // Create table-file in current_DB dir 
    FILE *fileptr;
    char * new_table_file = malloc(sizeof(table_name) + sizeof("DB_init.db/") + sizeof((*pDb)->name)+5);
    strcpy(new_table_file, "DB_init.db/");
    strcat(new_table_file, (*pDb)->name);
    strcat(new_table_file, "/");
    strcat(new_table_file, table_name);
    strcat(new_table_file, ".bin");
    printf("Full file path: %s\n", new_table_file);
    fileptr = fopen(new_table_file, "wb");
    if (fileptr == NULL){
        printf("Error: Could not create table_file\n");
        return NULL; 
    }

    //truncate file to specific length 
    if (ftruncate(fileno(fileptr), FILE_SIZE) != 0){
        printf("Error: Could not truncate file properly.\n");
        return NULL;
    }

    // Create new table 
    table *new_table = (table *) malloc(sizeof(table) + 4);
    strcpy(new_table->name, table_name);
    new_table->number_of_columns = *number_of_columns;
    new_table->table_file = fileptr;
    new_table->file_name = new_table_file; 
    new_table->number_of_entries = 0;
    new_table->test_index = 0; 
    new_table->pLastEntry = FILE_SIZE;
    // Add table to DB 
    for (int i = 0; i<MAX_TABLES; i++){
        if (!(*pDb)->tables[i]){
            (*pDb)->tables[i] = new_table; 
            printf("New table %s in %s\n", (*pDb)->name, new_table->name);
            break;
        }
    }
    // column names 
    for (int i=0; i<*number_of_columns; i++){
        printf("%s\n", column_names[i]);
    }

    // Writing to file 
    int * pNum_of_cols = number_of_columns;
    fseek(fileptr, 0, SEEK_SET);
    fwrite(table_name, MAX_TABLE_NAME_LENGTH, 1, fileptr); // table-name

    fseek(fileptr, LOCATION_NUMBER_OF_COLUMNS, SEEK_SET);
    fwrite(&(new_table->number_of_columns), sizeof(int), 1, fileptr); // number of cols 

    fseek(fileptr, LOCATION_NUMBER_OF_ENTRIES, SEEK_SET);
    fwrite(&(new_table->number_of_entries), sizeof(int), 1, fileptr); //number of entries

    // write pointer to last Entry Some issue :(
    int test_write = FILE_SIZE; 
    printf("LAST ENTRY: %d\n", test_write);
    fseek(fileptr, LOCATION_LAST_ENTRY_POINTER, SEEK_SET);
    fwrite(&test_write, sizeof(int), 1, fileptr);

    // write pointer to first & last index
    int * pLastIndex= malloc(sizeof(int));
    *pLastIndex = LOCATION_LAST_ENTRY_POINTER + MAX_COLUMN_NAME_LENGTH * (*number_of_columns);
    fseek(fileptr, LOCATION_FIRST_INDEX_POINTER, SEEK_SET);
    printf("Current position: %d\n", ftell(fileptr));
    printf("START_INDEX_VALUES: %d\n", *pLastIndex);
    fwrite(pLastIndex, sizeof(*pLastIndex), 1, fileptr);
    fseek(fileptr, LOCATION_LAST_INDEX_POINTER, SEEK_SET);
    fwrite(pLastIndex, sizeof(*pLastIndex), 1, fileptr);

    // write column names 
    fseek(fileptr, LOCATION_LAST_ENTRY_POINTER + sizeof(int), SEEK_SET);
    for (int i=0; i<*number_of_columns; i++){
        printf("%d) %s\n", i, column_names[i]);
        // fwrite(column_names[i], MAX_COLUMN_NAME_LENGTH, 1, fileptr);
        fwrite(column_names[i], strlen(column_names[i]) +1, 1, fileptr);
        // fwrite('\0', sizeof(char), 1, fileptr);
    }

    fclose(fileptr);
    return new_table;
}



void add_row (table *tb, char **values){
    // append row to table-file
    FILE * pFile = fopen(tb->file_name, "rb+");
    if (!pFile){
        printf("Error: Could not open file.\n");
        return;
    }
    printf("Printing row to file %s\n", tb->file_name);

    // increment number of entries by one (at SEEK_SET + MAX_TABLE_NAME_LENGTH + sizeof(number_of_columns))
    // read num of columns
    int *temp_noc = malloc(sizeof(int));
    fseek(pFile, MAX_TABLE_NAME_LENGTH, SEEK_SET);
    fread(temp_noc, sizeof(*temp_noc), 1, pFile);
    free(temp_noc);

    // increment number of entries 
    int *new_number_of_entries = malloc(sizeof(int));
    increment_number_of_entries(pFile);
    fseek(pFile, MAX_TABLE_NAME_LENGTH + sizeof(*new_number_of_entries), SEEK_SET);
    fread(new_number_of_entries, sizeof(*new_number_of_entries), 1, pFile);
    printf("New number of entries: %d\n", *new_number_of_entries); 


    // read pointer LastIndex
    int *pLastIndex = malloc(sizeof(int));
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*2), SEEK_SET);
    fread(pLastIndex, sizeof(*pLastIndex), 1, pFile);
    printf("Last index location: %d\n", *pLastIndex);

    // get last_index value 
    fseek(pFile, *pLastIndex, SEEK_SET);
    int * last_index = malloc(sizeof(int));
    fread(last_index, sizeof(*last_index), 1, pFile);
    (*last_index) ++;

    // increment last index pointer 
    (*pLastIndex) += sizeof(int)*2;
    printf("new index location: %d\n", *pLastIndex);
    update_last_index(pFile);
    // read pLastEntry 
    int *pLastEntry = malloc(sizeof(int));
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*3), SEEK_SET);
    fread(pLastEntry, sizeof(*pLastEntry), 1, pFile);
    printf("Last entry: %d\n", *pLastEntry);
    (*pLastEntry) -= sizeof(int); // -= SIZEOF(ENTRY)
    printf("new entry: %d\n", *pLastEntry);

    pFile = fopen(tb->file_name, "rb+");
    // update last index pointer 
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*2), SEEK_SET);
    //fwrite(pLastIndex, sizeof(*pLastIndex), 1, pFile);
    putw(*pLastIndex, pFile);
    fseek(pFile, *pLastIndex, SEEK_SET);
    printf("Current pos @ %d\n", ftell(pFile));
    // write index 
    fwrite(last_index, sizeof(*last_index), 1, pFile);
    printf("New current pos @ %d\n", ftell(pFile));
    // write pointer to value after index 
    fseek(pFile, *pLastIndex +sizeof(int), SEEK_SET);
    printf("pLastEntry: %d\n", *pLastEntry);
    fwrite(pLastEntry, sizeof(*pLastEntry), 1, pFile);
    fseek(pFile, -sizeof(int), SEEK_CUR);
    printf("New (2) current pos @ %d\n", ftell(pFile));
    int num;
    fread(&num, sizeof(num), 1, pFile);
    printf("Read num %d\n", num);
    free(last_index);
    free(pLastIndex);

    // update last Entry  
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*3), SEEK_SET);
    fwrite(pLastEntry, sizeof(*pLastEntry), 1, pFile);
    printf("Last entry at location: %d\n", *pLastEntry);

    // insert row-values 
    fseek(pFile, *pLastEntry, SEEK_SET);
    char * test_row_values = "Edos";
    // still need to adjust size 
    fwrite(test_row_values, sizeof(char)*4, 1, pFile); // bug: writes test_row_values but deletes all values before 
    char * buffer_values = malloc(FILE_SIZE - (*pLastEntry));

    fseek(pFile, *pLastEntry, SEEK_SET);
    printf("Reading %d Bytes from %d\n", (FILE_SIZE - (*pLastEntry)), *pLastEntry);
    fread(buffer_values, (FILE_SIZE - (*pLastEntry)), 1, pFile);
    printf("Read buffer values: %s\n", buffer_values);
    free(pLastEntry);

    fclose(pFile);
}


int read_databases(database* DBS[]){
    DIR* db_init_folder = opendir("DB_init.db");
    int files = 0;
    if (!db_init_folder){
        printf("Error: No DB_init.db folder.\n");
        return 1;
    }
    struct dirent *db_init_sub_dir;

    // check all current dbs 
    for (int i=0; i< MAX_DATABASES; i++){
        printf("DB %d: %s\n", i, DBS[i]);
    }

    // iterate over all dirs in DB_init-folder 
    while (db_init_sub_dir = readdir(db_init_folder)){
        // Remove all dirs that are not DBS 
        if (strcmp(db_init_sub_dir->d_name, ".") || strcmp(db_init_sub_dir->d_name, "..")){
            printf("Rm: ");
        }
        files ++;
        printf("File no %d: '%s'\n", files, db_init_sub_dir->d_name);

        // create new db 
        database * new_db = create_db(db_init_sub_dir->d_name);
        // list all files in folder (that are .bin) 

        // if MAX_DATABASES < all dirs in current folder: 

        //      for file in files_of_current_dir: 
        //          if MAX_TABLES < file_index:
        //              create_table_from_file(file) 

    }
    printf("Returning to main function.\n");
    return 0;
}


table * create_table_from_file(char * filename){
    FILE * pFile = fopen(filename, "rb");
    if (!pFile){
        printf("Could not read file %s\n", filename);
        return NULL;
    }
    table *tb = malloc(sizeof(table));

    // get length 
    fseek(pFile, 0, SEEK_END);
    printf("Length of file: %d\n", ftell(pFile));

    // name of table 
    fseek(pFile, 0, SEEK_SET);
    fread(tb->name, sizeof(char), MAX_TABLE_NAME_LENGTH, pFile);
    printf("%d: table_name: %s\n", ftell(pFile), tb->name);

    // number_of_columns
    fseek(pFile, LOCATION_NUMBER_OF_COLUMNS, SEEK_SET);
    fread(&(tb->number_of_columns), sizeof(int), 1, pFile);
    printf("%d: Number of cols: %d\n", ftell(pFile), tb->number_of_columns);

    // Column_names 
    char ** columns = malloc(sizeof(char*) * tb->number_of_columns);
    fseek(pFile, LOCATION_LAST_ENTRY_POINTER+sizeof(int), SEEK_SET);
    char * temp = malloc(sizeof(char));
    int start;

    // Read each character until \0 
    // Then read all previous chars to memory 
    for (int i=0; i<tb->number_of_columns; i++){
        start = 0; 
        do {
            fread(temp, sizeof(char), 1, pFile);
            start ++; 
        } while(*temp != '\0');
        *(columns + i) = malloc(start * sizeof(char));
        fread(*(columns +i), start*sizeof(char), 1, pFile);
        printf("Column name %d) %s\n", i+1, *(columns+i));
    }

    // number_of_entries 
    fseek(pFile, LOCATION_NUMBER_OF_ENTRIES, SEEK_SET);
    fread(&(tb->number_of_entries), sizeof(int), 1, pFile);
    printf("%d: Number of entries: %d\n", ftell(pFile),tb->number_of_entries);

    // first index 
    fseek(pFile, LOCATION_FIRST_INDEX_POINTER, SEEK_SET);
    fread(&(tb->pFirstIndex), sizeof(int), 1, pFile);
    printf("%d: First index: %d\n", ftell(pFile),tb->pFirstIndex);

    // last index 
    fseek(pFile, LOCATION_LAST_INDEX_POINTER, SEEK_SET);
    fread(&(tb->pLastIndex), sizeof(int), 1, pFile);
    printf("%d: Last index location: %d\n", ftell(pFile), tb->pLastIndex);
    fseek(pFile, LOCATION_LAST_INDEX_POINTER, SEEK_SET);
    int* last_index_value = malloc(sizeof(int));
    fread(last_index_value, sizeof(*last_index_value), 1, pFile);
    printf("%d: Last index value: %d\n", ftell(pFile), *last_index_value);

    // last entry 
    fseek(pFile, LOCATION_LAST_ENTRY_POINTER, SEEK_SET);
    fread(&(tb->pLastEntry), sizeof(int), 1, pFile);
    printf("%d: Last entry: %d\n", ftell(pFile), tb->pLastEntry);
    //fseek(pFile, tb->pLastEntry, SEEK_SET);
    //char *last_entry_value = malloc(sizeof(char)* 4);
    //fread(last_entry_value, strlen(last_entry_value), 1, pFile);
    //printf("Read last entry: %s\n", last_entry_value - sizeof(char)*4);
}

