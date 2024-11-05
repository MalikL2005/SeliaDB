#define MAX_TABLE_LENGTH 32
#define MAX_COLUMN_NAME_LENGTH 32
#define MAX_INDEX_NAME_LENGTH 32
#define MAX_COLUMNS 100
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    table_row *rows[];
} table; 

void add_row (table *tb, int number_of_columns, char *values[]){
    table_row *new_row = malloc(sizeof(table_row));
    // append *new_row to table->rows
}

void show_columns(table *table){
    printf("\n%s\n", table->name);
    for (int i=0; i<(table->number_of_columns); i++){
        printf("|\n|---- %s\n", table->columns[i]);
    }
}

table *create_table(char *table_name, int number_of_columns, char *column_names[]){
    // Create new table 
    table *new_table = (table *) malloc(sizeof(table)); 
    strcpy(new_table->name, table_name);
    new_table->number_of_columns = number_of_columns;
    
    for (int i=0; i<number_of_columns; i++){
        
        // Create new column 
        column *new_col = (column *) malloc(sizeof(column));
        strcpy(new_col->name, column_names[i]);
        
        // Add new_col to new_table.columns
        new_table->columns[i] = new_col;
    }
    printf("%s\n", (new_table->columns[0]->name));
    return (table *) new_table;
}



int main(){
    char *col_names[] = {"col1", "col2", "col3"};
    table *nt = create_table("DB_NAME", 3, col_names);
    printf("%s\n", nt->name);
    printf("%s\n", (nt->columns[0]->name));
    
    // Set type of column 
    strcpy(nt->columns[0]->type, "INT");
    printf("%s\n", (nt->columns[0]->type));
    
    // Add new row to column 
    char v1[] = "v1";
    char v2[] = "v2";
    char v3[] = "v3";
    //char ** values = {&v1, &v2, &v3};
    char *values[] = {v1, v2, v3};
    add_row(nt, nt->number_of_columns, values);
    show_columns(nt);
    
    return 0;
}


