
#ifndef DELETE_H
#define DELETE_H

#ifndef BTREE_H
#include "btree.h"
#endif

#ifndef INSERTT_H
#include "insert.h"
#endif


inline int delete(int value);
inline int removeValueFromNode(int value, node * current, int index_value);
inline int borrowValueFromSibling(int value, node * current);
inline int switchBorrowedValue(node * parent, int childIndex, bool sibling_is_left, int value, int i_sibling);
inline int pushChildUp(node * current, int value, int index, int child_index, bool child_is_left);
inline int mergeChildren(node * current, int value, int index);
inline int mergeParentAndChild(node * current, int value);
inline int pushChildUpLeft(node * current, int i_greatest_left, int i_value);
inline int pushChildUpRight(node * current, int i_right_child, int i_value);

#endif
