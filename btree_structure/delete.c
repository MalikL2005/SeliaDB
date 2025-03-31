/* 
 *
 *
 *
 * delete.c 
 * This file includes all definitions and functions that are needed for deletion of a record in the btree-structrue.
 *
 *
 *
*/


#include "delete.h"
#include "types.h"



/*
* This function is called when a value is to be deleted (duh).
*
* returns -1 if value does not exist in tree.
* returns 0 if value has been removed successfully.
* returns 1 if an error occured.
*/
int delete(int value, node ** root){
	printf("Removing %d from tree\n", value);

	// find node
	node * current = findValue(value, *root);
	if (current == NULL){
		printf("Value %d not found.\n", value);
		return -1;
	}
	int i_value = getNodeIndex(current, value);
	printf("Current node: %d %d %d\n", current->entries[0].value, current->entries[1].value, current->entries[2].value);
	// leaf node + num_of_keys > MIN_KEYS
	if (current->children[0] == NULL && current->entries[MIN_KEYS].value != 0){
		printf("Removing value from node\n");
		removeValueFromNode(value, current, -1);
		return 0;
	}

	// leaf node + num_of_keys <= MIN_KEYS
	if (current->children[0] == NULL && current->entries[MIN_KEYS].key == 0){
		printf("Borrowing from sibling (Value: %d).\n", value);
		if (borrowValueFromSibling(value, current, root) == 0) return 0;
		int res = mergeParentAndChild(current, value, root);
		printf("res: %d\n", res);
		// res == unsuccessfull
		if (res != 0){
			node * parent = findParent(current, *root);
			// find childIndex of current in parent->children
			int childIndex;
			for (childIndex = 0; childIndex<MIN_CHILDREN && parent->children[childIndex] != NULL && parent->entries[childIndex].key <= value && parent->entries[childIndex].key != 0; childIndex++){
				printf("%d) parent: %d, child: %d\n", childIndex, parent->entries[childIndex].key, parent->children[childIndex]->entries[0].key);
			}
			printf("Child index: %d\n", childIndex);

			// insert all children into parent (except for current)
			// if value is not greater than all values in parent->keys
			if (parent->entries[childIndex].key != 0){
				printf("That case\n");
				for (int i=0; i<MIN_KEYS; i++){
					parent->entries[i_value + 1 + i].key = parent->children[i_value + 1]->entries[i].key;
				}
				parent->children[i_value] = NULL;
				parent->children[i_value + 1] = NULL;

			} else if (parent->entries[i_value + 1].key == 0){
				printf("This case.\n");
				int save = parent->entries[i_value].key;
				parent->entries[i_value].key = parent->children[i_value]->entries[0].key;
				int new_index_parent;
				for (new_index_parent = 0; new_index_parent<MIN_KEYS; new_index_parent++){
					parent->entries[i_value + new_index_parent].key = parent->children[i_value]->entries[new_index_parent].key;
				}
				parent->entries[i_value + new_index_parent].key = save;
				parent->children[i_value] = NULL;
				parent->children[i_value + 1] = NULL;
			} else {
				printf("Something went wrong when deleting. Err-code:D5431\n");
			}
		}

		return 0;
	}


	// internal node
	if (current->children[0] != NULL){
		int child_index = getNodeIndex(current, value);
		int greatest_value = getIndexGreatestValue(current);

		int i_greatest_left = getIndexGreatestValue(current->children[child_index]);
		int num_of_keys_right = getIndexGreatestValue(current->children[child_index + 1]);

		// left-child-keys || right-child-keys > MIN_KEYS
		// left child
		if (i_greatest_left >= MIN_KEYS){
			return pushChildUpLeft(current, i_greatest_left, i_value, root);
		}

		// right child
		if (num_of_keys_right >= MIN_KEYS){
			return pushChildUpRight(current, child_index + 1, i_value, root);
		}

		// left-child-keys && right-child-keys <= MIN_KEYS
		if (i_greatest_left < MIN_KEYS && num_of_keys_right < MIN_KEYS){
			printf("l: %d, r: %d\n", current->children[child_index]->entries[0].key, current->children[child_index + 1]->entries[0].key);
			mergeChildren(current, value, child_index, root);
		}
		return 0;
	}
	return 1;
}



/*
 * Pushes the left child of a parent node to level of the parent node, i.e. inserts it before the parent node (as all of it´s values are smaller)
 * returns 0 or removeValueFromNode()
 *
 *
 */
