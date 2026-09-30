#ifndef RBT_H
#define RBT_H

#define RED 0
#define BLACK 1

typedef struct n
{
    struct n* left;
    struct n* right;
    struct n* parent;
    int data;
    int color;
} node;

typedef node* PtrToNode;

#define IS_BLACK(x) ((x) == NULL || (x)->color == BLACK)
#define IS_RED(x)   ((x) != NULL && (x)->color == RED)

PtrToNode RightRot(PtrToNode RBT);
PtrToNode LeftRot(PtrToNode RBT);
PtrToNode insert(PtrToNode RBT, int key);
PtrToNode deleteNode(PtrToNode RBT, int key);

#endif
