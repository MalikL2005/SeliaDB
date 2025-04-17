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
#include "types.h"
#include <string.h>

/* 
 * Void function that is called first when a id is inserted.
 * It traverses the tree until the right leaf (where the id is to be inserted) is found.
 * Upon finding the right leaf node, the function calls either insertToNode (if node is not full) or splitNode (if node is full).
*/
int insert(entry_t * entry, node *current, table_t * tb){
	// 1. If the tree is empty, allocate a root node and insert the key.
	if (tb->root == NULL){
		printf("Root is null\n");
		tb->root = malloc(sizeof(node));
        memset(tb->root, 0, sizeof(node));
        tb->root->entries = malloc(sizeof(entry_t)*MAX_KEYS);
        memset(tb->root->entries, 0, sizeof(entry_t)*MAX_KEYS);
		tb->root->entries[0] = *entry;
		return 0;
	}
    if (current->children == NULL){
        tb->root->children = malloc(sizeof(node *) * MAX_CHILDREN);
        memset(tb->root->children, 0, sizeof(node*)* MAX_CHILDREN);
    }
    if (current->entries == NULL){
        tb->root->entries = malloc(sizeof(entry_t) * MAX_KEYS);
        memset(tb->root->entries, 0, MAX_KEYS * sizeof(entry_t));
    }
	// node is leaf and is not full
	if (current->children[0] == NULL && current->entries[MAX_KEYS - 1].key == 0){
		printf("Leaf node and not full\n");
		insertToNode(*entry, current);
		return 0;
	}
	// node is leaf and is full
	if (current->children[0] == NULL && current->entries[MAX_KEYS - 1].key != 0){
		splitNode(*entry, current, &tb->root);
        free(current);
		return 0;
	}

	// node is not leaf
	if (current->children[0]){
		// traverse tree until right leaf node is found
		int num_of_children = 0;
		bool is_inserted = false;
		for (int i=0; i<MAX_CHILDREN && current->entries[i].key > 0; i++){
			printf("id: %d; current->entries: %d\n", entry->key, current->entries[i].key);
			if (entry->key <= current->entries[i].key){
				insert(entry, current->children[i], tb);
				is_inserted = true;
				break;
			}
			num_of_children ++;
		}
		printf("num of children %d\n", num_of_children);
        printf("entry_t: %d\n", entry->key);
		if (!is_inserted && entry->key > current->entries[num_of_children].key) insert(entry, current->children[num_of_children], tb);
	}
    // Return error-code if no insertion
    return 1;
}



/* 
 * This function creates and returns an array of entries.
 * It is called when a node has reached MAX_KEYS.
 * Therefore, it creates an array of size MAX_KEYS + 1 (because the node (current) overflows.
*/
int createTempArr(entry_t * temp_arr, entry_t entry, node *current){
	// copy current node´s key array unitl id
	int i_original;
	int i_copy;
	for (i_original=0; i_original<MAX_KEYS; i_original++){
		if (entry.key <= current->entries[i_original].key || current->entries[i_original].key == 0) break;
		*(temp_arr + i_original) = current->entries[i_original];
	}

	// insert id to temporary array
	i_copy = i_original;
	*(temp_arr + i_copy) = entry;
	i_copy ++;

	// copy rest
	for (int j=i_original; j<MAX_KEYS; j++){
		*(temp_arr + i_copy) = current->entries[j];
		i_copy ++;
	}

	// print temp array
	printf("New array: ");
	for (int i=0; i<MAX_KEYS + 1; i++){ printf("%d ", (temp_arr+i)->key); }
	printf("\n");
	return 0;
}


void free_temp_arr(entry_t * temp_arr){
    if (temp_arr == NULL){
        free (temp_arr);
        return;
    }
    for (int i=0; i<=MAX_KEYS; i++){
        // free entry
    }
    free(temp_arr);
}



/*
* inserts default values to entry
* Default values are: - VARCHAR: ""
*                     - INTEGER: 0
*                     - FLOAT: 0.0
*                     - BOOL: FALSE
* return value of NULL indicates error
*/
entry_t insertDefaultValues(table_metadata_t * tb){
    int offset = 0;
    for (int i=0; i<tb->num_of_columns; i++){
        printf("Size of [%s] this is %d\n", get_type_as_string(tb->columns[i]->type), tb->columns[i]->size);
        offset += tb->columns[i]->size;
        printf("Offset: %d\n", tb->columns[i]->size);
    }
    /*return (entry_t){tb->last_index +1, (tb->last_index +1)*2, }*/
    return (entry_t) {};
}