int pushChildUpLeft(node * current, int i_greatest_left, int i_value, node ** root){
	printf("Pushing left child up\n");

	// find child-node with closest value
	node * child = current->children[i_greatest_left-1]; // i - 1 because i is the index of current in it's parent's child array.
	while (child->children[0] != NULL){
		child = child->children[getIndexGreatestValue(child) + 1];
	}
	printf("Recursive child: %d %d %d\n", child->entries[0].key, child->entries[1].key, child->entries[2].key);
	int i_closest_value = getIndexGreatestValue(child);
	int closest_value = child->entries[i_closest_value].key;
	printf("Closest v: %d\n", closest_value);
	// Check if closes_value can be taken from child
	if (child->entries[MIN_KEYS].key == 0){
		// Can't take closest_value without modification of child
		printf("Must modify closest_value-node, inorder to take closest value.\n");
		delete(closest_value, root);
		current->entries[i_value].key = closest_value;
		return 0;
	}
	else {
		current->entries[i_value].key = closest_value;
		return removeValueFromNode(closest_value, child, i_closest_value);
	}
}



/*
 * similar to pushCildUpLeft; is used, when the right child of a parent node needs to be pushed to the level of the parent node.
 */
int pushChildUpRight(node * current, int i_right_child, int i_value, node ** root){
	printf("Pushing right child up\n");

	// find child-node with closest value
	node * child = current->children[i_right_child];
	while (child->children[0] != NULL){
		child = child->children[0];
	}
	printf("Recursive child: %d %d %d\n", child->entries[0].key, child->entries[1].key, child->entries[2].key);
	int closest_value = child->entries[0].key;
	printf("Closest v: %d\n", closest_value);
	// Check if closes_value can be taken from child
	if (child->entries[MIN_KEYS].key == 0){
		// Can't take closest_value without modification of child
		printf("Must modify closest_value-node, inorder to take closest value.\n");
		delete(closest_value, root);
		current->entries[i_value].key = closest_value;
		return 0;
	}
	else {
		current->entries[i_value].key = closest_value;
		return removeValueFromNode(closest_value, child, 0);
	}
}



/*
* This function is used, when the value that is to be deleted is in a node that has MIN_KEYS-many keys.
* So if the value is removed, the node would have less than MIN_KEYS values.
* It also returns switchBorrowedValue(), for which the same return-rules apply as for this function.
*
* returns 0 if the sibling-node has successfully been borrowed from (This also concludes the deletion process).
* return 1 if an error occured (So deletion did not happen).
*/
int borrowValueFromSibling(int value, node * current, node ** root){
	node * parent = findParent(current, *root);

	// find index of current in parent
	int childIndex;
	for (childIndex = 0; childIndex < MAX_CHILDREN && parent->children[childIndex] != current; childIndex ++){
	}
	printf("child index: %d\n", childIndex);

	// find value in siblings that may be borrowed
	int i_sibling;
	// check left sibling
	if (parent->children[childIndex - 1] != NULL && childIndex > 0){
		// find index of greatest value
		i_sibling = getIndexGreatestValue(parent->children[childIndex - 1]);

		// check if left sibling has enough keys (MIN_KEYS + 1)
		if (i_sibling >= MIN_KEYS){
			printf("May borrow from left sibling (it has %d values)\n", i_sibling);
			printf("Greatest value from left sibling: %d\n", parent->children[childIndex - 1]->entries[i_sibling-1].key);
			printf("One further... %d\n", parent->children[childIndex - 1]->entries[i_sibling].key);
			return switchBorrowedValue(parent, childIndex, true, value, i_sibling); // i_sibling +1
		} else printf("Can't borrow from left sibling (it has %d values)\n", i_sibling);
	}
	// check right sibling
	else if (parent->children[childIndex + 1] != NULL && childIndex <= MAX_CHILDREN){
		// find number of keys in right sibling
		i_sibling = getIndexGreatestValue(parent->children[childIndex + 1]);

		// check if left sibling has enough keys (MIN_KEYS + 1)
		if (i_sibling >= MIN_KEYS){
			printf("May borrow from right sibling (it has %d values)\n", i_sibling);
			// use ->keys[0] because smallest value is at index 0
			printf("Smallest value from right sibling: %d\n", parent->children[childIndex + 1]->entries[0].key);
			return switchBorrowedValue(parent, childIndex, false, value, i_sibling);
		} else printf("Can't borrow from right sibling (it has %d values)\n", i_sibling);
	}
	printf("Can't borrow from any sibling.\n");
	return 1;
}



