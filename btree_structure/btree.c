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

    // Create columns
    column_t * col1 = malloc(sizeof(column_t));
    column_t * col2 = malloc(sizeof(column_t));
    column_t * col3 = malloc(sizeof(column_t));
    if (create_column(col1, "Column1", "INTEGER", 0) != 0){
        printf("Error: could not create column\n");
        return -1;
    }
    if (create_column(col2, "Column2", "FLOAT", 0) != 0){
        printf("Error: could not create column\n");
        return -1;
    }
    if (create_column(col3, "Column3", "VARCHAR", 255) != 0){
        printf("Error: could not create column\n");
        return -1;
    }
    table_t * tb1 = malloc(sizeof(table_t));
    table_t * tb2 = malloc(sizeof(table_t));
    if (create_table(tb1, "Table1", 1, col1) != 0){
        printf("Error: could not create table\n");
        return -1;
    }
    if (create_table(tb2, "Table2", 1, col1) != 0){
        printf("Error: could not create table\n");
        return -1;
    }
    /*add_column(tb2, col1);*/
    database_t * db1 = malloc(sizeof(database_t));
    table_t ** pTb;
    int a = create_database(db1, pTb, "MY_DB", 2, tb1, tb2);
    if (argc < 2){
        printf("Usage: btree [num_to_insert_to]\nDefaulting to 100");
        argv[1] = "100";
    }


    int num_to_insert_to = atoi(argv[1]);


    table_t * tb = tb1;
    printf("argv[0]: %s\n", argv[1]); // Todo: Add error handling (behaviour is undefined if argv[1] is not numerical)
	for (int i=1; i<=num_to_insert_to; i++){
        buffer_t bf = (buffer_t) {};
    /*buffer_t bf = (buffer_t){malloc(sizeof(float)*tb->metadata->num_of_columns), 0, malloc(sizeof(int)*tb->metadata->num_of_columns), 0, malloc(sizeof(char *)*tb->metadata->num_of_columns), 0};*/
        entry_t * buffer_entry = malloc(sizeof(entry_t));
        entry_t * entr = create_entry(buffer_entry, bf, tb->metadata, tb->metadata->num_of_columns, i);
        printf("main got %p\n", entr);
        printf("got %d\n", *(int *) entr->values[0]);
		insert(entr, tb->root, tb);
	}

    printf("Now traversing\n");
    printf("Hello World\n");
    printf("Reached");
	traverse(tb->root, tb);
    if (tb == NULL) return 1;
    
    // Test for searching by key
    int key = 500;
    int * iterations;
    entry_t test = search_by_key(key, tb->root, iterations);
    if (test.key <= 0){
        printf("Key %d not found anywhere.\n", key);
        printf("Search iterations: %d\n", *iterations);
    } else {
        printf("Test findings: %d - %d\n", test.key, test.value);
    }
    free(iterations);

    free(col1);
    free(col2);
    free(col3);

    free(tb1);
    free(tb2);

    /*display_database(db1);*/
    /*entry_t * etr = create_entry(db1->tables[1]->metadata, db1->tables[1]->metadata->num_of_columns, 4.5, "HelloThere");*/
    /*entry_t * etr2 = create_entry(db1->tables[0]->metadata, db1->tables[0]->metadata->num_of_columns, 5);*/
    /**/
    /*int status = delete(3, &(tb->root));*/
    /*printf("Received status %d\n", status);*/
    /*printf("Now traversing again\n");*/
    /*if (tb->root == NULL) return 1;*/
    /*traverse(tb->root);*/

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
            printf("-> %d", *(int *)current->entries[j].values[2]);
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


