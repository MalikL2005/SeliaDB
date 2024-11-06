#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    char* buffer; 
    size_t buffer_length; 
    ssize_t input_length; 
} InputBuffer;


void print_prompt(){
    printf("db> ");
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


int handle_command(InputBuffer* buffer, database *current_db, database *DBS[]){
    if (compare(buffer->buffer, "SHOWCOLUMNS")){
        printf("Showing columns...\n");
        printf("%s\n", current_db->name);
        //show_columns(buffer);
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
            current_db = DBS[index];
        } else {
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
            if (!DBS[i]){
                current_db = create_db(dbname);
                DBS[i] = current_db;
                printf("DB %s created.\n", dbname);
                return 0;
            }
            if (compare(dbname, DBS[i]->name)){
                printf("DB already exists\n");
                continue; 
            }
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
    database *current_db;
    do {
        print_prompt();
        read_input(pBuffer);
        end = handle_command(pBuffer, current_db, DBS);
    } while(end != 1);
    
    return 0;
}
