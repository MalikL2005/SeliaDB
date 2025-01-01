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



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "insert.c"
#define MAX_BTREE_NAME_LENGTH 32

void traverse(struct node *current);
bool findValue(int value, struct node * current);


struct btree {
	char * name;
	struct node * root;
};




void main(){
	struct btree * mybtree = malloc(sizeof(struct btree));
	mybtree->name ="Edos_whoo";
	for (int i=1; i<100; i++){
		insert(i, root);
	}
	traverse(root);
	findValue(5, root);
	findValue(99, root);
	findValue(199, root);
}





// This function traverses and prints the tree (root, left, right).
void traverse(struct node *current){
	if (root == NULL) return;
	if (current == NULL) return;
	for (int j=0; j<MAX_KEYS; j++) printf("%d) %d\n", j+1, current->keys[j]);
	printf("\n");
	for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
		traverse(current->children[i]);
	}
}



bool findValue(int value, struct node * current){
	// iterate over values in current
	int i;
	for (i=0; i<MAX_KEYS && current->keys[i] != 0; i++){
		if (current->keys[i] == value){
			printf("%d has been found at node: %d %d %d\n", value, current->keys[0], current->keys[1], current->keys[2]);
			return true; 
			/*
			* for future functionality, it may be better to change the return type from bool to node *.
			* To check if the value exists, after calling findValue(in main-function or wherever), 
			* check if findValue() == NULL (-> value is not found).
			*/
		}
		else if (value < current->keys[i]){
			// leaf node
			if (current->children[0] == NULL) {
				printf("Value %d has not been found.\n", value);
				return false;
			}
			// current node has children
			return findValue(value, current->children[i]);
		}
	}
	// leaf node 
	if (current->children[0] == NULL) {
		printf("Value %d has not been found.\n", value);
		return false;
	}
	// current node has children
	if (value > current->keys[i]) return findValue(value, current->children[i]);
	printf("Value %d has not been found.\n", value);
	return false;
}
