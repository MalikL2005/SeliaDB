#ifndef BTREE_H
#include "btree.h"
#endif

#ifndef INSERTT_H
#include "insert.h"
#endif

#ifndef DELETE_H
#define DELETE_H



int delete(int value);
int removeValueFromNode(int value, node * current, int index_value);
int borrowValueFromSibling(int value, node * current);
int switchBorrowedValue(node * parent, int childIndex, bool sibling_is_left, int value, int i_sibling);
int pushChildUp(node * current, int value, int index, int child_index, bool child_is_left);
int mergeChildren(node * current, int value, int index);
int mergeParentAndChild(node * current, int value);
int pushChildUpLeft(node * current, int i_greatest_left, int i_value);
int pushChildUpRight(node * current, int i_right_child, int i_value);

#endif
