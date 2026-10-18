#ifndef BST_H
#define BST_H

/* 普通二叉搜索树（Binary Search Tree） */

typedef struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef TreeNode* PtrToBSTNode;


PtrToBSTNode insertBST(PtrToBSTNode root, int val);
PtrToBSTNode deleteBST(PtrToBSTNode root, int val);
int findBST(PtrToBSTNode root, int val);


#endif
