/*
 * A program that calculates the perimeter and area of a rectangle for the given
 * width and height.
 *
 * Concepts:
 * Read command line arguments.
 * Parse strings into numbers with basic validation.
 * Perform basic math operations.
 * Print formatted strings.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  /* At least three arguments needed. */
  /* The first argument is always the program name. */
  if (argc < 3) {
    printf("Please specify the width and height of the rectangle.\n");
    return 1;
  }

  /* Width is expected as the second argument. */
  const char *width_str = argv[1];
  /* strtod converts a string to a double. */
  /* NULL is passed as the second argument as there is no need to keep track of
   * the last parsed character. */
  double width = strtod(width_str, NULL);
  if (width <= 0) {
    /* Width should always be positive. */
    /* 0 is also returned if there is an error during parsing. */
    printf("Specified width <%s> is invalid.\n", width_str);
    return 1;
  }

  const char *height_str = argv[2];
  double height = strtod(height_str, NULL);
  if (height <= 0) {
    printf("Specified height <%s> is invalid.\n", height_str);
    return 1;
  }

  /* Numbers may overflow! */
  /* inf will be printed if they overflow. */
  double perimeter = 2 * (width + height);
  double area = width * height;

  printf("Width of the rectangle is %.4f.\n", width);
  printf("Height of the rectangle is %.4f.\n", height);
  printf("Perimeter of the rectangle is %.4f.\n", perimeter);
  printf("Area of the rectangle is %.4f.\n", area);
}
