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
    column_t * col1 = create_column("Column1", "INTEGER", 0);
    column_t * col2 = create_column("Column2", "FLOAT", 0);
    column_t * col3 = create_column("Column3", "VARCHAR", 255);
    table_t * tb1 = create_table("Table1", 1, col1);
    table_t * tb2 = create_table("Table2", 2, col2, col3);
    database_t * db1 = create_database("MY_DB", 2, tb1, tb2);
    db1->root = malloc(sizeof(node));
    if (argc < 2){
        printf("Usage: btree [num_to_insert_to]\nDefaulting to 100");
        argv[1] = "100";
    }
    int num_to_insert_to = atoi(argv[1]);


    table_t * tb = tb2;
    // Todo: Add error handling (behaviour is undefined if argv[1] is not numerical)
    printf("argv[0]: %s\n", argv[1]);
	for (int i=1; i<=num_to_insert_to; i++){


        char * t = "Hello";
        if (i == 2) t = "World";
        
        entry_t * entr = create_entry(tb->metadata, tb->metadata->num_of_columns, (float) i-0.3, t);
        /*printf("main got %p\n", entr->values);*/
        /*printf("got %f\n", *(float*) entr->values[0]);*/
        /*printf("got addr %p\n", entr->values[0]);*/
        /*printf("got %s\n", (char*) entr->values[1]);*/
		insert(entr, tb->root, tb);
	}

    printf("Now traversing\n");
    if (tb->root == NULL) return 1;
	traverse(tb->root, tb);
    
    // Test for searching by key
    int key = 500;
    int * iterations = malloc(sizeof(int));
    entry_t test = search_by_key(key, tb->root, iterations);
    if (test.key <= 0){
        printf("Key %d not found anywhere.\n", key);
        printf("Search iterations: %d\n", *iterations);
    } else {
        printf("Test findings: %d - %d\n", test.key, test.value);
    }
    free(iterations);


    /*display_database(db1);*/
    /*entry_t * etr = create_entry(db1->tables[1]->metadata, db1->tables[1]->metadata->num_of_columns, 4.5, "HelloThere");*/
    /*entry_t * etr2 = create_entry(db1->tables[0]->metadata, db1->tables[0]->metadata->num_of_columns, 5);*/
    /**/
    /*int status = delete(3, &(tb->root));*/
    /*printf("Received status %d\n", status);*/
    /*printf("Now traversing again\n");*/
    /*if (tb->root == NULL) return 1;*/
    /*traverse(tb->root);*/


    free(db1);
}





/*
* This function traverses and prints the tree (root, left, right).
* Root must not be NULL.
*/
void traverse(node *current, table_t * tb){
	if (current == NULL) return;
	for (int j=0; j<MAX_KEYS; j++) {
        printf("%d) %d (%d) ", j+1, current->entries[j].key, current->entries[j].value);
        if (current->entries[j].values != NULL){
            printf("-> %f", *((float *)current->entries[j].values[0]));
            printf("-> %s", (char*)current->entries[j].values[1]);
            printf("-> %p", current->entries[j].values);
        }
        printf("\n");
    }
	printf("\n");
	for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
        printf("Traversing child no %d\n", i);
		traverse(current->children[i], tb);
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


