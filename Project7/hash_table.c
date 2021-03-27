/*
 * CS 11, C Track, lab 7
 *
 * FILE: hash_table.c
 *
 *       Implementation of the hash table functionality.
 *
 */

/*
 * Include the declaration of the hash table data structures
 * and the function prototypes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "memcheck.h"


/*** Hash function. ***/

int hash(char *s)
{
  int value, len, i;
  len = strlen(s);
  value = 0;
  for (i = 0; i < len; i++)
  {
    value += s[i];
  }
  value = value % NSLOTS;
  return value;
}


/*** Linked list utilities. ***/

/* Create a single node. */
node *create_node(char *key, int value)
{
  node *result = (node *)malloc(sizeof(node));

  if (result == NULL)
  {
      fprintf(stderr, "Fatal error: out of memory. "
              "Terminating program.\n");
      exit(1);
  }

  result->key = key;  /* Fill in the new node with the given value. */
  result->value = value;
  result->next = NULL;

  return result;
}


/* Free all the nodes of a linked list. */
void free_list(node *list)
{
  node *n;     /* a single node of the list */

  while (list != NULL)
  {
      n = list;
      list = list->next;

      /*
       * 'n' now points to the first element of the list, while
       * 'list' now points to everything but the first element.
       * Since nothing points to 'n', it can be freed.
       */

      free(n->key);
      free(n);
  }
}


/*** Hash table utilities. ***/

/* Create a new hash table. */
hash_table *create_hash_table()
{
  hash_table *ht;
  ht = malloc(sizeof(hash_table));

  if (ht == NULL)
  {
    fprintf(stderr, "Fatal error: out of memory. "
            "Terminating program.\n");
    exit(1);
  }

  ht->slot = (node **)malloc(NSLOTS * sizeof(node *));

  if (ht->slot == NULL)
  {
    fprintf(stderr, "Fatal error: out of memory. "
            "Terminating program.\n");
    exit(1);
  }
  return ht;
}


/* Free a hash table. */
void free_hash_table(hash_table *ht)
{
  int i;

  if (ht == NULL)
  {
    return;
  }

  for (i = 0; i < NSLOTS; i++)
  {
    free_list(ht->slot[i]);
  }
  free(ht->slot);
  free(ht);
}


/*
 * Look for a key in the hash table.  Return 0 if not found.
 * If it is found return the associated value.
 */
int get_value(hash_table *ht, char *key)
{
  node *list;
  int num;

  num = hash(key);
  list = ht->slot[num];

  while (list != NULL)
  {
    if (strcmp(list->key, key) == 0)
    {
      break;
    }
    list = list->next;
  }

  if (list == NULL)
  {
    return 0;
  }
  return list->value;
}


/*
 * Set the value stored at a key.  If the key is not in the table,
 * create a new node and set the value to 'value'.  Note that this
 * function alters the hash table that was passed to it.
 */
void set_value(hash_table *ht, char *key, int value)
{
  node *n, *list;
  int num;

  n = create_node(key, value);
  num = hash(n->key);
  list = ht->slot[num];

  while (list != NULL)
  {
    if (strcmp(list->key, n->key) == 0)
    {
      list->value = n->value;
      free(n->key);
      free(n);
      return;
    }
    list = list->next;
  }
  if (list == NULL)
  {
    n->next = ht->slot[num];
    ht->slot[num] = n;
  }
}


/* Print out the contents of the hash table as key/value pairs. */
void print_hash_table(hash_table *ht)
{
  node *list;
  int i;

  if (ht == NULL)
  {
    return;
  }

  for (i = 0; i < NSLOTS; i++)
  {
    list = ht->slot[i];
    while (list != NULL)
    {
      printf("%s %d\n", list->key, list->value);
      list = list->next;
    }
  }
}
