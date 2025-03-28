/*
*
*
*
* btree.c
* This file is used to create and manage the database's btrees.
* Btrees are used to store indexes in the database.
* This file includes the functions that are needed to:
*	- create a new btree
*	- traverse a tree
*	- find a value
*
*
*
*/



#include "btree.h"
#include "insert.h"
#include "delete.h"
#include "search.h"
#include "types.h"


int main(int argc, char **argv){
    column_t * col = create_column("Column", "INTEGER", 0);
    table_t * tb = create_table("Table1", 1, col);
    database_t * db1 = create_database("MY_DB", 1, tb);
    db1->root = malloc(sizeof(node));
    if (argc < 2){
        printf("Usage: btree [num_to_insert_to]\nDefaulting to 100");
        argv[1] = "100";
    }
    int num_to_insert_to = atoi(argv[1]);
    // Add error handling (behaviour is undefined if argv[1] is not numerical)
    printf("argv[0]: %s\n", argv[1]);
	for (int i=1; i<=num_to_insert_to; i++){
        entry_t entr = {.key=i, .value = i*2};
		insert(entr, db1->root, &db1->root);
	}
    printf("Now traversing\n");
    if (db1->root == NULL) return 1;
	traverse(db1->root);
    
    printf("Hola????\n\n");
    // Test for searching by key
    int key = 500;
    int * iterations = malloc(sizeof(int));
    entry_t test = search_by_key(key, db1->root, iterations);
    if (test.key <= 0){
        printf("Key %d not found anywhere.\n", key);
        printf("Search iterations: %d\n", *iterations);
        return 1;
    }
    printf("Test findings: %d - %d\n", test.key, test.value);
    free(iterations);


    printf("Display db!\n");
    display_database(db1);
    printf("Hello World...\n");
    free(db1);
}





/*
* This function traverses and prints the tree (root, left, right).
* Root must not be NULL.
*/
void traverse(node *current){
	if (current == NULL) return;
	for (int j=0; j<MAX_KEYS; j++) printf("%d) %d (%d)\n", j+1, current->entries[j].key, current->entries[j].value);
	printf("\n");
	for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
        printf("Traversing child no %d\n", i);
		traverse(current->children[i]);
	}
}



/*
* This function traverses the tree looking for value in each node passed.
* If the value was found, a pointer to the node where the value is in, is returned.
* if the value was not found, NULL is returned.
*/
node * findValue(int id, node * current){
	// iterate over values in current
	int i;
	for (i=0; i<MAX_KEYS && current->entries[i].key != 0; i++){
		if (current->entries[i].key == id){
			printf("%d has been found at node: %d %d %d\n", id, current->entries[0].key, current->entries[1].key, current->entries[2].key);
			return current; 
		}
		else if (id < current->entries[i].key){
			// leaf node
			if (current->children[0] == NULL) {
				printf("Value %d has not been found.\n", id);
				return NULL;
			}
			// current node has children
			return findValue(id, current->children[i]);
		}
	}
	// leaf node 
	if (current->children[0] == NULL) {
		printf("Value %d has not been found.\n", id);
		return NULL;
	}
	// current node has children
	if (id > current->entries[i].key) return findValue(id, current->children[i]);
	printf("Value %d has not been found.\n", id);
	return NULL;
}



/*
 * Returns the index of a value in a node.
 * Optimization can easily be performed (e.g. using bianry search).
 */
int getNodeIndex(node * current, int value){
	int index;
	for (index=0; index<MAX_KEYS && current->entries[index].key != value; index ++){}
	return index;
}



/*
 * Returns the index of the greatest value in a node.
 * This can also be interpreted as the length of the node's key-arr.
 */
int getIndexGreatestValue(node * current){
	int len;
	for (len = 0; len<MAX_KEYS && current->entries[len].key != 0; len ++){}
	len --;
	return len;
}


