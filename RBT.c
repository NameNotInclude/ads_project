#include <stdio.h>
#include <stdlib.h>
#include "RBT.h"

PtrToNode RightRot(PtrToNode RBT)
{
    PtrToNode L = RBT->left;
    PtrToNode P = RBT->parent;

    RBT->left = L->right;
    if (L->right != NULL)
        L->right->parent = RBT;

    L->right = RBT;
    RBT->parent = L;

    // 把新的子树根接回原来的父亲（祖父）上
    L->parent = P;
    if (P != NULL)
    {
        if (P->left == RBT)
            P->left = L;
        else
            P->right = L;
    }

    return L;
}

PtrToNode LeftRot(PtrToNode RBT)
{
    PtrToNode R = RBT->right;
    PtrToNode P = RBT->parent;

    RBT->right = R->left;
    if (R->left != NULL)
        R->left->parent = RBT;

    R->left = RBT;
    RBT->parent = R;

    // 把新的子树根接回原来的父亲（祖父）上
    R->parent = P;
    if (P != NULL)
    {
        if (P->left == RBT)
            P->left = R;
        else
            P->right = R;
    }

    return R;
}

PtrToNode insert(PtrToNode RBT,int key)
{
    PtrToNode curr=RBT,prev=NULL;
    while (curr)
    {
        if (key < curr->data)
        {
            prev=curr;
            curr=curr->left;
        }
        else if (key > curr->data)
        {
            prev=curr;
            curr=curr->right;
        }
        else
            return RBT;
    }

    PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
    newnode->color=RED;
    newnode->data=key;
    newnode->left=NULL;
    newnode->right=NULL;
    newnode->parent=prev;

    PtrToNode father=prev,uncle,grandfather;

    //插入根节点
    if (father==NULL)
    {
        newnode->color=BLACK;
        return newnode;
    }

    if (key>father->data)
        father->right=newnode;
    else
        father->left=newnode;

    // 红黑树调整：父亲是红色时才有冲突
    while (father!=NULL && father->color==RED)
    {
        grandfather=father->parent;

        // 父亲是根，直接涂黑
        if (grandfather==NULL)
        {
            father->color=BLACK;
            break;
        }

        // 找叔叔
        if (grandfather->left==father)
            uncle=grandfather->right;
        else
            uncle=grandfather->left;

        // 情况一：叔叔是红色 —— 只变色，冲突向上传递
        if (IS_RED(uncle))
        {
            father->color=BLACK;
            uncle->color=BLACK;
            grandfather->color=RED;

            newnode=grandfather;
            father=newnode->parent;
        }
        // 情况二：叔叔是黑色 —— 旋转
        else
        {
            if (grandfather->left==father)
            {
                // LR：先左旋父亲
                if (newnode==father->right)
                {
                    newnode=LeftRot(father);
                    father=newnode->parent;
                }
                // LL：右旋祖父
                PtrToNode sub=RightRot(grandfather);
                sub->color=BLACK;
                sub->right->color=RED;
            }
            else
            {
                // RL：先右旋父亲
                if (newnode==father->left)
                {
                    newnode=RightRot(father);
                    father=newnode->parent;
                }
                // RR：左旋祖父
                PtrToNode sub=LeftRot(grandfather);
                sub->color=BLACK;
                sub->left->color=RED;
            }

            break;
        }
    }

    // 回到整棵树的根，并保证根是黑色
    while (RBT!=NULL && RBT->parent!=NULL)
        RBT=RBT->parent;
    RBT->color=BLACK;

    return RBT;
}

PtrToNode findmin(PtrToNode RBT)
{
    PtrToNode check = RBT;
    while (check->left)
    {
        check = check->left;
    }

    return check;
}

// 用 v 替换 u 的位置（v 可以为 NULL），返回新的树根
static PtrToNode transplant(PtrToNode RBT, PtrToNode u, PtrToNode v)
{
    if (u->parent == NULL)
        RBT = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;

    return RBT;
}

