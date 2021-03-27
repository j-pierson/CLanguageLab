/*
This file creates a one dimensional cellular automaton that will be printed to
the terminal using the transition rule:

1. If the current cell is empty, and one of the two adjacent cells is full
   (but not both), set the current cell to be full.

2. Otherwise set the current cell to be empty.

Full cells are indicated by '*' symbols and empty cells are indicated by '-'
symbols
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "memcheck.h"

void initialize_board(int board[], int len);
void board_update(int board[], int newboard[], int len);
void print_cells(int board[], int len);

int main(int argc, char *argv[])
{
  int i, len, gens;
  int *board, *newboard;;
  /* Checking to make sure a commandline argument was passed */
  if (argc < 3)
  {
    fprintf(stderr, "usage: %s population generations\n", argv[0]);
    exit(1);
  }
  len = atoi(argv[1]);
  gens = atoi(argv[2]);
  srand(time(0));
  board = (int *) calloc(len, sizeof(int));
  /* Check that the calloc call succeeded. */
  if (board == NULL)
  {
    fprintf(stderr, "Error! Memory allocation failed!\n");
    exit(1);  /* abort the program */
  }
  /* create the initial board */
  initialize_board(board, len);
  for (i = 0; i < gens; i++)
  {
    /* turning a board into the visual cells, printing the cells, and then
    updating the board for each generation */
    newboard = (int *) calloc(len, sizeof(int));
    /* Check that the calloc call succeeded. */
    if (newboard == NULL)
    {
        fprintf(stderr, "Error! Memory allocation failed!\n");
        exit(1);  /* abort the program */
    }
    print_cells(board, len);
    board_update(board, newboard, len);
    free(newboard);
  }
  free(board);
  print_memory_leaks();
  return 0;
}

/*
initialize_board is a function that creates the first board. It uses a random
variable to give each cell a 50% chance of being full or empty at the start.
*/
void initialize_board(int board[], int len)
{
  int i, r;
  board[0] = 0;
  board[len - 1] = 0;
  for (i = 1; i < len - 1; i++)
  {
    /* for each cell, a 0 or 1 is chosen randomly */
    r = rand() % 2;
    board[i] = r;
  }
}

/*
board_update is a function that creates a new board for the next generation
based on the transition rule.
*/
void board_update(int board[], int newboard[], int len)
{
  int i;
  for (i = 1; i < len - 1; i++)
  {
    /* check that cell i is empty */
    if (board[i] == 0)
    {
      /* check that only one of cell i's neighbors is full */
      if (board[i] + board[i + 1] + board[i - 1] == 1)
      {
        newboard[i] = 1;
      }
      else
      /* otherwise, cell i is empty */
      {
        newboard[i] = 0;
      }
    }
    else
    /* if cell i is full, it becomes empty */
    {
      newboard[i] = 0;
    }
  }
  /* we modify our acting board to match the one used to store the new gen */
  for (i = 0; i < len; i++)
  {
    board[i] = newboard[i];
  }
}

/*
create_cells is a function that turns a board into a character array that
represents the visual representation of the board_update
*/
void print_cells(int board[], int len)
{
  int i;
  for (i = 0; i < len; i++)
  {
    /* a 0 in board --> a - in cells; a 1 in board --> a * in cells */
    if (board[i] == 0)
    {
      printf("-");
    }
    else
    {
      printf("*");
    }
  }
  printf("\n");
}