/*
 * Void function that splits full nodes, starting with a full leaf node.
 * After splitting the full leaf node, the middle id (upon which the split happened) is inserted to the leaf node's parent
 * If the parent itself is full, the method is called recursively on the parent of each respectable node until A or B happens:
 * A) a non-full node is found: the middle id that is passed from the child's split will be inserted.
 * B) the function's recursive call reaches the root (which is full): then a new root is allocated and the previous root is split.
*/
void splitNode(entry_t entry, node *current, node **root){
	printf("id: %d\n", entry.key);
	printf("SplitNode\n");
	printf("%d %d %d\n", current->entries[0].key, current->entries[1].key, current->entries[2].key);
	// current->entries is full
	if (current->entries[MAX_KEYS - 1].key != 0){
        entry_t temp_arr [MAX_KEYS]= {(entry_t){}};
	    createTempArr(temp_arr, entry, current);
		// calculate middle id
        entry_t middle_entry = current->entries[(MAX_KEYS/2)+1];
		printf("SN Middle id: %d\n", middle_entry.key);

		// create two seperate nodes
        node * new_left = malloc(sizeof(node));
        memset(new_left, 0, sizeof(node));
        new_left->entries = malloc(sizeof(entry_t)*MAX_KEYS);
        memset(new_left->entries, 0, sizeof(entry_t)*MAX_KEYS);
        new_left->children = malloc(sizeof(node*)*MAX_CHILDREN);
        memset(new_left->children, 0, sizeof(node*)*MAX_CHILDREN);

        node * new_right = malloc(sizeof(node));
        memset(new_right, 0, sizeof(node));
        new_right->entries = malloc(sizeof(entry_t)*MAX_KEYS);
        memset(new_right->entries, 0, sizeof(entry_t)*MAX_KEYS);
        new_right->children = malloc(sizeof(node*)*MAX_CHILDREN);
        memset(new_right->children, 0, sizeof(node*)*MAX_CHILDREN);
        printf("alloced nr %X\n", new_right->children);
        printf("alloced nr %X\n", new_right->entries);

		// copy ids to new nodes
		int k = 0;
        printf("\nmiddle: %d\n", middle_entry.key);
		for (int i=0; i<MAX_KEYS+1; i++){
			printf("temp arr: %d\n", (temp_arr + i)->key);
			if (temp_arr[i].key < middle_entry.key){
				new_left->entries[i] = current->entries[i];
                memset(&current->entries[i], 0, sizeof(entry_t));
				printf("Left (%d): %d\n", i, new_left->entries[i].key);
			}
			else if (temp_arr[i].key > middle_entry.key) {
				new_right->entries[k] = current->entries[i];
				printf("Right(%d): %d\n", k, new_right->entries[k].key);
                k++;
			}
		}


		if (current == *root){
			// create new root with children
			printf("Splitting root node\n");
			node * new_root = malloc(sizeof(node));
            memset(new_root, 0, sizeof(node));
            new_root->entries = malloc(sizeof(entry_t)* MAX_KEYS);
            memset(new_root->entries, 0, sizeof(entry_t)*MAX_KEYS);
            new_root->children = malloc(sizeof(node *)*MAX_CHILDREN);
            memset(new_root->children, 0, sizeof(node *)*MAX_CHILDREN);

			// adjust children of children of new root pointers
			int middle = MAX_CHILDREN / 2;
			int right = 0;
			for (int i=0; i<MAX_CHILDREN; i++){
				if (i <= middle){
					new_left->children[i] = (*root)->children[i];
                    printf("Left: %d\n", i);
				} else {
					new_right->children[right] = (*root)->children[i];
                    printf("Right: %d\n", right);
					right ++;
				}
			}
            
            printf("Printing right child: \n");
            for (int i=0; i<MAX_KEYS; i++){
                printf("%d ", new_right->entries[i].key);
            }
            printf("\n");
            printf("Printing left child: \n");
            for (int i=0; i<MAX_KEYS; i++){
                printf("%d ", new_left->entries[i].key);
            }
            printf("\n");

            free((*root)->entries);
            free((*root)->children);
			*root = new_root;
			(*root)->entries[0] = middle_entry;
			(*root)->children[0] = new_left;
			(*root)->children[1] = new_right;
            printf("New root: %d\n", (*root)->entries[0].key);
            printf("Left: %d\n", (*root)->children[0]->entries[0].key);
            printf("Right: %d\n", (*root)->children[0]->entries[0].key);

		} else {
			// adjust children of parent node
			printf("Finding parent\n");
			printf("%d %d %d\n", current->entries[0].key, current->entries[1].key, current->entries[2].key);
			node * parent = findParent(current, *root);

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
			for (int i=0; i<MAX_CHILDREN; i++){
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
				for (int i=MAX_CHILDREN-1; i>child_index; i--){
					parent->children[i] = parent->children[i-1];
				}
				// adjust parent to children pointer
				parent->children[child_index] = new_left;
				parent->children[child_index + 1] = new_right;
                printf("parent: %d\n", parent->entries[0].key);
                printf("Middle: %d\n", middle_entry.key);
                /*printf("New left: %d\n", root->children[child_index]->entries[0].key);*/
                /*printf("New right: %d\n", root->children[child_index+1]->entries[0].key);*/
				printf("SplitNode parent\n");
                // recursive call to split the parent or to insert middle_id if it's not full
                splitNode(middle_entry, parent, root);
			}
		}
	}
	// current node is not full
	else if (current->entries[MAX_KEYS - 1].key == 0){
        if ((*root)->children[1] != NULL && (*root)->children[1]->children[0] != NULL){printf("Children I: %d\n", (*root)->children[1]->entries[0].key);}
		insertToNode(entry, current);
        if ((*root)->children[1] != NULL && (*root)->children[1]->children[0] != NULL){printf("Children II: %d\n", (*root)->children[1]->entries[0].key);}
	}
}