// 删除后的调整：x 是缺失一重黑色的结点（可能为 NULL），
// parent 是 x 的父亲（x 为 NULL 时用它定位）
static PtrToNode deleteFixup(PtrToNode RBT, PtrToNode x, PtrToNode parent)
{
    while (x != RBT && IS_BLACK(x))
    {
        if (parent == NULL)
            break;

        if (x == parent->left)
        {
            PtrToNode w = parent->right;   // 兄弟结点

            if (w == NULL)
            {
                x = parent;
                parent = x->parent;
                continue;
            }

            // 情况一：兄弟是红色
            if (IS_RED(w))
            {
                w->color = BLACK;
                parent->color = RED;
                if (parent == RBT)
                    RBT = LeftRot(parent);
                else
                    LeftRot(parent);
                w = parent->right;
            }

            // 情况二：兄弟的两个孩子都是黑色
            if (IS_BLACK(w->left) && IS_BLACK(w->right))
            {
                w->color;
                x = parent;
                parent = x->parent;
            }
            else
            {
                // 情况三：兄弟的右孩子是黑色（左孩子是红色）
                if (IS_BLACK(w->right))
                {
                    if (w->left != NULL)
                        w->left->color = BLACK;
                    w->color = RED;
                    RightRot(w);
                    w = parent->right;
                }

                // 情况四：兄弟的右孩子是红色
                w->color = parent->color;
                parent->color = BLACK;
                if (w->right != NULL)
                    w->right->color = BLACK;
                if (parent == RBT)
                    RBT = LeftRot(parent);
                else
                    LeftRot(parent);

                x = RBT;
                parent = NULL;
                break;
            }
        }
        else
        {
            PtrToNode w = parent->left;    // 兄弟结点

            if (w == NULL)
            {
                x = parent;
                parent = x->parent;
                continue;
            }

            // 情况一：兄弟是红色
            if (IS_RED(w))
            {
                w->color = BLACK;
                parent->color = RED;
                if (parent == RBT)
                    RBT = RightRot(parent);
                else
                    RightRot(parent);
                w = parent->left;
            }

            // 情况二：兄弟的两个孩子都是黑色
            if (IS_BLACK(w->left) && IS_BLACK(w->right))
            {
                w->color = RED;
                x = parent;
                parent = x->parent;
            }
            else
            {
                // 情况三：兄弟的左孩子是黑色（右孩子是红色）
                if (IS_BLACK(w->left))
                {
                    if (w->right != NULL)
                        w->right->color = BLACK;
                    w->color = RED;
                    LeftRot(w);
                    w = parent->left;
                }

                // 情况四：兄弟的左孩子是红色
                w->color = parent->color;
                parent->color = BLACK;
                if (w->left != NULL)
                    w->left->color = BLACK;
                if (parent == RBT)
                    RBT = RightRot(parent);
                else
                    RightRot(parent);

                x = RBT;
                parent = NULL;
                break;
            }
        }
    }

    if (x != NULL)
        x->color = BLACK;

    return RBT;
}

PtrToNode deleteNode(PtrToNode RBT, int key)
{
    // 先找到要删除的结点 z
    PtrToNode z = RBT;
    while (z != NULL)
    {
        if (key < z->data)
            z = z->left;
        else if (key > z->data)
            z = z->right;
        else
            break;
    }

    if (z == NULL)          // 不存在，原树不变
        return RBT;

    PtrToNode y = z;                        // 真正被摘除的结点
    int y_original_color = y->color;
    PtrToNode x;                            // 接替 y 的结点（可能为 NULL）
    PtrToNode xParent;                      // x 的父亲

    if (z->left == NULL)
    {
        x = z->right;
        xParent = z->parent;
        RBT = transplant(RBT, z, z->right);
    }
    else if (z->right == NULL)
    {
        x = z->left;
        xParent = z->parent;
        RBT = transplant(RBT, z, z->left);
    }
    else
    {
        // 有两个孩子：用右子树最小结点 y 替换 z
        y = findmin(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z)
        {
            xParent = y;
            if (x != NULL)
                x->parent = y;
        }
        else
        {
            xParent = y->parent;
            RBT = transplant(RBT, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        RBT = transplant(RBT, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    // 被摘除的是黑色结点，破坏了黑高，需要调整
    if (y_original_color == BLACK)
        RBT = deleteFixup(RBT, x, xParent);

    return RBT;
}