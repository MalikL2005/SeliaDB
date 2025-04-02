/*
 *
 *
 * search.c
 * This file contains all functionality for searching by key/value 
 * (Value still needs to be implemented though)
 *
 *
 *
 * */



#include "search.h"
#include "types.h"


struct entry_t search_by_key(int key, node * current, int * search_iterations){
    (*search_iterations) ++;
	int i;
	for (i=0; i<MAX_KEYS && current->entries[i].key != 0; i++){
		if (current->entries[i].key == key){
			printf("%d has been found at node: %d %d %d\n", key, current->entries[0].key, current->entries[1].key, current->entries[2].key);
			return current->entries[i]; 
		}
		else if (key < current->entries[i].key){
			// leaf node
			if (current->children[0] == NULL) {
				printf("Key %d has not been found.\n", key);
                return (entry_t) {0,0};
			}
			// current node has children
			return search_by_key(key, current->children[i], search_iterations);
		}
	}
	// leaf node 
	if (current->children[0] == NULL) {
		printf("Key %d has not been found.\n", key);
        return (entry_t) {0,0};
	}
	// current node has children
	if (key > current->entries[i].key) return search_by_key(key, current->children[i], search_iterations);
	printf("Key %d has not been found.\n", key);
    return (entry_t) {0,0};
}



/*
* Compares two values of any type_t
* Returns 1 if val1 is greater
* Returns 0 if values are equal
* Returns -1 if val1 is smaller
* (similar to strcmp)
* Other return values indicate errors
*/
int compare_values(type_t type, void * val1, void * val2){
    switch (type){
        case INTEGER:
            if (*(int*)val1 > *(int*)val2){
                return 1;
            }
            else if (*(int*)val1 < *(int*)val2){
                return -1;
            } else if (*(int*)val1 == *(int*)val2){
                return 0;
            }         
        case FLOAT:
            if (*(float*)val1 > *(float*)val2){
                return 1;
            }
            else if (*(float*)val1 < *(float*)val2){
                return -1;
            } else if (*(float*)val1 == *(float*)val2){
                return 0;
            }
        case VARCHAR:
            return strcmp((char*)val1, (char*)val2);
        case BOOL:
            if ((bool) val1 == (bool) val2){
                return 0;
            }
            return 1;
        case NONE:
            if (val1 == NULL && val2 == NULL){
                return 0;
            }
            return 1;
        default:
            return 1000;
    }
}



/*
 *
 * Depth first search algorithm to (not efficiently) find an entry_t by value
 *
 * 
*/
entry_t search_by_value(node * current, void * value, type_t tp, column_t * col){
    if (current == NULL){
        return (entry_t) {0,0};
    }
    for (int i=0; i<MAX_KEYS || current->entries[i].values[i] != NULL; i++){
        if (current->entries[i].values[i] == value){
            return current->entries[i];
        }
    }
    // traverse children
    for (int i=0; i<MAX_CHILDREN || current->children[i] != NULL; i++){
        /*return search_by_value((int*)value, current->children[i], col);*/
    }
    return (entry_t) {0,0};
}


