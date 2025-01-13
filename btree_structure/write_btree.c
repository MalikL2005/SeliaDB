/*
*
*
*
*
*
*/


#include <string.h>
#include "btree.h"
#include "write_btree.h"
#define MAX_NAME_LENGTH 20
#define FILE_NAME_LENGTH 15

/*
*
*
*/
int openTreeFile(node * cur){
	printf("Opening file %s\n", "Edos.btree");
	FILE * pFile = fopen("Edos.btree", "wb+");
	if (pFile == NULL) return 1;
	fwrite(cur, sizeof(node), 1, pFile);
	return 0;

	
	// write metadata
	// if (writeMetaData(Tree, pFile) != 0) return 1;
	//return writeTreeToFile(Tree->root, pFile);
}


/*
 *
 *
 */
int writeMetaData(btree * Tree, FILE *pFile){
	// tree name
	fwrite(Tree->name, strlen(Tree->name), 1, pFile);
	fseek(pFile, MAX_NAME_LENGTH, SEEK_SET);
	fwrite(&(Tree->root), sizeof(node *), 1, pFile);
	fwrite(Tree->filename, strlen(Tree->filename), 1, pFile);
	fseek(pFile, MAX_NAME_LENGTH + FILE_NAME_LENGTH + sizeof(node *), SEEK_SET);
	fwrite(&(Tree->min_children), sizeof(int), 1, pFile);
	fwrite("End", strlen("End"), 1, pFile);
	return 0;
}


/*
*
*
*
*/
int writeTreeToFile(node *current, FILE * pFile){
	if (root == NULL) return 1;
	if (current == NULL) return 0;
	printf("Writing to file\n");
	// for (int j=0; j<MAX_KEYS; j++) printf("%d) %d\n", j+1, current->keys[j]);
	printf("\n");
	//for (int i=0; i<MAX_CHILDREN && current->children[i] != NULL; i++){
	//	writeTreeToFile(current->children[i], pFile);
	//}
	return 0;
}



int readTreeFromFile(btree * Tree, FILE * pFile){
	fseek(pFile, 0, SEEK_SET);
	printf("Here\n");
	
	Tree->name = malloc(sizeof(char)*MAX_NAME_LENGTH);
	fread(Tree->name, sizeof(char)*MAX_NAME_LENGTH, 1, pFile);
	printf("Here\n");

	Tree->root = malloc(sizeof(node *));
	fread(Tree->root, sizeof(node *), 1, pFile);
	printf("Here\n");

	Tree->filename = malloc(sizeof(char)*FILE_NAME_LENGTH);
	fread(Tree->filename, sizeof(char)*FILE_NAME_LENGTH, 1, pFile);
	printf("Here\n");

	Tree->min_children = malloc(sizeof(int));
	fread(&(Tree->min_children), sizeof(int), 1, pFile);
	printf("Here\n");
	return 0;
}





