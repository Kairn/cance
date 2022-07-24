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

## Syntax Memo
### Two \#include styles
1. Using angle brackets (e.g. `#include <foo.h>`): file should be found in standard system directories (try `gcc -xc -E -v -`).
2. Using double quotes (e.g. `#include "foo_local.h"`): file is in the current directory (same as the `.c` file that uses the directive).

### Usages of sizeof
1. sizeof is NOT a function and can be used without parentheses for identifiers (e.g. `sizeof my_var`).
2. Type names, however, must be enclosed in parentheses (e.g. `sizeof (unsigned int)`).

## TBD
***MORE TO BE COMPLETED***

## Play with Modules

### Source code format
`make format`

### Build all source modules
`make`
