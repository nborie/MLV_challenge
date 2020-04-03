#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#include "../../includes/util/misc.h"
#include "../../includes/util/error.h"

Exception Exception_new(char* file, int line, char* reason, char* message, ...)
{

    REQUIRE_NON_NULL(file);
    REQUIRE_NON_NULL(reason);
    REQUIRE_NON_NULL(message);

    Exception self = NEW(_Exception);
    self -> file = file;
    self -> line = line;
    self -> reason = reason;

    va_list args;
    va_start(args, message);
    vsprintf(self -> message, message, args);
    va_end(args);

    return self;

}

void Exception_log(Exception self, Boolean strict)
{
    
    REQUIRE_NON_NULL(self);

    fprintf(stderr, "Error: %s\n>>> Spotted at: (%s:%d)\n>>> Reason: %s\n", self -> message, self -> file, self -> line, self -> reason);
    if (strict)
        exit(1);

}

void Exception_free(Exception self)
{
    
    ENSURE_FREEABLE(self);

    FREE(self);
    
}
