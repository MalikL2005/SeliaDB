#include "btree.h"



struct entry_t search_by_key(int key, node * current, int * search_iterations);
entry_t search_by_value(node * current, void * value, type_t tp, column_t * col);




