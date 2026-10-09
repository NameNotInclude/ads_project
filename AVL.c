#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

void update(PtrToANode AVL)
{
    if (AVL == NULL)
        return;

    int h_l = AVL->left == NULL ? -1 : AVL->left->height;
    int h_r = AVL->right == NULL ? -1 : AVL->right->height;

    AVL->height = h_l > h_r ? h_l + 1 : h_r + 1;
    AVL->bf = h_l - h_r;
}

PtrToANode RightRotA(PtrToANode AVL)
{
    PtrToANode L = AVL->left;

    AVL->left = L->right;
    L->right = AVL;

    update(AVL);
    update(L);

    return L;
}

PtrToANode LeftRotA(PtrToANode AVL)
{
    PtrToANode R = AVL->right;

    AVL->right = R->left;
    R->left = AVL;

    update(AVL);
    update(R);

    return R;
}

PtrToANode insertAVL(PtrToANode AVL,int n)
{
    if (AVL==NULL)
    {
        PtrToANode newnode = (PtrToANode)malloc(sizeof(Anode));
        newnode->data = n;
        newnode->bf = 0;
        newnode->height = 0;
        newnode->left = NULL;
        newnode->right = NULL;

        return newnode;
    }

    if (n < AVL->data)
        AVL->left = insertAVL(AVL->left, n);
    else if (n > AVL->data)
        AVL->right = insertAVL(AVL->right, n);
    else
        return AVL;

    update(AVL);

    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRotA(AVL);

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRotA(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRotA(AVL->left);
        return RightRotA(AVL);
    }

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRotA(AVL->right);
        return LeftRotA(AVL);
    }

    return AVL;
}

PtrToANode FindMin(PtrToANode AVL)
{
    PtrToANode curr=AVL;

    while (curr != NULL && curr->left != NULL)
        curr = curr->left;

    return curr;
}

PtrToANode deleteAVL(PtrToANode AVL,int key)
{
    if (AVL == NULL)
        return NULL;

    if (AVL->data > key)
        AVL->left = deleteAVL(AVL->left, key);
    else if (AVL->data < key)
        AVL->right = deleteAVL(AVL->right, key);
    else
    {
        if (AVL->left == NULL || AVL->right == NULL)
        {
            PtrToANode temp = AVL->left ? AVL->left : AVL->right;
            free(AVL);
            return temp;
        }

        PtrToANode temp = FindMin(AVL->right);
        AVL->data = temp->data;
        AVL->right = deleteAVL(AVL->right, temp->data);
    }

    update(AVL);

    if (AVL == NULL)
        return NULL;

    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRotA(AVL);

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRotA(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRotA(AVL->left);
        return RightRotA(AVL);
    }

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRotA(AVL->right);
        return LeftRotA(AVL);
    }

    return AVL;
}
