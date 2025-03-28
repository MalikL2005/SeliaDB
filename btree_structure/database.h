

#include "types.h"



table_t * create_table (char * tb_name, int num_of_columns, column_t * columns);
database_t * create_database (char * name, int num_of_tables, table_t * tables);
column_t * create_column (char * name, char * type, int varchar_size);
void display_database(database_t * db);

