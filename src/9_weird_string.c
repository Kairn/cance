/*
 * A program that converts a user input sentence into a weird form by
 * uppercasing and reversing every word, stripping punctuations, and
 * re-concatenating the words in sorted order.
 *
 * Concepts:
 * How to tokenize a string (splitting).
 * Iterate and manipulate characters in strings.
 * Use standard string library functions (check length, compare, etc.).
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_CHAR_LEN = 1000;
const char *WORD_DELIMITER = " ";

void weird_transform(char *str);

int main(int argc, char const *argv[]) {
  char input[MAX_CHAR_LEN];
  /* Take a copy as "strtok" is applied twice. */
  char copy[MAX_CHAR_LEN];

  printf("Try to write an English sentence here 😊:\n");

  if (fgets(input, sizeof input, stdin)) {
    /* This removes the trailing linefeed. */
    input[strcspn(input, "\n")] = '\0';
    printf("Ok, you input has been accepted.\n");
    /* Be careful with string copy (buffer overflow risk). */
    strncpy(copy, input, MAX_CHAR_LEN);
  } else {
    printf("Sorry, the program has been interrupted.\n");
    return 1;
  }

  printf("\nMagic is being applied...\n");

  /* First count how many words are in the sentence. */
  int num_tokens = 0;
  char *next_token = strtok(input, WORD_DELIMITER);

  while (next_token != NULL) {
    ++num_tokens;
    /* NULL in the first argument will continue the search on the same string.
     */
    next_token = strtok(NULL, WORD_DELIMITER);
  }

  /* Allocate the word array and place the tokens into it after transformation.
   */
  char *tokens[num_tokens];
  next_token = strtok(copy, WORD_DELIMITER);
  int i = 0;

  while (next_token != NULL) {
    weird_transform(next_token);
    tokens[i++] = next_token;
    next_token = strtok(NULL, WORD_DELIMITER);
  }

  /* Simple bubble sort. */
  int is_sorted;
  int k;
  do {
    is_sorted = 1;
    for (k = 0; k < num_tokens - 1; ++k) {
      /* String comparison by ASCII ordering. */
      if (strcmp(tokens[k + 1], tokens[k]) < 0) {
        /* Swap the two strings. */
        char *temp = tokens[k];
        tokens[k] = tokens[k + 1];
        tokens[k + 1] = temp;
        is_sorted = 0;
      }
    }
  } while (is_sorted == 0);

  printf("Success! The output is:\n<");

  int j = 0;
  for (; j < num_tokens; ++j) {
    char *token = tokens[j];
    if (strlen(token) <= 0) {
      continue;
    }

    printf("%s ", tokens[j]);
  }

  printf("\b> (END)\n");

  return 0;
}

/* Weirdly transforms the input string (in-place). */
void weird_transform(char *str) {
  int len = strlen(str);
  /* An extra byte for the null terminator. */
  char temp[len + 1];

  /* Keep track of how many characters will be written. */
  int write_pos = 0;

  int read_pos = len - 1;
  for (; read_pos >= 0; --read_pos) {
    char current_char = *(str + read_pos);
    /* Only take in alphanumeric characters. */
    if (isalnum(current_char) != 0) {
      temp[write_pos++] = toupper(current_char);
    }
  }

  /* Overwrite the original string. */
  int i = 0;
  for (; i < write_pos; ++i) {
    *(str + i) = temp[i];
  }

  /* Write the null byte to terminate the string. */
  str[write_pos] = 0;

  return;
}