/*
 * Void function that is used to insert a key to a non-full node.
 * This node may be the root, a leaf or an internal node.
*/
void insertToNode(entry_t entry, node *current){
	int placeToInsert;
	for (placeToInsert=0; placeToInsert<MAX_KEYS; placeToInsert++){
		if(entry.key <= current->entries[placeToInsert].key || current->entries[placeToInsert].key == 0){
			break;
		}
	}
	printf("Inserting at index %d\n", placeToInsert);
    if (current->children[1] != NULL && current->children[1]->children[0] != NULL){printf("Children I.I: %d\n", current->children[1]->entries[0].key);}
    printf("Current: %d\n", current->entries[0].key);
    struct node * save_children[10];
    memcpy(save_children, current->children, sizeof(current->children));
	// shift array to right
	for (int j=MAX_KEYS-1; j>placeToInsert; j--){
		current->entries[j] = current->entries[j-1];
	}
    memcpy(current->children, save_children, sizeof(current->children));
	// insert entry
	current->entries[placeToInsert] = entry;
}



/* 
 * This function finds the parent of a target node.
 * It traverses the tree until one of the nodes' children match the target node.
 * Upon finding the parent of target, the parent node is returned.
*/
node * findParent(node * target, node * current){
    printf("Printing current: \n");
    for (int i=0; i<MAX_KEYS; i++){
        printf("%d -", current->entries[i].key);
    }
    printf("\n");
	// print address and children of current
	printf("Searching for %d\n", target->entries[0].key);
	// printf("Current: %d %d %d\n", current->entries[0], current->entries[1], current->entries[2]);
	if (current->children[0] != NULL){
		for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
			printf("%d %d %d\n", current->children[i]->entries[0].key, current->children[i]->entries[1].key, current->children[i]->entries[2].key);
		}
	}
	// iterate over all children; if a child equals target parent node is found
	for (int i=0; i<MAX_CHILDREN; i++){
		if (current->children[i] == target){
			printf("Found parent\n");
			for (int j=0; j<MAX_KEYS; j++){
				printf("%d) parent: %d, child: %d\n", j+1, current->entries[j].key, target->entries[j].key);
			}
			return current;
		}
	}
	printf("Current is not a direct parent\n");
	// if current is not a direct parent of target; find next closest node id -> iterate over tree
	int index;
	for (index=0; index<MAX_KEYS; index++){
		// current is not full; searched id is greater than all in current
		if (current->entries[index].key == 0 && current->entries[index-1].key < target->entries[0].key){
			printf("Largest id at index %d\n", index);
            printf("Here\n");
            printf("%d\n", current->children[index]->entries[0].key);
            return findParent(target, current->children[index]);
		}
		// current is full; searched id is greater than all in current
		if (index == MAX_KEYS-1 && current->entries[index-1].key < target->entries[0].key){
			printf("largest id at index %d\n", index);
			return findParent(target, current->children[index + 1]);
		}
		printf("%d) %d - %d\n", index, current->entries[index].key, target->entries[0].key);
		if (target->entries[0].key <= current->entries[index].key && current->children[index] != NULL){
			printf("goint to child of index %d\n", index);
			return findParent(target, current->children[index]);
		}
	}
    return current;
}


