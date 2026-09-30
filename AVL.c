#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

void update(PtrToNode AVL)
{
    if (AVL == NULL)
        return;

    int h_l = AVL->left == NULL ? -1 : AVL->left->height;
    int h_r = AVL->right == NULL ? -1 : AVL->right->height;

    AVL->height = h_l > h_r ? h_l + 1 : h_r + 1;
    AVL->bf = h_l - h_r;
}

PtrToNode RightRot(PtrToNode AVL)
{
    PtrToNode L = AVL->left;

    AVL->left = L->right;
    L->right = AVL;

    update(AVL);
    update(L);

    return L;
}

PtrToNode LeftRot(PtrToNode AVL)
{
    PtrToNode R = AVL->right;

    AVL->right = R->left;
    R->left = AVL;

    update(AVL);
    update(R);

    return R;
}

PtrToNode insert(PtrToNode AVL,int n)
{
    if (AVL==NULL)
    {
        PtrToNode newnode = (PtrToNode)malloc(sizeof(node));
        newnode->data = n;
        newnode->bf = 0;
        newnode->height = 0;
        newnode->left = NULL;
        newnode->right = NULL;

        return newnode;
    }

    if (n < AVL->data)
        AVL->left = insert(AVL->left, n);
    else if (n > AVL->data)
        AVL->right = insert(AVL->right, n);
    else
        return AVL;

    update(AVL);

    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRot(AVL);

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRot(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRot(AVL->left);
        return RightRot(AVL);
    }

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRot(AVL->right);
        return LeftRot(AVL);
    }

    return AVL;
}

PtrToNode FindMin(PtrToNode AVL)
{
    PtrToNode curr=AVL;

    while (curr != NULL && curr->left != NULL)
        curr = curr->left;

    return curr;
}

PtrToNode deleteNode(PtrToNode AVL,int key)
{
    if (AVL == NULL)
        return NULL;

    if (AVL->data > key)
        AVL->left = deleteNode(AVL->left, key);
    else if (AVL->data < key)
        AVL->right = deleteNode(AVL->right, key);
    else
    {
        if (AVL->left == NULL || AVL->right == NULL)
        {
            PtrToNode temp = AVL->left ? AVL->left : AVL->right;
            free(AVL);
            return temp;
        }

        PtrToNode temp = FindMin(AVL->right);
        AVL->data = temp->data;
        AVL->right = deleteNode(AVL->right, temp->data);
    }

    update(AVL);

    if (AVL == NULL)
        return NULL;

    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRot(AVL);

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRot(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRot(AVL->left);
        return RightRot(AVL);
    }

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRot(AVL->right);
        return LeftRot(AVL);
    }

    return AVL;
}