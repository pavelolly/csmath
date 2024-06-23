#include <stdlib.h>
#include <stdbool.h>

#include "array.h"


#define DEREF_AS(type, ptr) (*(type *)(ptr))

struct TreeNode;

INSTANTIATE_DARRAY(TreeNodes, struct TreeNode *);

typedef struct TreeNode {
    void *value;
    int hint;
    size_t value_size;

    struct TreeNode *parent;
    TreeNodes children;
} TreeNode;

/*!
* adds child to tree node or creates root if first argument is NULL
* @param root tree node
* @param value pointer to heap allocated value returned by malloc
* @param value_size sizeof(*value)
* @return pointer to newly created child or pointer to root if node is NULL
*/
TreeNode *TreeAddChildValue(TreeNode *root, void *value, size_t value_size);

TreeNode *TreeAddChildNode(TreeNode *root, TreeNode *node);

void TreeFree(TreeNode *node);