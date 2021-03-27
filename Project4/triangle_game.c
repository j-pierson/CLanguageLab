/*
This file presents the user with a board for the game Triangle. The user is
asked to place pegs onto the board in any arrangement, and the program will
check to see if the puzzle created by the user is solvable. A puzzle is solvable
if there exists a way to reduce the board to only one peg by the rules of
the game.
*/

#include <stdio.h>
#include "triangle_routines.h"
#define NMOVES 36
#define BOARDSIZE 15

int board_solve_check(int board[]);
int npegs(int board[]);
int valid_move(int board[], int move[]);
void make_move(int board[], int move[]);
void unmake_move(int board[], int move[]);


/*
moves is a global variable that stores all possble moves in the game as an array
of triples as such:

{peg being moved, peg being jumped, empty slot peg is jumping to}

*/
int moves[NMOVES][3] =
{
  {0, 1, 3},
  {0, 2, 5},
  {1, 3, 6},
  {1, 4, 8},
  {2, 4, 7},
  {2, 5, 9},
  {3, 1, 0},
  {3, 4, 5},
  {3, 6, 10},
  {3, 7, 12},
  {4, 7, 11},
  {4, 8, 13},
  {5, 2, 0},
  {5, 4, 3},
  {5, 8, 12},
  {5, 9, 14},
  {6, 3, 1},
  {6, 7, 8},
  {7, 4, 2},
  {7, 8, 9},
  {8, 4, 1},
  {8, 7, 6},
  {9, 5, 2},
  {9, 8, 7},
  {10, 6, 3},
  {10, 11, 12},
  {11, 7, 4},
  {11, 12, 13},
  {12, 11, 10},
  {12, 7, 3},
  {12, 8, 5},
  {12, 13, 14},
  {13, 12, 11},
  {13, 8, 4},
  {14, 9, 5},
  {14, 13, 12}
};

int main(void)
{
    int board[BOARDSIZE];
    triangle_input(board);
    printf("Here is the board you entered:\n");
    triangle_print(board);
    if (!board_solve_check(board))
    {
      printf("This board is not solvable\n");
    }
    return 0;
}

/*
board_solve_check is a function that checks whether a given board configuration
is solvable by making all valid moves until a solution has been found or all
possible moves have been taken.

Arguments:

board --> a board configuration represented by a 15 integer array

Return:

int 0 --> there exists a sequence of moves that yields a solution

int 1 --> there exists no sequences of moves that yield a solution
*/
int board_solve_check(int board[])
{
  /*
  Variables:

  i --> in iterator

  pegs --> stores the number of pegs on the board
  */
  int pegs, i;
  pegs = npegs(board);
  /* We know if there is only one peg, we have reached a solution */
  if (pegs == 1)
  {
    printf("Here are the boards that lead to solution:\n");
    triangle_print(board);
    return 1;
  }
  /* Otherwise, we have to make all valid moves to find a solution */
  else
  {
    for (i = 0; i < NMOVES; i++)
    {
      if (valid_move(board, moves[i]))
      {
        /*
        When a move is found, it is made. Then, we recursively call the
        funciton to check for more possible moves.
        */
        make_move(board, moves[i]);
        if (board_solve_check(board))
        {
          /* If our recursion yields a solution, then we print the steps */
          unmake_move(board, moves[i]);
          triangle_print(board);
          return 1;
        }
        else
        {
          unmake_move(board, moves[i]);
        }
      }
    }
  }
  /*
  At this point, either there were no valid moves, or all valid moves did
  not reach solution
  */
  return 0;
}

/*
npegs is a function that counts the number of pegs on a board input

Arguments:

board --> a board configuration represented by a 15 integer array

Return:

count --> the number of pegs on the board input
*/
int npegs(int board[])
{
  /*
  Variables:

  i --> in iterator

  count --> stores the number of pegs on the board
  */
  int i, count;
  count = 0;
  /*
  The function loops through the board array and counts the pegs, which are
  represented by 1 characters
  */
  for (i = 0; i < BOARDSIZE; i++)
  {
    if (board[i] == 1)
    {
      count++;
    }
  }
  return count;
}

/*
valid_move is a function that counts the number of pegs on a board input

Arguments:

board --> a board configuration represented by a 15 integer array

move[] --> a specific triple from the global moves array

Return:

1 --> the move is possible given the board argument

0 --> the move is not possible given the board argument
*/
int valid_move(int board[], int move[])
{
  /*
  A move is only possible if there is a peg to be moved, a peg to be jumped,
  and an empty space to jump into
  */
  return board[move[0]] && board[move[1]] && !board[move[2]];
}

/*
valid_move is a function that makes a move on an argument board

Arguments:

board --> a board configuration represented by a 15 integer array

move[] --> a specific triple from the global moves array

Return:

void
*/
void make_move(int board[], int move[])
{
  /*
  A move pulls a peg from a spot, pull the peg it jumps over, and places it
  in a new spot.
  */
  board[move[0]] = 0;
  board[move[1]] = 0;
  board[move[2]] = 1;
}

/*
valid_move is a function that makes a move on an argument board

Arguments:

board --> a board configuration represented by a 15 integer array

move[] --> a specific triple from the global moves array

Return:

void
*/
void unmake_move(int board[], int move[])
{
  /*
  A move is undone by putting a peg back where it was before the move and then
  replacing the peg that was jumped
  */
  board[move[0]] = 1;
  board[move[1]] = 1;
  board[move[2]] = 0;
}
