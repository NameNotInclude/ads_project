#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

/*
 * 迭代版 AVL 树。
 *
 * 原递归实现依赖函数调用栈在回溯时更新高度/平衡因子，
 * 这里改为显式维护一条从根到操作结点的路径栈，从而去掉递归。
 *
 * AVL 树高度是 O(log n)，对 int 数据而言高度最多几十，
 * 128 的栈容量足够容纳任意现实规模（n 远小于 2^128）的树。
 */
#define AVL_STACK_SIZE 128

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

static PtrToANode NewANode(int n)
{
    PtrToANode newnode = (PtrToANode)malloc(sizeof(Anode));
    newnode->data = n;
    newnode->bf = 0;
    newnode->height = 0;
    newnode->left = NULL;
    newnode->right = NULL;

    return newnode;
}

/* 根据平衡因子做 LL / RR / LR / RL 旋转，返回旋转后的子树根 */
static PtrToANode balanceAVL(PtrToANode AVL)
{
    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRotA(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRotA(AVL->left);
        return RightRotA(AVL);
    }

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRotA(AVL);

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRotA(AVL->right);
        return LeftRotA(AVL);
    }

    return AVL;
}

PtrToANode insertAVL(PtrToANode AVL, int n)
{
    PtrToANode path[AVL_STACK_SIZE];
    int top = 0;

    // 空树：直接作为根
    if (AVL == NULL)
        return NewANode(n);

    // 1. 自上而下找到插入位置，并记录路径
    PtrToANode curr = AVL;
    while (curr != NULL)
    {
        path[top++] = curr;

        if (n < curr->data)
        {
            if (curr->left == NULL)
                break;
            curr = curr->left;
        }
        else if (n > curr->data)
        {
            if (curr->right == NULL)
                break;
            curr = curr->right;
        }
        else
        {
            // 已存在，原树不变
            return AVL;
        }
    }

    // 2. 挂上新结点
    PtrToANode node = NewANode(n);
    if (n < curr->data)
        curr->left = node;
    else
        curr->right = node;

    // 3. 自下而上回溯，更新高度并旋转；子树高度未变则可提前结束
    while (top > 0)
    {
        PtrToANode p = path[--top];
        int oldHeight = p->height;

        update(p);
        PtrToANode sub = balanceAVL(p);

        // 把（可能旋转后的）子树根接回祖父
        if (top > 0)
        {
            PtrToANode gp = path[top - 1];
            if (gp->left == p)
                gp->left = sub;
            else
                gp->right = sub;
        }
        else
        {
            AVL = sub;
        }

        if (sub->height == oldHeight)
            break;
    }

    return AVL;
}

PtrToANode deleteAVL(PtrToANode AVL, int key)
{
    PtrToANode path[AVL_STACK_SIZE];
    int top = 0;

    // 1. 查找目标结点，并记录根到它的路径
    PtrToANode curr = AVL;
    while (curr != NULL && curr->data != key)
    {
        path[top++] = curr;
        if (key < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    // 不存在，原树不变
    if (curr == NULL)
        return AVL;

    path[top++] = curr;                 // 栈顶为待删除结点
    PtrToANode del = curr;

    // 2. 有两个孩子时，用右子树最小结点（后继）替换数据，
    //    真正被摘除的结点变为后继（它没有左孩子）
    if (del->left != NULL && del->right != NULL)
    {
        PtrToANode succ = del->right;
        path[top++] = succ;
        while (succ->left != NULL)
        {
            succ = succ->left;
            path[top++] = succ;
        }

        del->data = succ->data;
        del = succ;
    }

    // 3. 摘除 del（至多一个孩子），用 child 顶替
    top--;                              // 弹出 del
    PtrToANode child = del->left != NULL ? del->left : del->right;

    if (top == 0)
    {
        AVL = child;                    // del 是根
    }
    else
    {
        PtrToANode parent = path[top - 1];
        if (parent->left == del)
            parent->left = child;
        else
            parent->right = child;
    }

    free(del);

    // 4. 自下而上回溯，更新高度并旋转；子树高度未变则可提前结束
    while (top > 0)
    {
        PtrToANode p = path[--top];
        int oldHeight = p->height;

        update(p);
        PtrToANode sub = balanceAVL(p);

        if (top > 0)
        {
            PtrToANode gp = path[top - 1];
            if (gp->left == p)
                gp->left = sub;
            else
                gp->right = sub;
        }
        else
        {
            AVL = sub;
        }

        if (sub->height == oldHeight)
            break;
    }

    return AVL;
}
