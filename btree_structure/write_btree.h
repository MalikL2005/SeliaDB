#ifndef WRITE_BTREE_H
#define WRITE_BTREE_H



int openTreeFile(node * cur);
int writeMetaData(btree * Tree, FILE *pFile);
int writeTreeToFile(node *current, FILE * pFile);
int readTreeFromFile(btree * Tree, FILE * pFile);


#endif
