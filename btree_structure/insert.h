/*
 *
 *
 *
 *
 */


#ifndef INSERT_H
#define INSERT_H

#include "btree.h"

void insert(entry_t entry, node *current);
entry_t * createTempArr(entry_t entry, node *current);
void splitNode(entry_t entry, node *current);
void insertToNode(entry_t entry, node *current);
node *findParent(node * target, node * current);


#endif


