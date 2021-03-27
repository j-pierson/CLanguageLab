/*
This file sorts any group of numbers presented as command arguments as long as
there are 32 or fewer number arguments. It defaults to a minimum element sort,
but the optional command argument "-b" will make it sort using a bubble sort.
The only other optional command argument is "-q", and this will suppress the
printing of the sorted list.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#define MAXVALUES 32

void minimum_entry_sort(int nums[], int argc);
void bubble_sort(int nums[], int len);

int main(int argc, char *argv[])
{
  int i, b, q, count, nums[MAXVALUES];
  /* Checking to make sure a commandline argument was passed */
  if (argc == 1)
  {
    fprintf(stderr, "usage: %s [-b] [-q] number1 [number2 ... ]"
            " (maximum 32 numbers)\n", argv[0]);
    exit(1);
  }
  b = 0;
  q = 0;
  count = 0;
  /*
  Looping through each element and adding to an array if int, or noting
  whether -b or -q has been read using b and q variables
  */
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-b") == 0)
    {
      b = 1;
    }
    else if (strcmp(argv[i], "-q") == 0)
    {
      q = 1;
    }
    else
    {
      nums[count] = atoi(argv[i]);
      count += 1;
      if (count > 32)
      {
        /* return an error if too many ints passed */
        fprintf(stderr, "usage: %s [-b] [-q] number1 [number2 ... ]"
                " (maximum 32 numbers)\n", argv[0]);
        exit(1);
      }
    }
  }
  /* return an error if no ints passed */
  if (count == 0)
  {
    fprintf(stderr, "usage: %s [-b] [-q] number1 [number2 ... ]"
            " (maximum 32 numbers)\n", argv[0]);
    exit(1);
  }
  /* minimum_entry_sort if no -b args */
  if (b == 0)
  {
    minimum_entry_sort(nums, count);
  }
  /* bubble_sort if -b arg found */
  else if (b == 1)
  {
    bubble_sort(nums, count);
  }
  /* print results of sorting only if no -q args */
  if (q == 0)
  {
    for (i = 0; i < count; i++)
    {
      fprintf(stdout, "%d\n", nums[i]);
    }
  }
  return 0;
}

/*
minimum_entry_sort is a function that sorts an array of ints from least
to greatest using the minimum entry sort method, and then it will print the
integers to the terminal in this order.

Arguments:

nums[] --> the integer array containing the numbers to be sorted
len --> the number of integers in the integer array

Return:

void

Note: The minimum entry sort method is done by swapping the least member of an
      array with the first member, then swapping the least member of the rest
      of the array with the second member, etc.
*/

void minimum_entry_sort(int nums[], int len)
{
  /*
  Variables:

  i --> an index varible used to loop through each position in the array

  n --> an index varibale used to compare every position with position i

  temp --> a temporary storage necessary to swap two positions in an array

  min --> a marker that denotes the minimum index in the arry
  */
  int i, n, temp, min;
  /*
  the function remembers the smallest index, then loops through and swaps
  positions every time a smalller number is read
  */
  for (i = 0; i < len; i++)
  {
    min = i;
    for (n = i; n < len; n++)
    {
      if (nums[min] > nums[n])
      {
        min = n;
      }
    }
    temp = nums[i];
    nums[i] = nums[min];
    nums[min] = temp;
  }
  /* assert the sort has worked properly */
  for (i = 1; i < len; i++)
  {
    assert(nums[i] >= nums[i-1]);
  }
}

/*
print_bubble_sort is a function that sorts an array of ints from least
to greatest using the bubble sort method, and then it will print the
integers to the terminal in this order.

Arguments:

nums[] --> the integer array containing the numbers to be sorted

len --> the number of integers in the integer array

Return:

void

*/

void bubble_sort(int nums[], int len)
{
  /*
  Variables:

  i --> an index varible used to run the bubble sort on each pair at least len
        number of times
  n --> an index varibale used to compare every adjacent pair of values in the
        array
  temp --> a temporary storage necessary to swap two positions in an array
  */
  int i, n, temp;
  /*
  the function compares adjacent entries and swaps them if the right one is
  larger than the left one. This is done len number of times to ensure sorting
  */
  for (i = 0; i < len - 1; i++)
  {
    for (n = 0; n < len - 1; n++)
    {
      if (nums[n] > nums[n + 1])
      {
        temp = nums[n];
        nums[n] = nums[n + 1];
        nums[n + 1] = temp;
      }
    }
  }
  /* assert sorting has been done properly */
  for (i = 1; i < len; i++)
  {
    assert(nums[i] >= nums[i-1]);
  }
}
