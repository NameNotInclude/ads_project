#ifndef SPLAY_H
#define SPLAY_H

/* 伸展树（Splay Tree） */

typedef struct SplayNode
{
    int val;
    struct SplayNode *left;
    struct SplayNode *right;
} SplayNode;

typedef SplayNode* PtrToSplayNode;

PtrToSplayNode splay(PtrToSplayNode root, int val);
PtrToSplayNode insertSplay(PtrToSplayNode root, int val);
PtrToSplayNode deleteSplay(PtrToSplayNode root, int val);
int findSplay(PtrToSplayNode* root, int val);


#endif