/*
* current -> parent
* parent -> sibling
* sibling -> delete-value
* For return-scenarios see comment above functions borrowValueFromSibling() and removeValueFromNode().
*/
int switchBorrowedValue(node * parent, int childIndex, bool sibling_is_left, int value, int i_sibling){
	printf("sBV->child: %d\n", parent->children[childIndex]->entries[0].key);
	entry_t borrowed_value;
	entry_t parent_value;

	// sibling is to the left
	if (sibling_is_left){
		borrowed_value = parent->children[childIndex - 1]->entries[i_sibling];
		parent_value = parent->entries[childIndex - 1];
		// set parent-value to borrowed-value
		parent->entries[childIndex - 1] = borrowed_value;

		// find index of value (that is to be deleted) in keys
		int index = getNodeIndex(parent->children[childIndex], value);
		printf("Index: %d, borrowed: %d, parent: %d\n", index, borrowed_value.key, parent_value.key);
		// keys[index + 1]: because index stops incrementing one before value is found

		// set value in current to parent-value
		parent->children[childIndex]->entries[index] = parent_value;

		// remove borrowed value from sibling
		printf("Removing value from sibling at index %d\n", index);
		return removeValueFromNode(borrowed_value.key, parent->children[childIndex - 1], index + 1);
	}
	// sibling is to the right
	else {
		printf("Right\n");
		borrowed_value = parent->children[childIndex + 1]->entries[0];
		parent_value = parent->entries[childIndex];
		printf("Bor val: %d, parent-val: %d\n", borrowed_value.key, parent_value.key);

		// set parent-value to borrowed-value
		parent->entries[childIndex] = borrowed_value;

		// find index of value (that is to be deleted) in keys
		int index = getNodeIndex(parent->children[childIndex], value);
		printf("Child index: %d\n", index);

		// set value in current to parent-value

		parent->children[childIndex]->entries[index] = parent_value;
		printf("Current: %d %d %d\n", parent->children[childIndex]->entries[0].key, parent->children[childIndex]->entries[1].key, parent->children[childIndex]->entries[2].key);
		printf("Parent: %d %d %d\n", parent->entries[0].key ,parent->entries[1].key, parent->entries[2].key);
		printf("Sibling: %d %d %d\n", parent->children[childIndex + 1]->entries[0].key, parent->children[childIndex + 1]->entries[1].key, parent->children[childIndex + 1]->entries[2].key);

		// remove borrowed value from sibling
		return removeValueFromNode(borrowed_value.key, parent->children[childIndex + 1], index);
	}
	printf("Could not remove value from node.\n");
	return 1;
}



/*
* Function to remove a value from a node (where num_of_keys > MIN_KEYS).
* parameters:
*	- index_value: index of last key in current (if -1 then no index has been specified -> index will be specified).
*
* returns 0 if removal was successfull.
* returns 1 if an error occured.
*/
int removeValueFromNode(int value, node * current, int index_value){
	if (index_value < 0) index_value = getNodeIndex(current, value);
	if (getIndexGreatestValue(current) < MIN_KEYS) return 1;
	// shift array to left
	for (int i=index_value; i<MAX_KEYS && current->entries[i].key != 0; i++){
		printf("i: %d, i+1: %d\n", current->entries[i].key, current->entries[i+1].key);
		current->entries[i] = current->entries[i+1];
	}
	return 0;
}


/*
* What about children pointer of child?
* returns 0 if child was pushed up successfully.
* returns 1 if error occured.
*/
int pushChildUp(node * current, int value, int index, int child_index, bool child_is_left){
	printf("index: %d, child-index: %d\n", index, child_index);
	entry_t child_value;
	int rem;
	// left child can be pushed up
	if (child_is_left && current->children[index] != NULL){
		child_value = current->children[index]->entries[child_index];
		current->entries[index] = child_value;
		rem = removeValueFromNode(child_value.key, current->children[index], child_index);
	}
	// right child can be pushed up
	else if (!child_is_left && current->children[index + 1] != NULL){
		child_value = current->children[index + 1]->entries[0];
		current->entries[index] = child_value;
		rem = removeValueFromNode(child_value.key, current->children[index + 1], child_index);
	}
	// no child can be pushed up
	else return 1;

	printf("Child value: %d\n", child_value.key);

	printf("removal %d\n", rem);

}



