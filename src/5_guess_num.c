/*
 * A game that the user has to guess a number between 0 and 64 within 6
 * attempts.
 *
 * Concepts:
 * Get the current timestamp (minor).
 * How to generate a random number.
 * Standard loop constructs (for and while).
 * Sophisticated control flow with nested loops.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ATTEMPTS 6
#define MAX_NUMBER 64

int main(int argc, char const *argv[]) {
  /* Can also pass in a pointer to track the return value. */
  time_t current_time = time(NULL);

  /* Initialize the RNG seed. */
  srandom((unsigned int)current_time);

  /* Get a random number bound between 0 and 64. */
  int magic_number = random() % (MAX_NUMBER + 1);

  printf("The magic number has been generated, and the game has started!\n");
  printf("You must guess the magic number between 0 and %d (inclusive).\n\n",
         MAX_NUMBER);

  /* For loop counter. */
  /* Must be declared outside of the loop construct in C89. */
  int guess_count;
  for (guess_count = 0; guess_count < MAX_ATTEMPTS; ++guess_count) {
    printf("You have %d attempt(s) left.\n", MAX_ATTEMPTS - guess_count);
    int guessed = -1;

    /* Read user input until a valid number is entered. */
    while (guessed == -1) {
      char input[16];
      int input_num;

      printf("Enter your guess #%d:\n", guess_count + 1);

      if (fgets(input, sizeof input, stdin)) {
        if (sscanf(input, "%d", &input_num) == 1) {
          if (input_num >= 0 && input_num <= MAX_NUMBER) {
            guessed = input_num;
            break;
          } else {
            printf(
                "You guessed a number outside the allowed range. Try again.\n");
            continue;
          }
        }
      } else {
        /* The program may enter an infinite loop if EOF is reached without this
         * condition. */
        printf(
            "We have encountered a problem with I/O. The game must end now.\n");
        return 1;
      }

      printf("You input does not seem valid. Could you try again?\n");
    }

    printf("You guessed <%d>!\n", guessed);

    if (guessed == magic_number) {
      printf("Congratulations, this is the magic number!\n");
      printf("You have won!\n");
      return 0;
    } else if (guessed < magic_number) {
      printf("But unfortunately, this is too low.\n");
    } else {
      printf("But unfortunately, this is too high.\n");
    }
  }

  printf(
      "\nYou have exhausted all you chances. The magic number is actually "
      "<%d>.\n",
      magic_number);
  printf("Game Over.\n");

  return 0;
}
