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
#include "btree.h"


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
 *
 * Depth first search algorithm to (not efficiently) find an entry_t by value
 *
 * 
*/
entry_t search_by_value(int value, node * current){
    if (current == NULL){
        return (entry_t) {0,0};
    }
    for (int i=0; i<MAX_KEYS || current->entries[i].key <= 0; i++){
        if (current->entries[i].value == value){
            return current->entries[i];
        }
    }
    return (entry_t) {0,0};
}


