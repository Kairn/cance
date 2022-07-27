/*
 * A program that prints all prime numbers less than 500.
 * The algorithm employed is "Sieve of Eratosthenes".
 *
 * Concepts:
 * Array constructs.
 * Work with arrays to create a sophisticated algorithm.
 */

#include <stdio.h>

#define MAX_NUMBER 500
#define FIRST_PRIME 2

int main(int argc, char const *argv[]) {
  /* Create an array to store where numbers have been marked as non-prime. */
  /* An unsigned char array is suitable as only true/false flags are stored. */
  /* The prime flag of a number is looked up by passing it as the array index.
   */
  unsigned char prime_status[MAX_NUMBER + 1];

  /* Initialize all flags to be 1 (prime). */
  int i;
  for (i = 0; i < MAX_NUMBER + 1; ++i) {
    prime_status[i] = 1;
  }

  printf("Searching for all prime numbers up to %d...\n", MAX_NUMBER);
  int total_prime_count = 0;

  /* Despite of its name, current_prime may NOT be a real prime number. This is
   * simply a loop variable for the completeness of covering every possible
   * integer, but whether it's prime or not will be checked against the
   * prime_status array. */
  int current_prime = FIRST_PRIME;
  for (; current_prime <= MAX_NUMBER; ++current_prime) {
    /* If it has not been marked non-prime, it is prime. */
    if (prime_status[current_prime] == 0) {
      continue;
    }

    printf("Found prime: %d.\n", current_prime);
    ++total_prime_count;

    /* Mark all multiples of this prime as non-prime, starting from its squared
     * value. */
    int marker = current_prime * current_prime;
    for (; marker < MAX_NUMBER + 1; marker += current_prime) {
      prime_status[marker] = 0;
    }
  }

  printf("Finished! Found a total of %d prime numbers.\n", total_prime_count);

  return 0;
}
