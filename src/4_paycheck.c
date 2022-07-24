/*
 * A program that calculates the net pay based on the hours worked in a week.
 *
 * Concepts:
 * Read user inputs from console.
 * How to convert a string (char array) into a number.
 * How to use the #define directive.
 * Create sophisticated business logic with multiple if conditions.
 */

#include <stdio.h>

/* #define macros are processed similar to text substitutions. */
/* These do NOT have types. */
#define BASE_PAY_RATE 14.00
#define OVERTIME_PAY_RATE_MULTIPLIER 1.5
#define OVERTIME_THRESHOLD 40

#define TAX_BRACKET_1_THRESHOLD 600
#define TAX_BRACKET_1_RATE 0.1
#define TAX_BRACKET_2_THRESHOLD 400
#define TAX_BRACKET_2_RATE 0.15
#define TAX_BRACKET_REMAINING_RATE 0.2

int main(int argc, char const *argv[]) {
  /* User input buffer. */
  char input[32];
  /* Store the parsed input as a double, may overflow! */
  double hours;

  printf("Enter the number of hours worked in the week:\n");

  /* fgets loads some bytes into the buffer from stdin. */
  /* A falsy value is returned if an error occurs. */
  if (fgets(input, sizeof input, stdin)) {
    /* Parse the input string into a double. */
    if (sscanf(input, "%lf", &hours) == 1) {
      if (hours < 0) {
        printf("Hours must NOT be negative.\n");
        return 1;
      }
    } else {
      printf("Input <%s> is not a valid number.\n", input);
      return 1;
    }
  } else {
    printf("Unable to read your input.\n");
    return 1;
  }

  double overtime_hours;
  double overtime_pay_rate = BASE_PAY_RATE * OVERTIME_PAY_RATE_MULTIPLIER;

  /* Calculate gross pay amount. */
  double gross_pay;
  if (hours > OVERTIME_THRESHOLD) {
    overtime_hours = hours - OVERTIME_THRESHOLD;
    gross_pay =
        OVERTIME_THRESHOLD * BASE_PAY_RATE + overtime_hours * overtime_pay_rate;
  } else {
    overtime_hours = 0;
    gross_pay = hours * BASE_PAY_RATE;
  }

  /* Calculate the total tax payable. */
  double total_tax;
  if (gross_pay <= TAX_BRACKET_1_THRESHOLD) {
    /* Deduct with bracket 1's rate. */
    total_tax = gross_pay * TAX_BRACKET_1_RATE;
  } else if (gross_pay <= (TAX_BRACKET_1_THRESHOLD + TAX_BRACKET_2_THRESHOLD)) {
    /* Deduct with bracket 1's rate for the income within bracket 1. */
    total_tax = TAX_BRACKET_1_THRESHOLD * TAX_BRACKET_1_RATE;
    /* Deduct the remaining with bracket 2's rate. */
    total_tax += (gross_pay - TAX_BRACKET_1_THRESHOLD) * TAX_BRACKET_2_RATE;
  } else {
    /* Deduct in all three brackets. */
    total_tax = TAX_BRACKET_1_THRESHOLD * TAX_BRACKET_1_RATE;
    total_tax += TAX_BRACKET_2_THRESHOLD * TAX_BRACKET_2_RATE;
    total_tax +=
        (gross_pay - TAX_BRACKET_1_THRESHOLD - TAX_BRACKET_2_THRESHOLD) *
        TAX_BRACKET_REMAINING_RATE;
  }

  printf("=== Weekly paycheck summary ===\n");
  printf("Total hours worked: %.2lf\n", hours);
  printf("Overtime hours: %.2lf\n", overtime_hours);
  printf("Gross pay: %.2lf\n", gross_pay);
  printf("Tax payable: %.2lf\n", total_tax);
  printf("Net pay: %.2lf\n", gross_pay - total_tax);

  return 0;
}
