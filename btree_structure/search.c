
#include "search.h"


struct entry search_by_key(int key, node * current){
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
                return (entry) {0,0};
			}
			// current node has children
			return search_by_key(key, current->children[i]);
		}
	}
	// leaf node 
	if (current->children[0] == NULL) {
		printf("Key %d has not been found.\n", key);
        return (entry) {0,0};
	}
	// current node has children
	if (key > current->entries[i].key) return search_by_key(key, current->children[i]);
	printf("Key %d has not been found.\n", key);
    return (entry) {0,0};
}




