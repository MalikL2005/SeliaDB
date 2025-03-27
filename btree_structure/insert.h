/*
 *
 *
 *
 *
 */


#ifndef INSERT_H
#define INSERT_H

#include "btree.h"

void insert(entry entry, node *current);
entry * createTempArr(entry entry, node *current);
void splitNode(entry entry, node *current);
void insertToNode(entry entry, node *current);
node *findParent(node * target, node * current);


#endif


