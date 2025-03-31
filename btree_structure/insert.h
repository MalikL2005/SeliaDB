/*
 *
 *
 *
 *
 */


#ifndef INSERT_H
#define INSERT_H

#include "btree.h"
#include "database.h"
#include "types.h"

int insert(entry_t entry, node *current, table_t * tb);
entry_t * createTempArr(entry_t entry, node *current);
void insertToNode(entry_t entry, node *current, node **root);
void splitNode(entry_t entry, node *current, node **root);
node *findParent(node * target, node * current);
entry_t insertDefaultValues(table_metadata_t * tb);


#endif


