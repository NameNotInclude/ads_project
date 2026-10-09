#ifndef RBT_H
#define RBT_H

#define RED 0
#define BLACK 1

typedef struct nR
{
    struct nR* left;
    struct nR* right;
    struct nR* parent;
    int data;
    int color;
} Rnode;

typedef Rnode* PtrToRNode;

#define IS_BLACK(x) ((x) == NULL || (x)->color == BLACK)
#define IS_RED(x)   ((x) != NULL && (x)->color == RED)

PtrToRNode RightRotR(PtrToRNode RBT);
PtrToRNode LeftRotR(PtrToRNode RBT);
PtrToRNode insertRBT(PtrToRNode RBT, int key);
PtrToRNode deleteRBT(PtrToRNode RBT, int key);

#endif
