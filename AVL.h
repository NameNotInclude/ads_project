#ifndef AVL_H
#define AVL_H

typedef struct nA
{
    struct nA* left;
    struct nA* right;
    int data;
    int bf;
    int height;
}Anode;

typedef Anode* PtrToANode;

void update(PtrToANode AVL);
PtrToANode RightRotA(PtrToANode AVL);
PtrToANode LeftRotA(PtrToANode AVL);
PtrToANode insertAVL(PtrToANode AVL, int n);
PtrToANode deleteAVL(PtrToANode AVL, int key);
#endif
