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

int handle_command(InputBuffer* buffer){
    if (compare(buffer->buffer, "SHOWCOLUMNS")){
        printf("Showing columns...\n");
        //show_columns(buffer);
    } else if (compare(buffer->buffer, "EXIT")){
        printf("Exiting... \n");
        return 1;
    } else if (compare(buffer->buffer, "")){
        return 0;
    } else {
        printf("Unknown command.\n");
    }
    return 0;
}


int main() {
    InputBuffer* pBuffer = newInputBuffer();
    int end;
    
    do {
        print_prompt();
        read_input(pBuffer);
        end = handle_command(pBuffer);
    } while(end != 1);
    
    return 0;
}