/*
* Function that merges two children, if the parent in between them is deleted.
* for return-scenarios, view removeValueFromNode().
*/
int mergeChildren(node * current, int value, int index, node ** root){
	printf("Merging: \n");
	printf("\t%d %d %d\n", current->children[index]->entries[0].key, current->children[index]->entries[1].key, current->children[index]->entries[2].key);
	printf("\t%d %d %d\n", current->children[index + 1]->entries[0].key, current->children[index + 1]->entries[1].key, current->children[index + 1]->entries[2].key);
	int len_left = getIndexGreatestValue(current->children[index]);
	int len_right = getIndexGreatestValue(current->children[index + 1]);
	for (int i=0; i<len_right+1; i++){
		current->children[index]->entries[len_left + 1 + i] = current->children[index + 1]->entries[i];
	}
	current->children[index + 1] = NULL;

	// move children pointer greater than index + 1 one to the left
	// printf("children: %d %d\n", current->children[0]->keys[0], current->children[2]->keys[0]);
	for (int i = index+1; i<MAX_CHILDREN; i++){
		current->children[i] = current->children[i+1];
	}
	printf("Here\n");
	bool new_root = false;
	if (current == *root) new_root = true;
	printf("Current ch of 0: %d\n", current->children[0]->entries[1].key);

	// if node could not have been removed + new root is to be created
	if (removeValueFromNode(value, current, index) != 0 && new_root){
		// copy all values from root into current->children[0]
		*root = current->children[0];
	}
	return 0;
}



/*
* merges parent and a child-node.
*
*
*/
int mergeParentAndChild(node * current, int value, node ** root){
	printf("ABC Current: %d\n", current->entries[0].key);
	node * parent = findParent(current, *root);

	// return error status if the parent has not enough children (child cannot be taken from parent).
	if (parent->children[MIN_CHILDREN] == NULL) return 1;

	// find index of current node in parent->children
	int childIndex;
	for (childIndex=0; childIndex<MAX_CHILDREN && parent->children[childIndex] != current; childIndex++){}

	// Move parent-value into child
	int last_current_index = getIndexGreatestValue(current);
	current->entries[last_current_index + 1] = parent->entries[childIndex];

	// Remove parent-value from parent->keys
	removeValueFromNode(parent->entries[childIndex].key, parent, childIndex);



	
	// Node that is to be removed 
	if (parent->children[childIndex + 1]){
		printf("Node has parent->children[childIndex + 1]\n");
		
		// copy all values from parent->children[childIndex + 1]->keys to current->keys
		printf("Length of current: %d\n", parent->entries[0].key);
		for (int i=0; i<MIN_KEYS; i++){
			current->entries[last_current_index + 2 + i] = parent->children[childIndex + 1]->entries[i];
		}
	}

	// node (where value_remove is) disappears
	else {
		printf("Value to be removed at index %d\n", childIndex);
		printf("Index of parent: %d\n", parent->entries[childIndex - 1].key);
		printf("children: %d, %d\n", parent->children[childIndex-1]->entries[0].key, parent->children[childIndex]->entries[0].key);
		
		// shift right: parent->children[childIndex -1]->keys (make room for insertion of parent value)
		for (int i=MAX_KEYS - 1; i>1; i--){
			printf("child: %d\n", parent->children[childIndex - 1]->entries[i].key);
			parent->children[childIndex - 1]->entries[i] = parent->children[childIndex - 1]->entries[i-1];
		}
		// insert parent value to lesser array as possible first key.
		// edge case that it´s greater exists, but calculation in normal case is minimal.
		int index;
		for (index=0; index<MAX_KEYS && parent->children[childIndex - 1]->entries[index].key >= parent->children[childIndex -1]->entries[index +1].key && parent->children[childIndex -1]->entries[index].key != 0; index++){}
		printf("Index: %d\n", index);
		parent->children[childIndex - 1]->entries[index] = parent->entries[childIndex - 1];
		parent->entries[childIndex - 1] = (entry_t){};
		
		parent->children[childIndex] = NULL;
	}

	
	// Shift left: parent->children
	for (int i = childIndex + 1; i < MAX_CHILDREN && parent->children[i] != NULL; i++){
		parent->children[i] = parent->children[i + 1];
	}


	// remove value from current
	removeValueFromNode(value, current, getNodeIndex(current, value));

	return 0;
}








