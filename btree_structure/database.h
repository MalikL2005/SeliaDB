

#include "types.h"



table_t * create_table (char * tb_name, int num_of_columns, ...);
database_t * create_database (database_t * db, table_t ** pTb, char * name, int num_of_tables, ...);
column_t * create_column (char * name, char * type, int varchar_size);
void display_database(database_t * db);
const char * get_type_as_string (type_t tp);
entry_t * create_entry (table_metadata_t * tb, int num_of_columns, ...);
int add_table (database_t * db, table_t * new_tb);
int add_column (table_t * tb, column_t * new_col);

