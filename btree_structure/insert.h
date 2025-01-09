/*
 *
 *
 *
 *
 */


#ifndef INSERT_H
#define INSERT_H

#include "btree.h"

void insert(int value, node *current);
int * createTempArr(int value, node *current);
void splitNode(int value, node *current);
void insertToNode(int value, node *current);
node *findParent(node * target, node * current);


#endif


