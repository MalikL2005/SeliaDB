#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "methods.c"
// Test comment
// Test 2

typedef struct {
    char* buffer; 
    size_t buffer_length; 
    ssize_t input_length; 
} InputBuffer;


void print_prompt(database **pCurrent_db){
    if ((*pCurrent_db)->name){
        printf("%s> ", (*pCurrent_db)->name);
    } 
    else {
        printf("db> ");
    }
}


InputBuffer* newInputBuffer(){
    InputBuffer* buffer = malloc(sizeof(InputBuffer));
    buffer->buffer = NULL;
    buffer->buffer_length = 0; 
    buffer->input_length = 0; 
    
    return buffer;
}


void read_input(InputBuffer* buffer_in){
    ssize_t bytes_read = getline(&(buffer_in->buffer), &(buffer_in->buffer_length), stdin);
    if (bytes_read <= 0){
        printf("Could not read binaries properly. ");
        exit(1);
    }
    buffer_in->input_length = bytes_read -1;
    buffer_in->buffer[bytes_read -1] = 0; 
}


void close_buffer_in(InputBuffer* buffer_in){
    free(buffer_in->buffer);
    free(buffer_in);
}


int compare(char s1[], char s2[]){
    return strcmp (s1, s2) == 0;
}


int handle_command(InputBuffer* buffer, database **pCurrent_db, database *DBS[]){
    if (compare(buffer->buffer, "SHOWCOLUMNS")){
        if (pCurrent_db){
            printf("|%s\n|\n", (*pCurrent_db)->name);
            for (int i=0; i<MAX_TABLES; i++){
                if ((*pCurrent_db)->tables[i]->name){
                    printf("|---- %s\n|\n", (*pCurrent_db)->tables[i]->name);
                } 
            }
        } else {
            printf("No Database selected (use \"USEDATABASE\", or create a new one.)\n");
        }
           // show_columns(buffer);
    }
    else if (compare(buffer->buffer, "EXIT")){
        printf("Exiting... \n");
        return 1;
    } 
    else if (compare(buffer->buffer, "USEDATABASE")){
        int index; //not memory efficient
        printf("Give db index\n");
        scanf("%d", &index); // Todo: validate input
        if (index < 5 && index >= 0 && DBS[index]){
            *pCurrent_db = DBS[index];
            printf("Use_db: %s\n", (*pCurrent_db)->name);
        } 
        else {
            printf("This index is not valid.\n");
        }
    } 
    else if (compare(buffer->buffer, "CREATEDATABASE")){ // Todo: add regex to check for "COMMAND <space> NAME"
        // Read DB name
        char dbname[MAX_DB_NAME_LENGTH]; //not memory efficient
        printf("Give db name: ");
        scanf("%s", dbname); // Todo: validate input 
        
        // Check if DB already exists
        for (int i=0; i<MAX_DATABASES; i++){
            if (DBS[i]){
                continue;
            }
            *pCurrent_db = create_db(dbname);
            if (!*pCurrent_db){
                printf("Could not create Database.\n");
                break; 
            } else {
                DBS[i] = *pCurrent_db;
                printf("DB %s created.\n", dbname); 
                printf("current db: %s\n", (*pCurrent_db)->name);
                return 0;
            }
        }
    }
    else if (compare(buffer->buffer, "CREATETABLE")){
        if (*pCurrent_db){
            printf("\nCreating table in %s\n", (*pCurrent_db)->name);
            char *new_table_name = malloc(MAX_TABLE_NAME_LENGTH);
            int *number_of_columns = malloc(sizeof(int));
            printf("Name for new table: ");
            scanf("%s", new_table_name);
            printf("Assigned name, %s\n", new_table_name); // Make sure that name does not exist already 
            printf("Number of columns: ");
            scanf("%d", number_of_columns);
            printf("%d many columns.\n", *number_of_columns);
            if (*number_of_columns > MAX_COLUMNS || *number_of_columns <= 0){
                printf("Error: Invalid column number. Could not create table.\n");
                return 0; 
            }
            // read column names & types 
            char * column_names[*number_of_columns];

            for (int i=0; i<*number_of_columns; i++){
                column_names[i] = malloc(MAX_COLUMN_NAME_LENGTH);
                printf("%d) Column name: ", i+1);
                scanf("%s", column_names[i]);
                printf("result: %s\n", column_names[i]);
            }


            table *new_table = create_table(pCurrent_db, new_table_name, number_of_columns, column_names);
            printf("Currents new table: %s\n", (*pCurrent_db)->tables[0]->name);
        } 
        else {
            printf("No Database selected (use \"USEDATABASE\", or create a new one.)\n");
        }
    }
    else if (compare(buffer->buffer, "SHOWDATABASES")){
        printf("| Your DBs\n|-----------\n");
        for (int i=0; i<MAX_DATABASES; i++){
            if (DBS[i]){
                printf("|---- %s\n", DBS[i]->name);
            } else {
                printf("|---- Empty slot\n");
            }
        }
    }
    else if (compare(buffer->buffer, "READDBS")){
        if (read_databases(DBS) == 0){
            printf("Read databases successfully.\n");
        }else{
            printf("Could not read databases.\n");
        }
    }
    else if (compare(buffer->buffer, "READFILE")){
        printf("Reading file...\n");
        create_table_from_file("test.bin");
    }
    else if (compare(buffer->buffer, "ADDROW")){
        int *index = malloc(sizeof(int));
        printf("Table index: ");
        scanf("%d", index);
        if (*pCurrent_db){
            for (int i=0; i<10; i++){
                if ((*pCurrent_db)->tables[i]){
                    printf("\n%s, index %d\n", (*pCurrent_db)->tables[i]->name, i);
                    char ** values = malloc((*pCurrent_db)->tables[i]->number_of_columns * sizeof(char*));
                    for (int j=0; j<(*pCurrent_db)->tables[i]->number_of_columns; j++){
                        values[j] = "Value";
                    }
                add_row((*pCurrent_db)->tables[i], values);
                break;
                }
            }
        } 
        else {
            printf("No Database or Table selected (use \"USEDATABASE\" and \"CREATETABLE\")\n");
        }
    }
    else if (compare(buffer->buffer, "")){
        return 0;
    } 
    else {
        printf("Unknown command.\n");
    }
    return 0;
}



int main() {
    database *DBS [MAX_DATABASES];
    InputBuffer* pBuffer = newInputBuffer();
    int end;
    database *current_db = DBS[0];
    check_for_init_folder();

    do {
        print_prompt(&current_db);
        read_input(pBuffer);
        end = handle_command(pBuffer, &current_db, DBS);
    } while(end != 1);
    
    return 0;
}
