#define MAX_TABLE_LENGTH 32
#define MAX_COLUMN_NAME_LENGTH 32
#define MAX_INDEX_NAME_LENGTH 32
#define MAX_COLUMNS 100
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name [MAX_COLUMN_NAME_LENGTH];
    char type[25];
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
} table; 


table *create_table(char *table_name, int number_of_columns, char *column_names[]){
    //create new table 
    table *new_table = (table *) malloc(sizeof(table)); 
    strcpy(new_table->name, table_name);
    new_table->number_of_columns = number_of_columns;
    
    for (int i=0; i<number_of_columns; i++){
        
        // create new column 
        column *new_col = (column *) malloc(sizeof(column));
        strcpy(new_col->name, column_names[i]);
        
        // ad new_col to new_table.columns
        new_table->columns[i] = new_col;
    }
    printf("%s\n", (new_table->columns[0]->name));
    return (table *) new_table;
}

void main(){
    char *col_names[] = {"c1", "c2", "c3"};
    table *nt = create_table("Hello", 3, col_names);
    printf("%d\n", &(nt->name));
    printf("%s\n", nt->name);
    printf("%s\n", (nt->columns[0]->name));
    printf("%s\n", (nt->columns[1]->name));
    printf("%s\n", (nt->columns[2]->name));
    printf("%s", "Hi");
    
}
