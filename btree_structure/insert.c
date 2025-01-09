/* 
 *
 *
 *
 * Insert.c 
 * This file includes all definitions and functions that are needed for insertion in the btree-structrue.
 *
 *
 *
*/




#include "insert.h"

/* 
 * Void function that is called first when a value is inserted.
 * It traverses the tree until the right leaf (where the value is to be inserted) is found.
 * Upon finding the right leaf node, the function calls either insertToNode (if node is not full) or splitNode (if node is full).
*/
void insert(int value, node *current){
	// 1. If the tree is empty, allocate a root node and insert the key.
	if (root == NULL){
		printf("Root is null\n");
		node * new_root = malloc(sizeof(node));
		new_root->keys[0] = value;
		root = new_root;
		return;
	}

	// node is leaf and is not full
	if (current->children[0] == NULL && current->keys[MAX_KEYS - 1] == 0){
		printf("Leaf node and not full\n");
		insertToNode(value, current);
		return;
	}
	// node is leaf and is full
	if (current->children[0] == NULL && current->keys[MAX_KEYS - 1] != 0){
		splitNode(value, current);
		return;
	}

	// node is not leaf
	if (current->children[0]){
		// traverse tree until right leaf node is found
		int num_of_children = 0;
		bool is_inserted = false;
		for (int i=0; i<MAX_CHILDREN && current->keys[i] != 0; i++){
			printf("value: %d; current->keys: %d\n", value, current->keys[i]);
			if (value <= current->keys[i]){
				insert(value, current->children[i]);
				is_inserted = true;
				break;
			}
			num_of_children ++;
		}
		printf("num of children %d\n", num_of_children);
		if (!is_inserted && value > current->keys[num_of_children]) insert(value, current->children[num_of_children]);
	}
}



/* 
 * This function creates and returns an array of ints.
 * It is called when a node has reached MAX_KEYS.
 * Therefore, it creates an array of size MAX_KEYS + 1 (because the node (current) overflows.
*/
int * createTempArr(int value, node *current){
	// create temporary overflowing array with all values
	int * temp_arr = malloc(sizeof(int) * MAX_KEYS +1);

	// copy current node´s key array unitl value
	int i_original;
	int i_copy;
	for (i_original=0; i_original<MAX_KEYS; i_original++){
		if (value <= current->keys[i_original]) break;
		*(temp_arr + i_original) = current->keys[i_original];
	}

	// insert value to temporary array
	i_copy = i_original;
	*(temp_arr + i_copy) = value;
	i_copy ++;

	// copy rest
	for (int j=i_original; j<MAX_KEYS; j++){
		*(temp_arr + i_copy) = current->keys[j];
		i_copy ++;
	}

	// print temp array
	printf("New array: ");
	for (int i=0; i<MAX_KEYS + 1; i++){ printf("%d ", *(temp_arr + i)); }
	printf("\n");
	return temp_arr;
}



