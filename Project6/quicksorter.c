/*
This file sorts any group of numbers presented as command arguments. It uses a
quicksort. The only optional command argument is "-q", and this will suppress
the printing of the sorted list.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "memcheck.h"

node *make_quicksort(node *list);

int main(int argc, char *argv[])
{
  int quiet, i, count;
  node *list, *sorted_list;
  /* Checking to make sure a commandline argument was passed */
  if (argc == 1)
  {
    fprintf(stderr, "usage: %s [-q] number1 [number2 ... ]\n", argv[0]);
    exit(1);
  }
  quiet = 0;
  count = 0;
  list = NULL;
  /*
  Looping through each element and adding to linked list if int, or noting
  whether -q has been read using q variable
  */
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-q") == 0)
    {
      quiet = 1;
    }
    else
    {
      list = create_node(atoi(argv[i]), list);
      count += 1;
    }
  }
  /* return an error if no ints passed */
  if (count == 0)
  {
    fprintf(stderr, "usage: %s [-q] number1 [number2 ... ]\n", argv[0]);
    exit(1);
  }
  sorted_list = make_quicksort(list);
  if (quiet == 0)
  {
    print_list(sorted_list);
  }
  free_list(list);
  free_list(sorted_list);
  print_memory_leaks();
  return 0;
}


/*
make_quicksort is a function that accepts a linked list as an input and will
return a linked list with quicksorted content of the input list via recursion
*/
node *make_quicksort(node *list)
{
  node *sorted_list, *lesser_list, *greater_list, *item, *temp;
  sorted_list = NULL;
  lesser_list = NULL;
  greater_list = NULL;
  sorted_list = create_node(list->data, sorted_list);
  /* make sure that the list is not empty */
  if (list->next == NULL)
  {
    return sorted_list;
  }
  /*
  loop through the list, and add element to two separate lists depending once
  whether the element is larger or smaller than the first element
  */
  list = list->next;
  for (item = list; item != NULL; item = item->next)
  {
    if (item->data >= sorted_list->data)
    {
      greater_list = create_node(item->data, greater_list);
    }
    if (item->data < sorted_list->data)
    {
      lesser_list = create_node(item->data, lesser_list);
    }
  }
  /* quicksort each of the two lists recursively */
  if (greater_list != NULL)
  {
    temp = make_quicksort(greater_list);
    free_list(greater_list);
    greater_list = temp;
  }
  if (lesser_list != NULL)
  {
    temp = make_quicksort(lesser_list);
    free_list(lesser_list);
    lesser_list = temp;
  }
  /* add the lists together to get a complete sorted list */
  temp = append_lists(sorted_list, greater_list);
  free_list(sorted_list);
  sorted_list = temp;
  temp = append_lists(lesser_list, sorted_list);
  free_list(sorted_list);
  sorted_list = temp;
  free_list(greater_list);
  free_list(lesser_list);
  /* return a new quicksorted linked list */
  return sorted_list;
}
