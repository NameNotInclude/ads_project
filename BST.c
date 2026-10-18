#include <stdlib.h>
#include "BST.h"

//分配新节点，初始化内容
static PtrToBSTNode newBSTNode(int val)
{
    PtrToBSTNode node = (PtrToBSTNode)malloc(sizeof(TreeNode));
    if (node == NULL)
        return NULL;

    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

PtrToBSTNode insertBST(PtrToBSTNode root, int val)
{
    //空树，直接返回
    if (root == NULL)
        return newBSTNode(val);

    PtrToBSTNode curr = root;
    while (1)
    {
        //小于当前键，往左子树寻找
        if (val < curr->val)
        {
            if (curr->left == NULL)
            {
                curr->left = newBSTNode(val);
                break;
            }
            curr = curr->left;
        }
        //大于往右子树寻找
        else if (val > curr->val)
        {
            if (curr->right == NULL)
            {
                curr->right = newBSTNode(val);
                break;
            }
            curr = curr->right;
        }
        else
        {
            //已存在，不重复插入
            break;
        }
    }

    return root;
}

PtrToBSTNode deleteBST(PtrToBSTNode root, int val)
{
    PtrToBSTNode parent = NULL;
    PtrToBSTNode curr = root;

    //查找目标结点，同时记录其父结点
    while (curr != NULL && curr->val != val)
    {
        parent = curr;
        curr = (val < curr->val) ? curr->left : curr->right;
    }

    if (curr == NULL)
        return root;

    //有两个孩子时，用右子树最小结点替换目标结点
    if (curr->left != NULL && curr->right != NULL)
    {
        PtrToBSTNode successorParent = curr;
        PtrToBSTNode succ = curr->right;
        while (succ->left != NULL)
        {
            successorParent = succ;
            succ = succ->left;
        }

        curr->val = succ->val;
        parent = successorParent;
        curr = succ;
    }

    //被删除结点至多有一个孩子，用该孩子接替它的位置
    PtrToBSTNode child = (curr->left != NULL) ? curr->left : curr->right;

    //删除根，更新根节点
    if (parent == NULL)
    {
        root = child;
    }

    //接回树种，根据键值的大小关系
    else if (parent->left == curr)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }

    free(curr);
    return root;
}

int findBST(PtrToBSTNode root, int val)
{
    while (root != NULL)
    {
        if (val == root->val)
            return 1;

        root = (val < root->val) ? root->left : root->right;
    }

    return 0;
}
