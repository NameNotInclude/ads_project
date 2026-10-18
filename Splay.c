#include <stdlib.h>
#include "Splay.h"


//分配新结点，并初始化内容
static PtrToSplayNode newSplayNode(int val)
{
    PtrToSplayNode node = (PtrToSplayNode)malloc(sizeof(SplayNode));
    if (node == NULL)
        return NULL;

    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 左旋：返回子树的新根
static PtrToSplayNode LeftRot(PtrToSplayNode root)
{
    PtrToSplayNode right = root->right;
    PtrToSplayNode tmp = right->left;

    root->right = tmp;
    right->left = root;

    return right;
}

//右旋：返回子树的新根
static PtrToSplayNode RightRot(PtrToSplayNode root)
{
    PtrToSplayNode left = root->left;
    PtrToSplayNode tmp = left->right;

    root->left = tmp;
    left->right = root;

    return left;
}

/*
 * 把 val 所在结点（找不到时是路径上最接近 val 的结点）伸展到根。
 * 返回新的树根；树为空时返回 NULL。
 */
PtrToSplayNode splay(PtrToSplayNode root, int val)
{
    SplayNode header;          //栈上哨兵，只用到 left / right
    PtrToSplayNode leftTree;   //键 < val 的结点组成的临时树（用 ->right 串联）
    PtrToSplayNode rightTree;  //键 > val 的结点组成的临时树（用 ->left  串联）

    if (root == NULL)
        return NULL;

    header.left = NULL;
    header.right = NULL;
    leftTree = &header;
    rightTree = &header;

    while (1)
    {
        if (val < root->val)
        {
            if (root->left == NULL)
                break;

            // Zig-Zig：自顶向下只需对当前根右旋一次 
            if (val < root->left->val)
            {
                root = RightRot(root);
                if (root->left == NULL)
                    break;
            }

            // 把当前根挂到右临时树上，继续向左走 
            rightTree->left = root;
            rightTree = root;
            root = root->left;
        }
        else if (val > root->val)
        {
            if (root->right == NULL)
                break;

            // Zag-Zag：自顶向下只需对当前根左旋一次 
            if (val > root->right->val)
            {
                root = LeftRot(root);
                if (root->right == NULL)
                    break;
            }

            // 把当前根挂到左临时树上，继续向右走 
            leftTree->right = root;
            leftTree = root;
            root = root->right;
        }
        else
        {
            //找到
            break;             
        }
    }

    // 组装：左临时树 + 根 + 右临时树（两条链的尾指针顺便切断临时指针）
    leftTree->right = root->left;
    rightTree->left = root->right;
    root->left = header.right;
    root->right = header.left;

    return root;
}

PtrToSplayNode insertSplay(PtrToSplayNode root, int val)
{
    PtrToSplayNode node;

    if (root == NULL)
        return newSplayNode(val);

    root = splay(root, val);
    //已存在，不重复插入
    if (root->val == val)      
        return root;

    node = newSplayNode(val);
    if (node == NULL)
        return root;

    if (val < root->val)
    {
        // 新结点成为根，原根及其右子树作为新根的右子树
        node->left = root->left;
        node->right = root;
        root->left = NULL;
    }
    else
    {
        // 新结点成为根，原根及其左子树作为新根的左子树 
        node->right = root->right;
        node->left = root;
        root->right = NULL;
    }

    return node;
}

PtrToSplayNode deleteSplay(PtrToSplayNode root, int val)
{
    PtrToSplayNode target;
    PtrToSplayNode rightSubtree;

    if (root == NULL)
        return NULL;

    root = splay(root, val);
    //不存在，原树不变
    if (root->val != val)      
        return root;

    target = root;

    if (root->left == NULL)
    {
        root = root->right;
    }
    else
    {
        //先把左子树中的最大结点伸展到左子树根，再挂上原右子树
        rightSubtree = root->right;
        root = splay(root->left, val);
        root->right = rightSubtree;
    }

    free(target);
    return root;
}

int findSplay(PtrToSplayNode* root, int val)
{
    if (root == NULL || *root == NULL)
        return 0;

    *root = splay(*root, val);
    return (*root)->val == val;
}
