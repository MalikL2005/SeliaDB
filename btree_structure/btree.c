/*
* btree.c
* This file is ised to create and manage a btree.
* Btrees are used to store indexes in the database.
* This file includes the functions that are needed to traverse the 
*
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

//
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

}
