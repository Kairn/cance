/*
 * A game of tic-tac-toe that can be played between 2 human players.
 *
 * Concepts:
 * Function constructs.
 * Pass arrays as function arguments using pointers.
 * How to erase printed text to simulate frame refresh.
 * Modular program design.
 */

#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3

/* How many retries has a player used. This will determine how many lines to
 * clear after each move. */
int input_retry_count = 0;

int run_game_loop(int player_id, unsigned char *game_state);
void draw_board(unsigned char *game_state);
char *state_to_str(unsigned char state);
int read_player_input(unsigned char *game_state);
int evaluate_board(int player_id, unsigned char *game_state);
int evaluate_line(int state_1, int state_2, int state_3);
void erase_board();
void erase_lines(int num_lines);

int main(int argc, char const *argv[]) {
  printf("Welcome to Tic-Tac-Toe game!\n");
  printf("Use a mono-space console font for the best visual experience.\n\n");

  printf("Make a move using a number between 1 and 9:\n");
  printf("1 -> A1, 2 -> A2, 3 -> A3\n");
  printf("4 -> B1, 5 -> B2, 6 -> B3\n");
  printf("7 -> C1, 8 -> C2, 9 -> C3\n\n");

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
  unsigned char game_state[ROWS * COLS];
  int i = 0;
  for (; i < ROWS * COLS; ++i) {
    game_state[i] = 0;
  }

  int next_player_id = 1;

  for (;;) {
    next_player_id = run_game_loop(next_player_id, game_state);

    /* Game has ended. */
    if (next_player_id == 255) {
      printf(
          "\nWe have encountered a problem with I/O. The game must end now.\n");
      return 1;
    } else if (next_player_id < 0) {
      printf("      🎉 🎉 🎉 🎉 🎉\n");
      printf("      ✨ ✨ ✨ ✨ ✨\n");
      draw_board(game_state);
      printf("Player %d has won, congratulations!\n", next_player_id * -1);
      break;
    } else if (next_player_id == 0) {
      printf("      🌛 🌛 🌛 🌛 🌛\n");
      printf("      🌜 🌜 🌜 🌜 🌜\n");
      draw_board(game_state);
      printf("The game has ended in draw.\n");
      break;
    }
  }

  printf("Thanks for playing. Hope to see you again soon.\n");

  return 0;
}

/* Runs a game loop that allows the specified player to make a move and
 * evaluates the result. */
int run_game_loop(int player_id, unsigned char *game_state) {
  printf("Player %d's turn.\n", player_id);
  printf("===== Current board =====\n");

  draw_board(game_state);

  int next_move = read_player_input(game_state);
  if (next_move == 255) {
    return 255;
  } else if (player_id == 1) {
    game_state[next_move] = 1;
  } else if (player_id == 2) {
    game_state[next_move] = 2;
  }

  erase_board();

  return evaluate_board(player_id, game_state);
}

/* Renders the board based on the current game state. */
void draw_board(unsigned char *game_state) {
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
      "   ------ ----- ------\n\n";

  printf(board_str, state_to_str(game_state[0]), state_to_str(game_state[1]),
         state_to_str(game_state[2]), state_to_str(game_state[3]),
         state_to_str(game_state[4]), state_to_str(game_state[5]),
         state_to_str(game_state[6]), state_to_str(game_state[7]),
         state_to_str(game_state[8]));
}

/* Converts a state indicator to the string to be displayed. */
char *state_to_str(unsigned char state) {
  if (state == 1) {
    return "X";
  } else if (state == 2) {
    return "O";
  }
  return " ";
}

/* Reads an integer input from the player that indicates the location for the
 * next move. The input is validated against the game state.*/
int read_player_input(unsigned char *game_state) {
  int next_move = -1;

  while (next_move == -1) {
    char input[8];
    int input_num;

    printf("Please enter your next move [1-9]: ");

    if (fgets(input, sizeof input, stdin)) {
      ++input_retry_count;
      if (sscanf(input, "%d", &input_num) == 1) {
        if (input_num >= 1 && input_num <= 9) {
          if (game_state[input_num - 1] == 0) {
            --input_retry_count;
            next_move = input_num;
          } else {
            printf("You cannot place your piece on top of another one.\n");
          }
        } else {
          printf("You must pick a valid spot on the board.\n");
        }
      } else {
        printf("Please enter a valid number.\n");
      }
    } else {
      /* The program may enter an infinite loop if EOF is reached without this
       * condition. */
      printf("\nUh-Oh, something went wrong.\n");
      return 255;
    }
  }

  /* Adjust to the correct index in the array. */
  return next_move - 1;
}

/* Evaluates the game state and decides if the game should continue. */
int evaluate_board(int player_id, unsigned char *game_state) {
  int next_player_id = player_id == 1 ? 2 : 1;

  /* Check all 8 lines for a possible winner. */
  int winner_id = 0;
  /* All 3 rows. */
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[0], game_state[1], game_state[2]);
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[3], game_state[4], game_state[5]);
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[6], game_state[7], game_state[8]);
  /* All 3 columns. */
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[0], game_state[3], game_state[6]);
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[1], game_state[4], game_state[7]);
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[2], game_state[5], game_state[8]);
  /* 2 diagonal lines. */
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[0], game_state[4], game_state[8]);
  winner_id = winner_id != 0
                  ? winner_id
                  : evaluate_line(game_state[2], game_state[4], game_state[6]);

  if (winner_id != 0) {
    return winner_id * -1;
  }

  int is_full = 1;
  int i = 0;
  for (; i < ROWS * COLS; ++i) {
    if (game_state[i] == 0) {
      is_full = 0;
    }
  }

  return is_full == 1 ? 0 : next_player_id;
}

/* Checks if all three points on a valid board line are occupied by the same
 * player piece. */
int evaluate_line(int state_1, int state_2, int state_3) {
  if (state_1 == state_2 && state_1 == state_3 && state_2 == state_3) {
    if (state_1 != 0 || state_2 != 0 || state_3 != 0) {
      return state_1;
    }
  }
  return 0;
}

/* Erase the text output that will be rerendered. */
void erase_board() {
  int lines_to_remove = (input_retry_count + 1) * 2 + 17;
  erase_lines(lines_to_remove);
  input_retry_count = 0;
}

void erase_lines(int num_lines) {
  int i = 0;
  for (; i < num_lines; ++i) {
    printf("\x1b[1F");
    printf("\x1b[2K");
  }
}
