/*
 * A program that prints the system information and memory occupation of basic C
 * data types for the underlying system architecture.
 *
 * Concepts:
 * Get system info through the uname function (minor).
 * How to use the sizeof operator.
 * How much memory does each of the basic data type occupy on the system.
 * Primer to pointers and memory management.
 */

#include <stdio.h>
#include <sys/utsname.h> /* A Unix like enviornment is required. */

int main(int argc, char const *argv[]) {
  /* A structure used to store data returned from the uname call. */
  struct utsname uname_data;

  /* Give the data pointer to the uname call. */
  uname(&uname_data);
  printf("Your system information:\n");

  /* Retrieve data from the uname call. */
  printf("System name: %s\n", uname_data.sysname);
  printf("Machine: %s\n", uname_data.machine);
  printf("Release: %s\n", uname_data.release);
  printf("Version: %s\n", uname_data.version);

  printf("=== End of system info ===\n\n");

  printf("A <char> data type takes <%ld> bytes of memory on your system.\n", sizeof(char));
  printf(
      "An <unsigned char> data type takes <%ld> bytes of memory on your "
      "system.\n",
      sizeof(unsigned char));
  printf("A <short> data type takes <%ld> bytes of memory on your system.\n", sizeof(short));
  printf(
      "An <unsigned short> data type takes <%ld> bytes of memory on your "
      "system.\n",
      sizeof(unsigned short));
  printf("An <int> data type takes <%ld> bytes of memory on your system.\n", sizeof(int));
  printf(
      "An <unsigned int> data type takes <%ld> bytes of memory on your "
      "system.\n",
      sizeof(unsigned int));
  printf("A <long> data type takes <%ld> bytes of memory on your system.\n", sizeof(long));
  printf(
      "An <unsigned long> data type takes <%ld> bytes of memory on your "
      "system.\n",
      sizeof(unsigned long));
  printf("A <float> data type takes <%ld> bytes of memory on your system.\n", sizeof(float));
  printf("A <double> data type takes <%ld> bytes of memory on your system.\n", sizeof(double));
  printf(
      "A <long double> data type takes <%ld> bytes of memory on your "
      "system.\n\n",
      sizeof(long double));

  int *int_ptr;
  int int_array[10];
  int int_2d_array[10][10];

  printf("An int pointer takes <%ld> bytes of memory on your system.\n", sizeof int_ptr);
  printf(
      "An int array of 10 elements takes <%ld> bytes of memory on your "
      "system.\n",
      sizeof int_array);
  printf(
      "A 10 by 10 matrix of int elements takes <%ld> bytes of memory on your "
      "system.\n\n",
      sizeof int_2d_array);

  char *string_short = "pretty short";
  char *string_longer =
      "a much much much much much much much much much much much much much "
      "longer string :)";
  char string_static[50] = "I need to be careful to not be too long";

  printf(
      "A pointer to a short string with a few characters takes <%ld> bytes of "
      "memory on your system.\n",
      sizeof string_short);
  printf(
      "A pointer to a longer string with more characters takes <%ld> bytes of "
      "memory on your system.\n",
      sizeof string_longer);
  printf(
      "A string declared as a char array of size 50 takes <%ld> bytes of "
      "memory on your system.\n",
      sizeof string_static);

  return 0;
}