/*
 * Void function that splits full nodes, starting with a full leaf node.
 * After splitting the full leaf node, the middle value (upon which the split happened) is inserted to the leaf node's parent
 * If the parent itself is full, the method is called recursively on the parent of each respectable node until A or B happens:
 * A) a non-full node is found: the middle value that is passed from the child's split will be inserted.
 * B) the function's recursive call reaches the root (which is full): then a new root is allocated and the previous root is split.
*/
void splitNode(int value, node *current){
	printf("value: %d\n", value);
	int * temp_arr = createTempArr(value, current);
	printf("SplitNode\n");
	printf("%d %d %d (%X)\n", current->keys[0], current->keys[1], current->keys[2], current);
	// current->keys is full
	if (current->keys[MAX_KEYS - 1] != 0){
		// calculate middle value
		int middle_value = *(temp_arr + (MAX_KEYS/2 + 1));
		printf("SN Middle value: %d\n", middle_value);

		// create two seperate nodes
		node *new_left = malloc(sizeof(node));
		node *new_right = malloc(sizeof(node));

		// copy values to new nodes
		int k = 0;
		for (int i=0; i<MAX_KEYS+1; i++){
			printf("temp arr: %d\n", *(temp_arr + i));
			if (*(temp_arr + i) < middle_value){
				new_left->keys[i] = *(temp_arr + i);
				printf("Left (%d): %d\n", i, new_left->keys[i]);
			}
			else if (*(temp_arr + i) > middle_value) {
				new_right->keys[k] = *(temp_arr + i);
				printf("Right (%d): %d\n", k, new_right->keys[k]);
			}
		}


		if (current == root){
			// create new root with children
			printf("Splitting root node\n");
			printf("root: %X %X %X %X\n", root->children[0], root->children[1], root->children[2], root->children[3]);
			node * new_root = malloc(sizeof(node));

			// adjust children of children of new root pointers
			int middle = MAX_CHILDREN / 2;
			int right = 0;
			for (int i=0; i<MAX_CHILDREN + 1; i++){
				if (i <= middle){
					new_left->children[i] = root->children[i];
				} else {
					new_right->children[right] = root->children[i];
					right ++;
				}
			}


			new_root->keys[0] = middle_value;
			new_root->children[0] = new_left;
			new_root->children[1] = new_right;
			root = new_root;

		} else {
			// adjust children of parent node
			printf("Finding parent\n");
			printf("%d %d %d (%X)\n", current->keys[0], current->keys[1], current->keys[2], current);
			node * parent = findParent(current, root);

			// parent->children is not full
			// find child_index of current in parent 
			int child_index;
			for (child_index=0; child_index<MAX_CHILDREN && parent->children[child_index] != NULL; child_index++){
				printf("child index: %d\n", child_index);
				if (parent->children[child_index] == current) break;
			}
			printf("Parent child index: %d\n", child_index);

			// Adjust new_left's and new_right's children pointer
			int middle = MAX_CHILDREN / 2;
			int right = 0;
			for (int i=0; i<MAX_CHILDREN + 1; i++){
				if (i <= middle){
					new_left->children[i] = current->children[i];
				} else {
					new_right->children[right] = current->children[i];
					right ++;
				}
			}


			// insert pointer to new_left and new_right
			if (child_index < MAX_CHILDREN){
				// shift children-array to right 
				for (int i=MAX_CHILDREN; i>child_index; i--){
					parent->children[i] = parent->children[i-1];
				}
				// adjust parent to children pointer
				parent->children[child_index] = new_left;
				parent->children[child_index + 1] = new_right;
				printf("SplitNode parent\n");
				// recursive call to split the parent or to insert middle_value if it's not full
				splitNode(middle_value, parent);
			}
		}
	}
	// current node is not full
	else if (current->keys[MAX_KEYS - 1] == 0){
		insertToNode(value, current);
	}
}



/*
 * Void function that is used to insert a value to a non-full node.
 * This node may be the root, a leaf or an internal node.
*/
void insertToNode(int value, node *current){
	int placeToInsert;
	for (placeToInsert=0; placeToInsert<MAX_KEYS; placeToInsert++){
		if(value <= current->keys[placeToInsert] || current->keys[placeToInsert] == 0){
			break;
		}
	}
	printf("Inserting at index %d\n", placeToInsert);
	// shift array to right
	for (int j=MAX_KEYS; j>placeToInsert; j--){
		current->keys[j] = current->keys[j-1];
	}
	// insert value
	current->keys[placeToInsert] = value;
}



/* 
 * This function finds the parent of a target node.
 * It traverses the tree until one of the nodes' children match the target node.
 * Upon finding the parent of target, the parent node is returned.
*/
node * findParent(node * target, node * current){
	// print address and children of current
	printf("Searching for %d\n", target->keys[0]);
	printf("Current: %d %d %d\n", current->keys[0], current->keys[1], current->keys[2]);
	if (current->children[0] != NULL){
		for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
			printf("%d %d %d\n", current->children[i]->keys[0], current->children[i]->keys[1], current->children[i]->keys[2]);
		}
	}
	// iterate over all children; if a child equals target parent node is found
	for (int i=0; i<MAX_CHILDREN; i++){
		if (current->children[i] == target){
			printf("Found parent\n");
			for (int j=0; j<MAX_KEYS; j++){
				printf("%d) parent: %d, child: %d\n", j+1, current->keys[j], target->keys[j]);
			}
			return current;
		}
	}
	printf("Current is not a direct parent\n");
	// if current is not a direct parent of target; find next closest node value -> iterate over tree
	int index;
	for (index=0; index<MAX_KEYS; index++){
		// current is not full; searched value is greater than all in current
		if (current->keys[index] == 0 && current->keys[index - 1] < target->keys[0]){
			printf("Largest value at index %d\n", index -1);
			return findParent(target, current->children[index]);
		}
		// current is full; searched value is greater than all in current
		if (index == MAX_KEYS-1 && current->keys[index-1] < target->keys[0]){
			printf("largest value at index %d\n", index);
			return findParent(target, current->children[index + 1]);
		}
		printf("%d) %d - %d\n", index, current->keys[index], target->keys[0]);
		if (target->keys[0] <= current->keys[index] && current->children[index] != NULL){
			printf("goint to child of index %d\n", index);
			return findParent(target, current->children[index]);
		}
	}
}


