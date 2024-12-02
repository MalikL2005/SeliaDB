#define MAX_TABLE_LENGTH 32
#define MAX_TABLES 10
#define MAX_DB_NAME_LENGTH 32
#define MAX_TABLE_NAME_LENGTH 10
#define MAX_COLUMN_NAME_LENGTH 32
#define MAX_INDEX_NAME_LENGTH 32
#define MAX_COLUMNS 100
#define MAX_DATABASES 5
#define MAX_ROWS 200
#define START_ENTRY_POINTERS MAX_TABLE_NAME_LENGTH + sizeof(int)*4
#define FILE_SIZE 1024

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
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
    int number_of_entries;
    table_row *rows[MAX_ROWS];
    table_row *last_row; 
    FILE *table_file;
    char *file_name; 
    int * pLastEntry;
    int * pLastIndex;
    int test_index; 
} table; 


typedef struct {
    char name[32];
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


table *create_table(database **pDb, char *table_name, int number_of_columns){

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
    new_table->number_of_columns = number_of_columns;
    new_table->table_file = fileptr;
    new_table->file_name = new_table_file; 
    new_table->number_of_entries = 0;
    new_table->test_index = 0; 
    // Add table to DB 
    for (int i = 0; i<MAX_TABLES; i++){
        if (!(*pDb)->tables[i]){
            (*pDb)->tables[i] = new_table; 
            printf("New table %s in %s\n", (*pDb)->name, new_table->name);
            break;
        }
    }

    // Writing to fileptr 
    int * pNum_of_cols = &number_of_columns;
    fwrite(table_name, MAX_TABLE_NAME_LENGTH, 1, fileptr); // table-name
    fwrite(&(new_table->number_of_columns), sizeof(int), 1, fileptr); // number of cols 
    fwrite(&(new_table->number_of_entries), sizeof(int), 1, fileptr); //number of entries
    fseek(fileptr, START_ENTRY_POINTERS, SEEK_SET);
    fwrite(&(new_table->test_index), sizeof(int), 1, fileptr);
    // create dummy columns values 
    char ** values = malloc(sizeof(char*)*number_of_columns);
    printf("Here\n");
    for (int i=0; i<number_of_columns; i++){
        values[i] = malloc(MAX_COLUMN_NAME_LENGTH);
        strcpy(values[i], "Column Number");
    }

    for (int i=0; i<number_of_columns; i++){
        fwrite(values[i], MAX_COLUMN_NAME_LENGTH, 1, fileptr);
    }

    // write pointer to last Entry 
    int *pLastEntry = malloc(sizeof(int));
    *pLastEntry = FILE_SIZE;
    fseek(fileptr, (MAX_TABLE_NAME_LENGTH + sizeof(int)*3), SEEK_SET);
    fwrite(pLastEntry, sizeof(*pLastEntry), 1, fileptr);


    // write pointer to last entry pointer to file 
    int * pLastIndex= malloc(sizeof(int));
    *pLastIndex = START_ENTRY_POINTERS;
    fseek(fileptr, (MAX_TABLE_NAME_LENGTH + sizeof(int)* 2), SEEK_SET);
    fwrite(pLastIndex, sizeof(*pLastIndex), 1, fileptr);

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

    // read current num of entries 
    int *new_number_of_entries = malloc(sizeof(int));
    fseek(pFile, MAX_TABLE_NAME_LENGTH + sizeof(*new_number_of_entries), SEEK_SET);
    fread(new_number_of_entries, sizeof(*new_number_of_entries), 1, pFile);
    (*new_number_of_entries) ++;

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
    (*pLastIndex) += sizeof(int);
    printf("new index location: %d\n", *pLastIndex);

    // read pLastEntry 
    int *pLastEntry = malloc(sizeof(int));
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*3), SEEK_SET);
    fread(pLastEntry, sizeof(*pLastEntry), 1, pFile);
    printf("Last entry: %d\n", *pLastEntry);
    (*pLastEntry) -= sizeof(int); // -= SIZEOF(ENTRY)
    printf("new entry: %d\n", *pLastEntry);
    
    // create new file to copy half of it 
    FILE *tempFile = fopen("DB_init.db/temp_file.bin", "wb"); // fopen("DB_init.db/<SELECTED_DB_NAME>/temp_file.bin", "wb");
    if (!tempFile){
        printf("Error: Could not create temporary file.\n");
        return; 
    }
    long position_in_file = ftell(pFile) - sizeof(int);
    char *buffer_front = malloc(position_in_file);
    rewind(pFile);
    fread(buffer_front, sizeof(char), position_in_file, pFile);
    fwrite(buffer_front, MAX_TABLE_NAME_LENGTH + sizeof(*temp_noc), 1, tempFile);

    // Write updated value to file
    fwrite(new_number_of_entries, sizeof(*new_number_of_entries), 1, tempFile);
    fseek(pFile, 0, SEEK_END);
    int position_after_noe = ftell(tempFile);

    // length of file
    fseek(pFile, 0, SEEK_END);
    long int length = ftell(pFile); 

    // Copy rest of file 
    char * buffer_back = malloc(length - position_after_noe);
    fread(buffer_back, sizeof(char), length - position_after_noe, pFile);
    fwrite(buffer_back, length - position_after_noe, 1, tempFile);
    fseek(tempFile, 0, SEEK_END);
    position_in_file = ftell(tempFile);
    printf("Position in tempfile: %d\n", position_in_file);

    // copy all the content of tempFile to pFile 
    pFile = fopen(tb->file_name, "wb");
    fwrite(buffer_front, MAX_TABLE_NAME_LENGTH + sizeof(*temp_noc), 1, pFile);
    free(buffer_front);
    fwrite(new_number_of_entries, sizeof(*new_number_of_entries), 1, pFile);
    free(new_number_of_entries);
    fwrite(buffer_back, length - position_after_noe, 1, pFile);
    free(buffer_back);

    // update last index pointer 
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*2), SEEK_SET);
    fwrite(pLastIndex, sizeof(*pLastIndex), 1, pFile);
    fseek(pFile, *pLastIndex, SEEK_SET);
    printf("Current pos @ %d\n", ftell(pFile));
    fwrite(last_index, sizeof(*last_index), 1, pFile);
    printf("Last index: %d\n", *pLastIndex);
    free(last_index);
    free(pLastIndex);

    // update last Entry  
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*3), SEEK_SET);
    fwrite(pLastEntry, sizeof(*pLastEntry), 1, pFile);
    printf("Last entry at location: %d\n", *pLastEntry);

    // insert row-values 
    fseek(pFile, *pLastEntry, SEEK_SET);
    char * test_row_values = "Hi!";
    fwrite(test_row_values, sizeof(test_row_values)*4, 1, pFile); // bug: writes test_row_values but deletes all values before 
    free(pLastEntry);

    fclose(tempFile);
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
    // get name of table 
    fread(tb->name, sizeof(char), MAX_TABLE_NAME_LENGTH, pFile);
    printf("table_name: %s\n", tb->name);

    // get number_of_columns
    fseek(pFile, MAX_TABLE_NAME_LENGTH, SEEK_SET);
    fread(&(tb->number_of_columns), sizeof(int), 1, pFile);
    printf("Number of cols: %d\n", tb->number_of_columns);

    // get number_of_entries 
    fseek(pFile, MAX_TABLE_NAME_LENGTH + sizeof(tb->number_of_columns), SEEK_SET);
    fread(&(tb->number_of_entries), sizeof(int), 1, pFile);
    printf("Number of entries: %d\n", tb->number_of_entries);

    // get length 
    fseek(pFile, 0, SEEK_END);
    printf("Length of file: %d\n", ftell(pFile));

    // last entry pointer
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*2), SEEK_SET);
    int lep;
    fread(&lep, sizeof(lep), 1, pFile);
    printf("Last index location: %d\n", lep);
    fseek(pFile, lep, SEEK_SET);
    int test;
    fread(&test, sizeof(test), 1, pFile);
    printf("Last index value: %d\n", test);

    // last entry 
    fseek(pFile, (MAX_TABLE_NAME_LENGTH + sizeof(int)*3), SEEK_SET);
    int le;
    fread(&le, sizeof(le), 1, pFile);
    printf("Last entry: %d\n", le);
    fseek(pFile, le, SEEK_SET);
    char *last_entry_value = malloc(sizeof(char)* 4);
    fread(last_entry_value, strlen(last_entry_value), 1, pFile);
    printf("Read last entry: %s\n", last_entry_value - sizeof(char)*4);

    
}

