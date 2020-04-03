#ifndef _LANG_
#define _LANG_

#include <stdlib.h>
#include <stdio.h>

/* Checks if a pointer is null. */
#define IS_NULL(o) (NULL == (o))

/* Checks if a pointer is not null. */
#define NON_NULL(o) (NULL != (o))

/* Dummy boolean definition. */
typedef char Boolean;
#define TRUE 1
#define FALSE 0

/* Allocates memory and returns a pointer to it,
   or throws a fatal exception if an error occurred. */
void* ensure_new(size_t size);

/* Simple macro to allocate instance of a given type and
   ensure success, with auto cast of the returned pointer.
   --- Example ----------------------------------------------
   int* my_allocated_int = NEW(int);
   ---------------------------------------------------------- */
#define NEW(t) ((t*) ensure_new(sizeof(t)))

/* Simple macro to allocate an array of instances of a given type and
   ensure success, with auto cast of the returned pointer.
   --- Example ----------------------------------------------
   int* my_array_of_int = NEW_MULTIPLE(int, 5);
   ---------------------------------------------------------- */
#define NEW_MULTIPLE(t,n) ((t*) ensure_new(n * sizeof(t)))

/* Simple macro to free previously allocated memory. */
#define FREE free

/* Simple macro to ensure that a given instance is free-able,
   exiting the current function if not. This macro should be used
   in functions especially defined to free an instance of a given type.
   --- Example ----------------------------------------------
   void free_my_allocated_int(int* self)
   {
       ENSURE_FREEABLE(self);
       FREE(self);
   }
   ---------------------------------------------------------- */
#define ENSURE_FREEABLE(o) if (IS_NULL(o)) { return; }

/* Pointer-ified type.
   --- Example ----------------------------------------------
   POINTER(int) my_pointer_on_int = NULL;
   ---------------------------------------------------------- */
#define POINTER(t) t*

/* Gets the address of an object.
   --- Example ----------------------------------------------
   int my_int = 42;
   POINTER(int) my_pointer_on_int = REF(my_int);
   ---------------------------------------------------------- */
#define REF(o) &(o)

/* Access to the value pointed by a given pointer.
   --- Example ----------------------------------------------
   int my_int = 42;
   POINTER(int) my_pointer_on_int = REF(my_int);
   printf("%d !", AT(my_pointer_on_int));
   ---------------------------------------------------------- */
#define AT(t) *t

/* Simpler way to define a function-type.
   --- Example ----------------------------------------------
   int foo(int a, char* b) { ... }
   FUNC(int, my_function, int, char*) = foo;
   int result = my_function(42, "Quarante-Deux");
   ---------------------------------------------------------- */
#define FUNC(r, n, ...) r (*n)(__VA_ARGS__)

/* Alias for a generic pointer. */
typedef void* Any;

/* Alias for a generic function, equivalent to : void foo(void) */
typedef FUNC(void, AnyFunc, void);

/* Function-like casting macro.
   --- Example ----------------------------------------------
   int my_int = 42;
   int* my_pointer_on_int = &my_int;
   Any my_generic_pointer = CAST(Any, my_pointer_on_int);
   ---------------------------------------------------------- */
#define CAST(t, o) ((t)(o))

/* Generify a pointer.
   --- Example ----------------------------------------------
   int my_int = 42;
   int* my_pointer_on_int = &my_int;
   Any my_generic_pointer = DESPECIALIZE(my_pointer_on_int);
   ---------------------------------------------------------- */
#define DESPECIALIZE(o) CAST(Any, o)

/* Generify a function.
   --- Example ----------------------------------------------
   int foo(int a, char* b) { ... }
   FUNC(int, my_function, int, char*) = foo;
   AnyFunc my_generic_function = DESPECIALIZE_FUNC(my_function);
   ---------------------------------------------------------- */
#define DESPECIALIZE_FUNC(f) CAST(AnyFunc, f)

/* Declares a parameter used as an output variable.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_OBJECT(int, my_output)) { ... }
   int result;
   foo(42, "Quarante-Deux", &result);
   ---------------------------------------------------------- */
#define OUTPUT_OBJECT(t,n) POINTER(POINTER(t)) n

/* Declares a generic parameter used as an output variable.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_ANY_OBJECT(my_output)) { ... }
   Any result;
   foo(42, "Quarante-Deux", &result);
   ---------------------------------------------------------- */
#define OUTPUT_ANY_OBJECT(n) POINTER(Any) n

/* Declares a function parameter used as an output variable.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_FUNC(int, my_output, int)) { ... }
   FUNC(int, result, int);
   foo(42, "Quarante-Deux", &result);
   ---------------------------------------------------------- */
#define OUTPUT_FUNC(r, n, ...) FUNC(r, *n, __VA_ARGS__)

/* Declares a generic function parameter used as an output variable.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_ANY_FUNC(my_output)) { ... }
   AnyFunc result;
   foo(42, "Quarante-Deux", &result);
   ---------------------------------------------------------- */
#define OUTPUT_ANY_FUNC(n) POINTER(AnyFunc) n

/* Registers an argument as an output.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_OBJECT(int, my_output)) { ... }
   int result;
   foo(42, "Quarante-Deux", OUTPUT_TO(result));
   ---------------------------------------------------------- */
#define OUTPUT_TO(o) REF(o)

/* Registers a generic argument as an output.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_ANY_OBJECT(my_output)) { ... }
   Any result;
   foo(42, "Quarante-Deux", OUTPUT_TO_ANY(result));
   ---------------------------------------------------------- */
#define OUTPUT_TO_ANY(o) REF(DESPECIALIZE(o))

/* Registers a function argument used as an output.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_FUNC(int, my_output, int)) { ... }
   FUNC(int, result, int);
   foo(42, "Quarante-Deux", OUTPUT_TO_FUNC(result));
   ---------------------------------------------------------- */
#define OUTPUT_TO_FUNC(f) REF(f)

/* Registers a generic function argument used as an output.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_ANY_FUNC(my_output)) { ... }
   AnyFunc result;
   foo(42, "Quarante-Deux", OUTPUT_TO_ANY_FUNC(result));
   ---------------------------------------------------------- */
#define OUTPUT_TO_ANY_FUNC(f) CAST(AnyFunc*, REF(f))

/* Saves a value to an output parameter.
   --- Example ----------------------------------------------
   void foo(int a, char* b, OUTPUT_OBJECT(int, my_output)) {
       ...
       SAVE_OUTPUT(my_output, 1234);
   }
   int* result;
   foo(42, "Quarante-Deux", OUTPUT_TO(result));
   ---------------------------------------------------------- */
#define SAVE_OUTPUT(o, v) (AT(o) = v)

/* Dummy macro allowing to generate an index on range without having
   to declare the variable and build the loop components. Useful in
   particular to walk through an array. Please note that the range is
   bounded as [start, end[
   --- Example ----------------------------------------------
   RANGE(i, 4, 10, 2) { ... }
   --- is equivalent to :
   int i; for (i = 4; i < 10; i += 2) { ... }
   ---------------------------------------------------------- */
#define RANGE(index, start, end, step) \
    int index; \
    for (index = start; index < end; i += step)

#endif