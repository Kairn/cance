# Cance

## What C Things Do
### Complier
The C compiler (GCC, Clang, etc.) helps "transform" C source code into machine executable(s). This transformation includes 4 granular stages.

### Compilation Stages
* Pre-process: Process directives (lines starting with a `#`), strip comments, cleanup syntax so that the source code is easier to ingest.
* Compile: Transform C code into Assembly instructions targeting a specific CPU architecture. The output is human-readable but cannot be brought to different platforms.
* Assemble: Transform compiled Assembly code into object code targeting the underlying processor. The output contains machine instructions but CANNOT be run.
* Link: Fetch referenced library functions and rearrange the machine code as needed to produce an executable.

### C Standards
Specifications for the C programming language. Similar to Java SE (8, 11, 17, etc.) editions for the Java language. Important ones include:
1. ANSI C (C89): The first and most stable version of C, ratified in 1989.
2. C99: Published in 1999 with many new (and a few broken ones too) features introduced. Use it at your own risk.
3. C11: Ratified in 2011. "Fixed" broken C99 features but still considered relatively "new" with its own risks.
4. C17: The most current standard (as of 2022). No new features.

### Header File
Typically contain function prototypes, constants, and other declarations that can be safely repeated without harmful consequences (no runtime logic) in each translation unit (regular `.c` file) fed to the compiler. Intended to be included in source `.c` files and should NOT be compiled independently.

### Function Prototype
Definition of a function's signature (name, return type, parameters and their types) without implementation. Look like an abstract method in Java. The existence of a function prototype is enough for subsequent code to use it, and this code can be compiled independently without knowing the implementation. But the linker must know where to find the implementation in order to make the final program.

### Directive - \#include
When the pre-processor sees this directive, it looks for the file in the declaration and replaces the directive with the file content.

### Directive - \#define
Define a key-value mapping (source code text to text). The pre-processor will substitute in the declared value wherever the defined key appears in the source code as a token (like string replacement).

### Operator - sizeof
Determine the number of bytes needed to store the value given in the operand. The operand can be an expression or a type name. This is evaluated at compile time, and it cannot be used to check memory occupancy at run time (e.g. find the actual size of an array with a pointer to the first element).

### Strings
C does not have an explicit "String" type like in many other languages, so operators like `+` cannot be applied to them, nor can they be equality checked using `==`. C strings are represented as an array of type "char". C strings are always terminated by a null character (`\0`, code point 0), so the actual size of the string (or char array) is always 1 byte greater than its literal length.

## Syntax Memo
### Two \#include styles
1. Using angle brackets (e.g. `#include <foo.h>`): file should be found in standard system directories (try `gcc -xc -E -v -`).
2. Using double quotes (e.g. `#include "foo_local.h"`): file is in the current directory (same as the `.c` file that uses the directive).

### Usages of sizeof
1. sizeof is NOT a function and can be used without parentheses for identifiers (e.g. `sizeof my_var`).
2. Type names, however, must be enclosed in parentheses (e.g. `sizeof (unsigned int)`).

### Pointer to arrays
1. To declare a pointer to an array (e.g. int array), use the syntax `int (ptr*)[n]` where n is the number of elements in the array. This is not equivalent to `int **ptr` which is a pointer to an int pointer with different pointer arithmetics. Similarly, `int (ptr*)[n][m]` points to a 2D array of size n by m.
2. `ptr[1]` is syntactically equivalent to `*(ptr + 1)` (dereferencing after doing pointer arithmetic) when ptr is a pointer to an array element (of any type).
3. As a function argument, `void func(int arr[])` is syntactically equivalent to `void func(int *arr)`. To pass a 2D array, use the syntax `void func(int (ptr*)[n])` or `void func(int ptr[][n])`, where only the size of the first dimension can be omitted.

### String declaration and initialization
1. Either `char my_str[] = "foo"` or `char *my_str = "foo"` is a valid way to declare and initialize a C string.
2. However, `char my_str[]; my_str = "foo"` is NOT valid because arrays cannot be assigned to directly in C.

## TBD
***MORE TO BE COMPLETED***

## Play with Modules

### Source code format
`make format`

### Build all source modules
`make`
