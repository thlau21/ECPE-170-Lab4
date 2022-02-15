// Compile this program:
//  unix>  make
// Run this program:
//  unix>  ./sorting_program bubble
//  unix>  ./sorting_program quick
//  unix>  ./sorting_program merge

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>   // Use bool as a datatype - requires newer C99 standard!
#include <string.h>    // For string comparison

#include "helper_functions.h"
#include "your_functions.h"

#define ARRAY_SIZE 100000

int main(int argc, char *argv[])
{
  int my_array[ARRAY_SIZE];
  int *temp_array = NULL;
  bool sorted = false;
 
  if( argc != 2)
    {
      printf("Program usage: %s sortname\n", argv[0]);
      return 1;
    }

  // Fill array with random numbers
  initArray(my_array, ARRAY_SIZE);

  // Sort array by chosen algorithm
  if(strcmp(argv[1], "bubble") == 0)
    bubbleSort(my_array, ARRAY_SIZE);
  else if(strcmp(argv[1], "tree") == 0)
    tree_sort(my_array,ARRAY_SIZE);
  else if(strcmp(argv[1], "merge") == 0)
    {
      // Merge sort needs a second (temporary) array
      // Dynamically allocate this
      temp_array = calloc(ARRAY_SIZE, sizeof(int));
 
      mergeSort(my_array, temp_array, ARRAY_SIZE);

      free(temp_array); // Release dynamic memory after use
    }
  else
    {
      printf("Invalid sort algorithm. Must specifiy 'bubble',  'tree', or 'merge'\n");
      return 1;
    }

  // Test if array is sorted correctly
  sorted = verifySort(my_array, ARRAY_SIZE);

  if(sorted)
    printf("Congrats! Array is sorted correctly\n");
  else
    printf("*** Error: Array sort algorithm fails verification test ***\n");

  return 0;
}
