#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

TreeNode *TreeAddChildValue(TreeNode *root, void *value, size_t value_size) {
    TreeNode *child = malloc(sizeof(TreeNode));
    memset(child, 0, sizeof(*child));

    child->value = value;
    child->value_size = value_size;

    if (root) {
        child->parent = root;
        DArrayAppend(&root->children, child);
    }

    return child;
}

TreeNode *TreeAddChildNode(TreeNode *root, TreeNode *node) {
    if (!root) {
        return node;
    }

    if (!node) {
        return NULL;
    }

    node->parent = root;
    DArrayAppend(&root->children, node);
    return node;
}

void TreeFree(TreeNode *root) {
    if (!root) {
        return;
    }

    for (size_t i = 0; i < root->children.count; ++i) {
        TreeFree(root->children.items[i]);
    }

    DArrayFree(&root->children);
    free(root->value);
    free(root);
}

// int main() {
//     int *values[6];
//     for (int i = 0; i < 6; ++i) {
//         values[i] = malloc(sizeof(int));
//         *values[i] = i;
//     }

//     TreeNode *root = TreeAddChildValue(NULL, values[0], sizeof(*values));
//     TreeAddChildValue(root, values[1], sizeof(*values));
//     TreeAddChildValue(root, values[2], sizeof(*values));
//     TreeAddChildValue(root, values[3], sizeof(*values));
//     TreeAddChildValue(root, values[4], sizeof(*values));
//     TreeAddChildValue(root, values[5], sizeof(*values));

//     printf("root: %d\n", *(int *)root->value);
//     for (size_t i = 0; i < root->children.count; ++i) {
//         printf("         child: %d\n", *(int *)root->children.items[i]->value);
//     }

//     TreeFree(root);

//     printf("no segfault yay!\n");

//     return 0;
// }