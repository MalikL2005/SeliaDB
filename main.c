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


int main() {
    InputBuffer* pBuffer = newInputBuffer();
    
    while (1){
        print_prompt();
        read_input(pBuffer);
        if (strcmp(pBuffer->buffer, "exit") == 0){
            printf("leaving");
            close_buffer_in(pBuffer);
            break;
        } else {
            printf("unrecognized command.\n");
        }
    }
    
    return 0;
}
