#ifndef B_H
#define B_H

typedef struct Bn
{
    int leaf;
    int* data;
    int num;

    struct Bn** children;
    struct Bn* next;
    struct Bn* prev;
}BNode;
typedef struct Bn* PtrToBNode;

typedef struct 
{
    PtrToBNode root;
    PtrToBNode head;
    int order;
}BTree;

BTree* initial(int order);
BTree* insertBPT(BTree* B, int key);
BTree* deleteBPT(BTree* B, int key);
#endif