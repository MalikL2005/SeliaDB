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


void main(){
	// struct btree * mybtree = malloc(sizeof(struct btree));
	// mybtree->name ="Edos_whoo";
	btree * index = malloc(sizeof(btree));
	root = index->root;
	for (int i=1; i<=10; i++){
		insert(i, root);
	}
	traverse(root);
	delete(10);
	delete(1);
	delete(2);
	delete(3);
	traverse(root);
	delete(9);
	traverse(root);
	delete(6);
	traverse(root);
	delete(5);
	traverse(root);
	printf("Deletion call of 8\n");
	delete(8);
	traverse(root);

	btree * second = malloc(sizeof(btree));
	root = second->root;
	insert(5, root);
	traverse(root);
}





/*
* This function traverses and prints the tree (root, left, right).
*/
void traverse(node *current){
	if (root == NULL) return;
	if (current == NULL) return;
	for (int j=0; j<MAX_KEYS; j++) printf("%d) %d\n", j+1, current->keys[j]);
	printf("\n");
	for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
		traverse(current->children[i]);
	}
}



/*
* This function traverses the tree looking for value in each node passed.
* If the value was found, a pointer to the node where the value is in, is returned.
* if the value was not found, NULL is returned.
*/
node * findValue(int value, node * current){
	// iterate over values in current
	int i;
	for (i=0; i<MAX_KEYS && current->keys[i] != 0; i++){
		if (current->keys[i] == value){
			printf("%d has been found at node: %d %d %d\n", value, current->keys[0], current->keys[1], current->keys[2]);
			return current; 
		}
		else if (value < current->keys[i]){
			// leaf node
			if (current->children[0] == NULL) {
				printf("Value %d has not been found.\n", value);
				return NULL;
			}
			// current node has children
			return findValue(value, current->children[i]);
		}
	}
	// leaf node 
	if (current->children[0] == NULL) {
		printf("Value %d has not been found.\n", value);
		return NULL;
	}
	// current node has children
	if (value > current->keys[i]) return findValue(value, current->children[i]);
	printf("Value %d has not been found.\n", value);
	return NULL;
}



/*
 * Returns the index of a value in a node.
 * Optimization can easily be performed (e.g. using bianry search).
 */
int getNodeIndex(node * current, int value){
	int index;
	for (index=0; index<MAX_KEYS && current->keys[index] != value; index ++){}
	return index;
}



/*
 * Returns the index of the greatest value in a node.
 * This can also be interpreted as the length of the node's key-arr.
 */
int getIndexGreatestValue(node * current){
	int len;
	for (len = 0; len<MAX_KEYS && current->keys[len] != 0; len ++){}
	len --;
	return len;
}


