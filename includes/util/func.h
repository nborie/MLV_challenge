#ifndef _FUNC_
#define _FUNC_

#include "lang.h"

/* Alias for a runnable, ie. a function taking
   zero argument and returning nothing. */
typedef AnyFunc Runnable;

/* Macro to easily declare a consumer, ie. a function
   taking one argument and returning nothing. */
#define CONSUMER(t, n) FUNC(void, n, t)

/* Alias for a generic consumer, ie. consuming a generic pointer. */
typedef CONSUMER(Any, AnyConsumer);

#endif
