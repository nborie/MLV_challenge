#ifndef _CONTRACT_
#define _CONTRACT_

#include <stdlib.h>
#include <stdio.h>

/* Transforms a pointer to another it it's null. */
#define IF_NULL_DEFAULT(o, d) (IS_NULL(o) ? (d) : (o))

/* Checks that a pointer is not null, and returns it.
   If the pointer is null, it throws a fatal exception,
   indicating in which file and at which line the requirement
   has been declared (to understand which pointer has been checked). */
void* require_non_null(void* o, char* file, int line);

/* Macro to simplify non-null requirement declaration,
   automatically retrieving the file and line of the declaration.
   --- Example -------------------------------------
   // my_file.c
   12. void foo(int* a)
   13. {
   14.    REQUIRE_NON_NULL(a);
   15. }
   --- Will display --------------------------------
   Error: Object is null (at my_file.c:14)\n
   ------------------------------------------------- */
#define REQUIRE_NON_NULL(o) require_non_null(o, __FILE__, __LINE__)

#endif
