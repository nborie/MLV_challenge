#include "../../includes/util/lang.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void* ensure_new(size_t size)
{

    void *instance = malloc(size);
    if (IS_NULL(instance))
    {
        fprintf(stderr, "Memory error while trying to allocate a new instance of an object.\n");
        exit(1);
    }

    return instance;

}

void ensure_free(void* o)
{
    free(o);
}
