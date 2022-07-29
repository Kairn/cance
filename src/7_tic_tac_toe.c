/*
 * A game of tic-tac-toe that can be played between 2 human players.
 *
 * Concepts:
 * Function constructs.
 * Work with 2-D array.
 * Pass pointers into functions.
 * How to erase printed text to simulate frame refresh.
 * Modular program design.
 */

#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3

int run_game_loop(int player_id);
void draw_board();

int main(int argc, char const *argv[]) {
  printf("Welcome to Tic-Tac-Toe game!\n");
  printf("Use a mono-space console font for the best visual experience.\n\n");

  printf("Rules:\n");
  printf(
      "Player 1 and player 2 will take turns to place their pieces on to the "
      "board.\n");
  printf("Player 1 goes first and uses 'X', and player 2 uses 'O'.\n");
  printf("No piece can be placed on top of another piece.\n");
  printf(
      "The game ends when either player connects three of his/her own pieces "
      "on a straight line (even diagonally).\n");
  printf("The game draws when the whole board is filled without a winner.\n\n");

  /* Create an empty board. */
  char game_state[ROWS][COLS];
  int i = 0;
  int j = 0;
  for (; i < ROWS; ++i) {
    for (; j < COLS; ++j) {
      game_state[i][j] = ' ';
    }
  }

  int next_player_id = 1;

  for (;;) {
    next_player_id = run_game_loop(next_player_id);

    /* Game has ended. */
    if (next_player_id == 0) {
      break;
    }
  }

  printf("\nThanks for playing. Hope to see you again soon.\n");

  return 0;
}

/* Runs a game loop that allows the specified player to make a move and
 * evaluates the result. */
int run_game_loop(int player_id) {
  printf("Player %d's turn.\n", player_id);
  printf("===== Current board =====\n");

  draw_board();
  return 0;
}

void draw_board() {
  char *board_str =
      "\n"
      "      1     2     3   \n"
      "   ------ ----- ------\n"
      "   |     |     |     |\n"
      " A |  %s  |  %s  |  %s  |\n"
      "   |     |     |     |\n"
      "   ------ ----- ------\n"
      "   |     |     |     |\n"
      " B |  %s  |  %s  |  %s  |\n"
      "   |     |     |     |\n"
      "   ------ ----- ------\n"
      "   |     |     |     |\n"
      " C |  %s  |  %s  |  %s  |\n"
      "   |     |     |     |\n"
      "   ------ ----- ------\n";

  printf(board_str, "X", " ", "X", "O", "O", " ", "X", " ", "O");
}
