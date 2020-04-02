#ifndef _LIST_
#define _LIST_

#include "lang.h"
#include "func.h"

/* List cell containing a generic object. */
struct _list_item {
    Any content;
    struct _list_item* next;
};

/* Alias for a list cell (for internal usage) */
typedef struct _list_item _ListItem;

/* Alias for a pointer on a list cell (for public usage,
   assuming that all list cells will be allocated dynamically) */
typedef _ListItem* ListItem;

/* Easy way to get the generic object stored in a list cell. */
#define ITEM_CONTENT(i) (i -> content)

/* Easy way to get the object stored in a list cell, strongly typed.
   Please note that you must know the type of the content. */
#define ITEM_CONTENT_AS(t,i) ((t) (i -> content))

/* Easy way to get the next cell in the list. */
#define NEXT(i) (i -> next)

/* List with size caching and O(1) appending. */
struct _list {
    ListItem head;
    ListItem last;
    int size;
};

/* Alias for a list (for internal usage) */
typedef struct _list _List;

/* Alias for a pointer on a list (for public usage,
   assuming that all lists will be allocated dynamically) */
typedef struct _list* List;

/* Creates a new empty list. */
List List_new();

/* Appends an element to the given list. */
void List_append(List self, Any content);

/* Appends an element to the given list, at a given position.
   Please note that position cannot be out of bounds :
   - if zero or negative, the element is inserted as new head.
   - if greater than the list size, the element is inserted at the end. */
void List_append_at(List self, int position, Any content);

/* Inserts an element as new head of given list. */
void List_push(List self, Any content);

/* Sends each element of the given list to a generic consumer. */
void List_for_each(List self, AnyConsumer action);

/* Returns the element in the list stored at given index or 
   throws a fatal error if it doesn't exist. */
Any List_get(List self, int index);

/* Clears the given list, removing all element.
   Elements in each cell can be deallocated if needed.
   You can provide a custom deallocator (will use
   simple free by default) */
void List_clear(List self, Boolean free_content, AnyConsumer custom_free);

/* Deletes the given list, freeing all allocated memory.
   Elements in each cell can be deallocated if needed.
   You can provide a custom deallocator (will use
   simple free by default) */
void List_free(List self, Boolean free_content, AnyConsumer custom_free);

/* Easy way to declare a (fake)-typed list (to improve readability). */
#define LIST_OF(t) List

/* Easy way to get a strongly typed element from a list at a given position. */
#define LIST_GET(t,l,i) ((t) List_get(l, i))

/* Easy way to get size of a list. */
#define LIST_SIZE(l) (l -> size)

/* Easy way to check if a list is empty. */
#define LIST_EMPTY(l) (LIST_SIZE(l) == 0)

/* Easy way to get head of a list. */
#define HEAD(l) (l -> head)

/* Easy way to get last cell of a list. */
#define LAST(l) (l -> last)

#endif
