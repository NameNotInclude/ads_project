#ifndef AVL_H
#define AVL_H

typedef struct n
{
    struct n* left;
    struct n* right;
    int data;
    int bf;
    int height;
}node;

typedef node* PtrToNode;

void update(PtrToNode AVL);
PtrToNode RightRot(PtrToNode AVL);
PtrToNode LeftRot(PtrToNode AVL);
PtrToNode insert(PtrToNode AVL, int n);
PtrToNode deleteNode(PtrToNode AVL, int key);
#endif
