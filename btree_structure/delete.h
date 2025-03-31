#ifndef BTREE_H
#include "btree.h"
#endif

#ifndef INSERTT_H
#include "insert.h"
#endif

#ifndef DELETE_H
#define DELETE_H



int delete(int value, node ** root);
int removeValueFromNode(int value, node * current, int index_value);
int borrowValueFromSibling(int value, node * current, node ** root);
int switchBorrowedValue(node * parent, int childIndex, bool sibling_is_left, int value, int i_sibling);
int pushChildUp(node * current, int value, int index, int child_index, bool child_is_left);
int mergeChildren(node * current, int value, int index, node ** root);
int mergeParentAndChild(node * current, int value, node ** root);
int pushChildUpLeft(node * current, int i_greatest_left, int i_value, node ** root);
int pushChildUpRight(node * current, int i_right_child, int i_value, node ** root);

#endif
