#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "your_functions.h"

// Merge sort algorithm
// Arguments:
//  (1) Pointer to start of array to sort
//  (2) Pointer to start of temporary array
//  (3) Number of elements in array
// Return value: None
void mergeSort(int *array_start, int *temp_array_start, int array_size)
{
  printf("Using merge sort algorithm...\n");

  // Solution from: http://p2p.wrox.com/visual-c/66348-merge-sort-c-source-code.html

  mergeSort_sort(array_start, temp_array_start, 0, array_size - 1);

  return;
}

void mergeSort_sort(int array_start[], int temp[], int left, int right)
{
  int mid;
 
  if (right > left)
  {
    mid = (right + left) / 2;
    mergeSort_sort(array_start, temp, left, mid);
    mergeSort_sort(array_start, temp, mid+1, right);
 
    mergeSort_merge(array_start, temp, left, mid+1, right);
  }
}
 
void mergeSort_merge(int array_start[], int temp[], int left, int mid, int right)
{
  int i, left_end, num_elements, tmp_pos;
 
  left_end = mid - 1;
  tmp_pos = left;
  num_elements = right - left + 1;
 
  while ((left <= left_end) && (mid <= right))
  {
    if (array_start[left] <= array_start[mid])
    {
      temp[tmp_pos] = array_start[left];
      tmp_pos = tmp_pos + 1;
      left = left +1;
    }
    else
    {
      temp[tmp_pos] = array_start[mid];
      tmp_pos = tmp_pos + 1;
      mid = mid + 1;
    }
  }
 
  while (left <= left_end)
  {
    temp[tmp_pos] = array_start[left];
    left = left + 1;
    tmp_pos = tmp_pos + 1;
  }
  while (mid <= right)
  {
    temp[tmp_pos] = array_start[mid];
    mid = mid + 1;
    tmp_pos = tmp_pos + 1;
  }
 
  for (i=0; i < num_elements; i++)
  {
    // JAS: Used to be <= num_elements...
    array_start[right] = temp[right];
    right = right - 1;
  }
}


// Tree sort algorithm
//Requires the following steps:
//1. Construct a Binary Tree using the array elements. If the current element is less than
//the node, place the element on the left branch, else place it on the right branch. See BTreeNode structure in your_functions.h.
//2. Once binary tree is constructed, perform in-order traversal of the binary tree (HINT: use recursion).
//FILL in the functions: inorder, insert_element, and tree_sort for sorting.

void inorder(struct BTreeNode *node,int *array)
{
	//Recursive In-order traversal: leftchild, element, rightchild

}

void insert_element(struct BTreeNode **node, int element)
{

}

void free_btree(struct BTreeNode **node)
{

}

void tree_sort(int *array, int size)
{

//1. Construct the binary tree using elements in array
//2. Traverse the binary tree in-order and update the array
//3. Free the binary tree
}


