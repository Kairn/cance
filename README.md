# Cance

This project aims to be a solid reference for learning the fundamentals of the C programming language. The primary audience are experienced developers who program in other languages. I strongly believe understanding C will help anyone become a more effective developer. This documentation and associated program modules are meant to be informational and not opinionated. Advanced C topics are mostly not covered, but they can be pursued with relative ease once the basics are firmly understood, especially for seasoned programmers.

## C Things Explained
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
3. C11: Ratified in 2011. "Fixed" broken C99 features but still considered relatively "new". Some of the new features may not have been implemented at the time of this writing.
4. C17: The most current standard (as of 2022). No new features.

### Header File
Typically contain function prototypes, constants, and other declarations that can be safely repeated without harmful consequences (no runtime logic) in each translation unit (regular `.c` file) fed to the compiler. Intended to be included in source `.c` files and should NOT be compiled independently.

### Function Prototype
Definition of a function's signature (name, return type, parameters and their types) without implementation. Look like an abstract method in Java. The existence of a function prototype is enough for subsequent code to use it, and this code can be compiled independently without knowing the implementation. But the linker must know where to find the implementation in order to make the final program.

### Directive - \#include
When the pre-processor sees this directive, it looks for the file in the declaration and replaces the directive with the file content.

### Directive - \#define
Define a key-value mapping (source code text to text). The pre-processor will substitute in the declared value wherever the defined key appears in the source code as a token, similar to a string replacement.

### Operator - sizeof
Determine the number of bytes needed to store the value given in the operand. The operand can be an expression or a type name. This is evaluated at compile time, and it cannot be used to check memory occupancy at run time (e.g. find the actual size of an array with a pointer to the first element).

### String
C does not have an explicit "String" type like in many other languages, so operators like `+` cannot be applied to them, nor can they be equality checked using `==`. C strings are represented as an array of type "char". C strings are always terminated by a null character (`\0`, code point 0), so the actual size of the string (or char array) is always 1 byte greater than its literal length. String literals (e.g. `"my-awesome-str"`) are allocated in read-only memory and are alive for the lifetime of the program (can be returned by functions directly without a local variable).

### Pointer
A C pointer represents a memory address (size depends on the system) that points to a single byte. The type of a pointer only affects how the memory is read by the program and how pointer arithmetic is performed. A void pointer (not the same as NULL) can point to any type, but it must be cast to an explicit type before use.

### Memory Allocation
The compiler automatically allocates memory on the stack for static declarations such as local variables, fixed size arrays, and functions, and these pieces of memory will be freed as soon as they go out of scope (their respective pointers will point to invalid memory). When memory is allocated dynamically (e.g. with `malloc`), it is created on the heap and will NOT be cleaned up until the program ends or when freed explicitly in the code. Note, when `realloc` is invoked, it takes care of freeing the old memory block pointed by the argument pointer if needed, so the programmer will only need to free the new memory region returned from the function.

### Keyword - struct
In essence, a struct is an abstraction of a chunk of memory (with a certain size) and a series of offsets (how deep is each member into the memory). Similar to a C array, a pointer to a structure is also a pointer to the first member (with an offset of 0) of the structure. Due to memory alignment, the size of a structure can be larger than the sum of the sizes of its members (with padding added between members of different sizes), so memory can sometimes be saved or wasted depending on how the members are ordered in the struct definition.

## Syntax Memo
### Two \#include styles
1. Using angle brackets (e.g. `#include <foo.h>`) to indicate the file should be found in standard system directories (try `gcc -xc -E -v -`).
2. Using double quotes (e.g. `#include "foo_local.h"`) to indicate the file is in the current directory (same as the `.c` file that uses the directive).

### Usages of sizeof
1. sizeof is NOT a function and can be used without parentheses for identifiers (e.g. `sizeof my_var`).
2. Type names, however, must be enclosed in parentheses (e.g. `sizeof (unsigned int)`).

### Pointer to Arrays
1. To declare a pointer to an array (e.g. int array), use the syntax `int (ptr*)[n]` where n is the number of elements in the array. This is not equivalent to `int **ptr` which is a pointer to an int pointer with different pointer arithmetics. Similarly, `int (ptr*)[n][m]` points to a 2D array of size n by m.
2. `ptr[1]` is syntactically equivalent to `*(ptr + 1)` (dereferencing after doing pointer arithmetic) when ptr is a pointer to an array element (of any type).
3. As a function argument, `void func(int arr[])` is syntactically equivalent to `void func(int *arr)`. To pass a 2D array, use the syntax `void func(int (ptr*)[n])` or `void func(int ptr[][n])`, where only the size of the first dimension can be omitted.

### String Declaration and Initialization
1. Either `char my_str[] = "foo"` or `char *my_str = "foo"` is a valid way to declare and initialize a C string.
2. However, `char my_str[]; my_str = "foo"` is NOT valid because arrays cannot be assigned to directly in C.

### Const Pointers
1. Use `const int *ptr` syntax to declare an int pointer that cannot be used to change the value that it points to (`*ptr = my_int` will be illegal). But you may change the pointer to point to a different int, and you may modify the int value directly (if the int is not const).
2. Use `int *const ptr` syntax to declare a pointer that will always point to the same address. But the value stored on the address may be changed.
3. `const int *const ptr` is the way to declare a constant pointer whose pointed value also cannot be changed via this pointer.

### Usages of struct
1. Use `struct my_struct_ab {int a; int b;}` to create a structure definition only.
2. Use `struct my_struct {...} my_struct_var` to create a definition and a variable instance at the same time.
3. Use `struct {...} my_var` to declare an unnamed structure (no definition) variable called "my_var".
4. Define a nested struct using `struct outer_struct {struct inner_struct {...} inner_member;}`, but the "inner_struct" in not visible outside the "outer_struct" definition.

### Initialization of struct
Assume we have a structure definition `struct my_struct {int a; int b;}`.
1. Use `struct my_struct my_var = {1, 2}` to initialize its members in how they are ordered in the definition. Not all members need to be initialized.
2. Use `struct my_struct my_var = {.a = 1, .b = 2}` to initialize members referencing their names.

### Access of struct
1. To access a struct member from a non-pointer struct variable, use the syntax `my_struct.member_a`.
2. If given a struct pointer, its member can be accessed using `my_struct_ptr->member_a` which is equivalent to `(*my_struct_ptr).member_a`. Note, this syntax grabs the member itself, not a pointer to the member.

### Usages of typedef
1. Use `typedef int my_int_alias` to create an alias to an existing/known type.
2. Use `typedef struct my_struct {...} my_struct_type` to create a type name for a struct. The `struct` keyword can be omitted when declaring a variable of "my_struct_type". Note, the struct may be unnamed. The struct itself can either be defined in-place (with `{...}`) or already existing (without `{...}`).

## Play with Modules
This project has 11 modules. Each module is a different program that is meant to demonstrate different concepts in C. They can be compiled and run separately with no internal dependencies. Everything is written in the C89 standard (no fancy compiler needed). A Unix-like operating environment is required, however. Make sure standard tools like "gcc" and "make" are installed.

### Source code format
`make format`

### Build all source modules
`make`

### Run modules
`./build/<MODULE_NAME>`, replace `<MODULE_NAME>` with the name of the module to run (e.g. `./build/1_hello`).
