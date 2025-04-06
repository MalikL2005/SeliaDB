

#include "types.h"



int create_table (table_t * tb, char * tb_name, int num_of_columns, ...);
int create_database (database_t * db, table_t ** pTb, char * name, int num_of_tables, ...);
int create_column (column_t * col, char * name, char * type, int varchar_size);
void display_database(database_t * db);
const char * get_type_as_string (type_t tp);
int create_entry (entry_t * new_entry, buffer_t * buffer, table_metadata_t * tb, int num_of_columns, ...);
int add_table (database_t * db, table_t * new_tb);
int add_column (table_t * tb, column_t * new_col);
void free_table (table_t * tb);
void free_database (database_t * db);
void free_buffer(buffer_t * bf);
void free_entry(entry_t * entry, table_t * tb);

