#include "../../includes/util/lang.h"

void* require_non_null(void* o, char* file, int line)
{

    if (IS_NULL(o))
    {
        fprintf(stderr, "Error: Object is null (at %s:%d)\n", file, line);
        exit(1);
    }

    return o;
    
}
