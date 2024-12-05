#include <stdio.h>
#include <stdlib.h>


// increments the number_of_entries in the file 
// returns -1 for error; returns 0 if all is good 
int increment_number_of_entries(FILE *pFile, int location_of_num_of_entries){
    if (!pFile){
        printf("Error: File is unwell.\n");
        return -1;
    }
    int * number_of_entries = malloc(sizeof(int));
    fseek(pFile, location_of_num_of_entries, SEEK_SET);
    fread(number_of_entries, sizeof(*number_of_entries), 1, pFile);
    printf("Read num of entries: %d\n", *number_of_entries);
    fseek(pFile, location_of_num_of_entries, SEEK_SET);
    fwrite(number_of_entries, sizeof(*number_of_entries), 1, pFile);
    free(number_of_entries);
    return 0; 
}


// calculates the location of the last entry 
// returns int location of last entry 
int get_location_of_last_entry(FILE *pFile, int location_of_last_entry_pointer, int *bufferPLastEntry){
    fseek(pFile, location_of_last_entry_pointer, SEEK_SET);
    fread(bufferPLastEntry, sizeof(*bufferPLastEntry), 1, pFile);
    printf("Last entry location: %d\n", *bufferPLastEntry);
    return *bufferPLastEntry; 
}


// updates last entry pointer 
// returns -1 for errors; returns 0 if all is good 
int update_last_entry_pointer(FILE *pFile, int location_of_last_entry_pointer, int entry_size){
    if (!pFile){
        printf("Error: File is unwell\n");
        return -1;
    }
    // read pLastEntry 
    int *pLastEntry = malloc(sizeof(int));
    *pLastEntry = get_location_of_last_entry(pFile, location_of_last_entry_pointer, pLastEntry);
    (*pLastEntry) -= entry_size; 
    printf("new entry location: %d\n", *pLastEntry);
    fseek(pFile, location_of_last_entry_pointer, SEEK_SET);
    fwrite(pLastEntry, sizeof(*pLastEntry), 1, pFile);

    free(location_of_lep);
    free(pLastEntry);
    return 0; 
}


// updates last index 
// returns -1 for errors; returns 0 if all is good 
int update_last_index(FILE *pFile, int location_of_last_index_pointer, int location_of_last_entry_pointer){
    if (!pFile){
        printf("Error: File is unwell.\n");
        return -1; 
    }

    // read pointer to the last index
    int *pLastIndex = malloc(sizeof(int));
    fseek(pFile, location_of_last_index_pointer, SEEK_SET);
    fread(pLastIndex, sizeof(*pLastIndex), 1, pFile);
    printf("Last index location: %d\n", *pLastIndex);

    // read last_index value
    fseek(pFile, *pLastIndex, SEEK_SET);
    int *last_index = malloc(sizeof(int));
    fread(last_index, sizeof(*last_index), 1, pFile);
    printf("Last index value: %d\n", *last_index);
    (*last_index) ++;
    printf("New last index value: %d\n", *last_index);

    // write last_index to file 
    fseek(pFile, *pLastIndex + sizeof(int)*2, SEEK_SET);
    fwrite(last_index, sizeof(*last_index), 1, pFile);

    // read pointer to last entry 
    int *pEntry = malloc(sizeof(int));
    *pEntry = get_location_of_last_entry(pFile, location_of_last_entry_pointer, pEntry);

    // write pointer to entry (after index) 
    fseek(pFile, *pLastIndex + sizeof(int), SEEK_SET);
    fwrite(pEntry, sizeof(*pEntry), 1, pFile);

    // update pointer to last_index 
    fseek(pFile, location_of_last_index_pointer, SEEK_SET);
    *pLastIndex += sizeof(int)*2; 
    printf("New last index location: %d\n", *pLastIndex);
    fwrite(pLastIndex, sizeof(*pLastIndex), 1, pFile);

    free(pLastIndex);
    free(last_index);
    free(pEntry);

    return 0; 
}


// writes entry char** to file 
// returns 0 if all is good 
int add_entry_as_char (FILE *pFile, int location_of_last_entry_pointer, char **value, int value_size){
    int *pLastEntry = malloc(sizeof(int));
    *pLastEntry = get_location_of_last_entry(pFile, location_of_last_entry_pointer, pLastEntry);
    fseek(pFile, *pLastEntry - value_size); 
    printf("Writing to file: %s\n", *value);
    fwrite(value, value_size, 1, pFile);
    return 0; 
}



