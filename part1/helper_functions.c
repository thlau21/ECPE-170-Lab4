#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "helper_functions.h"

// Initialize array with random numbers in range from 0 to RAND_MAX
// Arguments:
//  (1) Pointer to start of array
//  (2) Number of elements in array
// Return value: None
void initArray(int *array_start, int array_size)
{
  printf("Initializing %i elements in array...\n", array_size);

  for(int i=0; i<array_size; i++)
    {
      array_start[i] = rand();
    }

  return;
}


// Bubble sort algorithm
// Arguments:
//  (1) Pointer to start of array
//  (2) Number of elements in array
// Return value: None
void bubbleSort(int *array_start, int array_size)
{
  printf("Using bubble sort algorithm...\n");

  int temp;
 
  for (int i = (array_size - 1); i > 0; i--)
    {
      for (int j = 1; j <= i; j++)
	{
	  if (array_start[j-1] > array_start[j])
	    {
	      temp = array_start[j-1];
	      array_start[j-1] = array_start[j];
	      array_start[j] = temp;
	    }
	}
    }
  
  
  return;
}


// Test if array is sorted in ascending order
// Arguments:
//  (1) Pointer to start of array
//  (2) Number of elements in array
// Return value: True (if sorted), false otherwise
bool verifySort(int *array_start, int array_size)
{
  printf("Verifying array sorting...\n");

  if(array_size == 1)
    return true;  // Array with 1 element is always sorted
  else if(array_size <= 0)
    return false;  // Invalid array
  else
    {
      for(int i=1; i<array_size; i++)
	{
	  // Starting from element 1 (not 0), compare with preceeding
	  if(array_start[i-1] > array_start[i])
	    return false; // Found 1 example out of order - stop searching
	}

      return true;
    }
}
