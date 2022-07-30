/*
 * A demonstration of multi-dimensional arrays in C.
 *
 * Concepts:
 * How to create a multi-dimensional array.
 * Access and modify a multi-dimensional array with pointers.
 * Pass a multi-dimensional array into another function using pointers.
 * Pointer vs. array syntax.
 */

#include <stdio.h>

#define NUM_STACKS 2
#define NUM_ROWS 4
#define NUM_COLS 5
#define DEBUG 0

/* Different syntax is used deliberately for demo purpose. */
void fill_stack(int stack[][NUM_COLS]);
void fill_row(int *row);
void print_array(int (*arr)[NUM_ROWS][NUM_COLS], int is_addr);

int main(int argc, char const *argv[]) {
  /* Declare a 3-dimensional array. */
  /* Equivalent to 2 stacks of 4 by 5 tables. */
  int array_3d[NUM_STACKS][NUM_ROWS][NUM_COLS];

  /* Fill the entire array with arbitrary values */
  int i = 0;
  for (; i < NUM_STACKS; ++i) {
    fill_stack(array_3d[i]);
  }

  /* Print out all values. */
  printf("Printing the 3D array:\n");
  print_array(array_3d, 0);

  printf("\n----------------------------------------\n\n");

  /* Print out all addresses of the array elements. */
  printf("Printing all addresses in the same order:\n");
  print_array(array_3d, 1);

  return 0;
}

void fill_stack(int stack[][NUM_COLS]) {
  int i = 0;
  for (; i < NUM_ROWS; ++i) {
    fill_row(stack[i]);
  }
}

void fill_row(int *row) {
  int i = 0;
  for (; i < NUM_COLS; ++i) {
    if (DEBUG == 1) {
      printf("Filling %p with %d\n", row + i, i + 1);
    }
    row[i] = i + 1;
  }
}

void print_array(int arr[][NUM_ROWS][NUM_COLS], int is_addr) {
  int i;
  int j;
  int k;

  for (i = 0; i < NUM_STACKS; ++i) {
    printf("\n=== Stack No. %d ===\n", i);
    for (j = 0; j < NUM_ROWS; ++j) {
      for (k = 0; k < NUM_COLS; ++k) {
        if (is_addr == 0) {
          printf("%d ", arr[i][j][k]);
        } else {
          printf("%p ", *(*(arr + i) + j) + k);
        }
      }
      printf("\n");
    }
  }
}
