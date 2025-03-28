/*
 *
 *
 *
 *
 */


#ifndef INSERT_H
#define INSERT_H

#include "btree.h"

void insert(entry_t entry, node *current, node **root);
entry_t * createTempArr(entry_t entry, node *current);
void insertToNode(entry_t entry, node *current, node **root);
void splitNode(entry_t entry, node *current, node **root);
node *findParent(node * target, node * current);


#endif


