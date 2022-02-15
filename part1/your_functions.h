#ifndef YOUR_FUNCTIONS_H
#define YOUR_FUNCTIONS_H

//Students: Binary Tree structure that your functions will use to create a binary tree

struct BTreeNode
{
struct BTreeNode *leftnode;
struct BTreeNode *rightnode;
int element;
};

// Students: Add the required functions for tree sort here...
void inorder(struct BTreeNode *node, int *array);

void insert_element(struct BTreeNode **node, int element);

void tree_sort(int *array, int size);

void free_btree(struct BTreeNode **node);

void mergeSort(int *array_start, int *temp_array_start, int array_size);
void mergeSort_sort(int *array_start, int *temp_array_start, int left, int right);
void mergeSort_merge(int *array_start, int *temp_array_start, int left, int mid, int right);


#endif // YOUR_FUNCTIONS_H

