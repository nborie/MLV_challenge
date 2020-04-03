#ifndef _STR_
#define _STR_

#include "lang.h"
#include "list.h"

/* String containing a dynamic raw string and its cached length. */
typedef struct _string {
    char* str;
    int length;
} String;

/* Creates a new string with given content.
   Please note that:
   - The given content is deeply copied into it.
   - You must provide content with a final '\0' character. */
String* String_new(char* str);

/* Gets a sub-string from a given string, starting from
   start (inclusive) to end (exclusive).
   Please note that:
   - The given string is not modified and a new one is created.
   - If start and/or end is negative, the corresponding cursor is
     set from the end of the given string.
     --- Example ---------------
     String* str = String_new("Nicolas Borie aka Best Teacher");
     String* substr = String_sub(str, -7, LEN(str));
     // substr contains "Teacher"
   - If start and/or end is out of bounds (even after negative transformation),
     it will throw a fatal error. */
String* String_sub(String* self, int start, int end);

/* Concatenates a string with another.
   Please note that no string is modified and a new one is created. */
String* String_concat_str_str(String* self, String* other);

/* Concatenates a string with a raw one.
   Please note that no string is modified and a new one is created. */
String* String_concat_str_raw(String* self, char* other);

/* Concatenates a raw string with a string.
   Please note that no string is modified and a new one is created. */
String* String_concat_raw_str(char* self, String* other);

/* Concatenates a raw string with another.
   Please note that no string is modified and a new one is created. */
String* String_concat_raw_raw(char* self, char* other);

/* Splits a string into tokens, delimited by a given separator.
   You can specify that you want to keep empty tokens (separator
   is present at the end of the string, or adjacent ones) */
List String_split(String* self, char separator, Boolean keep_empty_tokens);

/* Prints the string into the standard output. */
void String_print(String* self);

/* Deletes the string and its allocated memory. */
void String_free(String* self);

/* Easy way to get length of a string. */
#define STR_LEN(s) (s -> length)

/* Easy way to check if a string is empty. */
#define STR_EMPTY(s) (STR_LEN(s) == 0)

/* Easy way to get content (raw string) of a string. */
#define STR(s) (s -> str)

#endif
