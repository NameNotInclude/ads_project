#include <stddef.h>
#include <stdlib.h>
typedef struct TreeNode {//  BST (Binary Search Tree) Part 
    int val; struct TreeNode *Left, *Right; // Key value and left/right child pointers
} TreeNode;
TreeNode* InsertNode(TreeNode* Root, int val) {
    if (Root == NULL) { // Boundary: empty spot found, allocate memory and initialize
        Root = (TreeNode*)malloc(sizeof(TreeNode)); Root->val = val; Root->Left = Root->Right = NULL; return Root;
    }
    if (val < Root->val) Root->Left = InsertNode(Root->Left, val);        // Smaller than current, recurse left
    else if (val > Root->val) Root->Right = InsertNode(Root->Right, val); // Greater than current, recurse right
    return Root; // Return current subtree root
}
TreeNode* GetMin(TreeNode* Root) { // Find minimum node in subtree
    while (Root != NULL && Root->Left != NULL) Root = Root->Left; // BST property: leftmost is the minimum
    return Root; 
}
TreeNode* DeleteNode(TreeNode* Root, int val) {
    if (Root == NULL) return NULL; // Boundary: not found
    if (val < Root->val) Root->Left = DeleteNode(Root->Left, val);        // Delete in left subtree
    else if (val > Root->val) Root->Right = DeleteNode(Root->Right, val); // Delete in right subtree
    else { // Target node found
        if (Root->Left == NULL) { TreeNode* tmp = Root->Right; free(Root); return tmp; } // No left child, right child replaces
        if (Root->Right == NULL) { TreeNode* tmp = Root->Left; free(Root); return tmp; } // No right child, left child replaces
        TreeNode* tmp = GetMin(Root->Right); // Both children exist: find min node in right subtree to replace
        Root->val = tmp->val; Root->Right = DeleteNode(Root->Right, tmp->val); // Assign value and delete the min node in right subtree
    }
    return Root;
}
int FindNode(TreeNode* Root, int val) {
    while (Root != NULL) {
        if (Root->val == val) return 1; // Found, return 1
        Root = val < Root->val ? Root->Left : Root->Right; // Ternary operator, effectively saves lines...
    }
    return 0; // Not found after traversal, return 0
}
void FreeTree(TreeNode* Root) {
    if (Root != NULL) { FreeTree(Root->Left); FreeTree(Root->Right); free(Root); } // Post-order traversal to free memory
}
typedef struct SplayTree { int val; struct SplayTree *Left, *Right; } SplayTree;// Splay Tree Part 
SplayTree* Splay(SplayTree* Root, int val) {
    SplayTree Header = {0, NULL, NULL}, *L = &Header, *R = &Header, *tmp; // Header dummy node, L/R build left/right trees
    if (Root == NULL) return NULL;
    for (;;) {
        if (val < Root->val) { // Target in left subtree
            if (Root->Left == NULL) break; // Reached bottom, break
            if (val < Root->Left->val) { // Left-Left case (Zig-Zig), single rotation
                tmp = Root->Left; Root->Left = tmp->Right; tmp->Right = Root; Root = tmp; // Left child rises, original root becomes right subtree
                if (Root->Left == NULL) break;
            }
            R->Left = Root; R = Root; Root = Root->Left; // Hang current root to right tree, continue left
        } else if (val > Root->val) { // Target in right subtree
            if (Root->Right == NULL) break; // Reached bottom, break
            if (val > Root->Right->val) { // Right-Right case (Zag-Zag), single rotation
                tmp = Root->Right; Root->Right = tmp->Left; tmp->Left = Root; Root = tmp; // Right child rises, original root becomes left subtree
                if (Root->Right == NULL) break;
            }
            L->Right = Root; L = Root; Root = Root->Right; // Hang current root to left tree, continue right
        } else break; // Found, break
    }
    L->Right = Root->Left; R->Left = Root->Right; Root->Left = Header.Right; Root->Right = Header.Left;     // Assemble left/right trees: concatenate L, Root, R
    return Root;
}
SplayTree* Splay_Insert(SplayTree* Root, int val) {
    SplayTree* NewNode;
    if (Root == NULL) { // Empty tree, create root directly
        NewNode = (SplayTree*)calloc(1, sizeof(SplayTree)); NewNode->val = val; return NewNode;
    }
    Root = Splay(Root, val); // Splay first, bring closest node to root
    if (Root->val == val) return Root; // Node exists, skip insertion
    NewNode = (SplayTree*)calloc(1, sizeof(SplayTree)); NewNode->val = val;
    if (val < Root->val) { // New value smaller: new node becomes root, old root + right subtree becomes new right subtree
        NewNode->Left = Root->Left; NewNode->Right = Root; Root->Left = NULL;
    } else {               // New value larger: new node becomes root, old root + left subtree becomes new left subtree
        NewNode->Right = Root->Right; NewNode->Left = Root; Root->Right = NULL;
    }
    return NewNode;
}
SplayTree* Splay_Delete(SplayTree* Root, int val) {
    SplayTree *tmp, *R;
    if (Root == NULL) return NULL; // Boundary check
    Root = Splay(Root, val); // Splay target node to root
    if (Root->val != val) return Root; // Not found, return original tree
    tmp = Root; // Temp store root to be freed
    if (Root->Left == NULL) Root = Root->Right; // No left subtree, right subtree replaces
    else { R = Root->Right; Root = Splay(Root->Left, val); Root->Right = R; } // Has left subtree: splay max node of left subtree up, connect old right subtree
    free(tmp); return Root;
}
int Splay_Find(SplayTree** Root, int val) {
    if (Root == NULL || *Root == NULL) return 0; // Null pointer defense
    *Root = Splay(*Root, val); // Double pointer, directly modify passed root pointer
    return (*Root)->val == val; // After splay, check if root is target
}
void Splay_Free(SplayTree* Root) {
    if (Root != NULL) { Splay_Free(Root->Left); Splay_Free(Root->Right); free(Root); } // Recursive post-order free
}