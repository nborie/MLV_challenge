#include <string.h>
#include <stdio.h>

#include "../../includes/util/str.h"
#include "../../includes/util/list.h"
#include "../../includes/util/misc.h"
#include "../../includes/util/error.h"

/* --- declaration of private helpers */
Boolean _index_is_in_bounds(String* self, int index);
int _adapt_neg_index(int length, int index);
String* _concat(char* a, int a_len, char* b, int b_len);

String* String_new(char* str)
{
    
    REQUIRE_NON_NULL(str);

    String* self = NEW(String);
    int len = strlen(str);
    STR(self) = NEW_MULTIPLE(char, len + 1);
    memcpy(STR(self), str, len + 1);
    STR_LEN(self) = len;

    return self;

}

String* String_sub(String* self, int start, int end)
{

    REQUIRE_NON_NULL(self);

    start = _adapt_neg_index(STR_LEN(self), start);
    end = _adapt_neg_index(STR_LEN(self), end);

    start = MIN(start, end);
    end = MAX(start, end);

    if (!_index_is_in_bounds(self, start) || !_index_is_in_bounds(self, end))
        THROW_AND_KILL("Index out of bounds !", "Cannot get sub-string [%d:%d] of '%s'.", start, end, STR(self));

    int sub_length = end - start;
    char* sub_content = NEW_MULTIPLE(char, sub_length + 1);
    memcpy(sub_content, &(STR(self)[start]), sub_length);
    sub_content[sub_length] = '\0';

    return String_new(sub_content);

}

String* String_concat_str_str(String* self, String* other)
{
    
    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(other);

    return _concat(STR(self), STR_LEN(self), STR(other), STR_LEN(other));

}

String* String_concat_str_raw(String* self, char* other)
{

    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(other);

    return _concat(STR(self), STR_LEN(self), other, strlen(other));

}

String* String_concat_raw_str(char* self, String* other)
{

    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(other);

    return _concat(self, strlen(self), STR(other), STR_LEN(other));

}

String* String_concat_raw_raw(char* self, char* other)
{

    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(other);

    return _concat(self, strlen(self), other, strlen(other));

}

LIST_OF(String) String_split(String* self, char separator, Boolean keep_empty_tokens)
{

    REQUIRE_NON_NULL(self);

    LIST_OF(String) tokens = List_new();

    int start = 0, index;
    for (index = 0; index < STR_LEN(self); index++)
    {
        
        if (STR(self)[index] != separator)
            continue;
        
        if ((start != index) || keep_empty_tokens)
            List_append(tokens, String_sub(self, start, index));

        start = index + 1;

    }

    return tokens;

}

void String_print(String* self)
{
    
    REQUIRE_NON_NULL(self);

    printf("%s\n", STR(self));

}

void String_free(String* self)
{
    
    ENSURE_FREEABLE(self);

    FREE(STR(self));
    FREE(self);

}

/* --- private helpers */
Boolean _index_is_in_bounds(String* self, int index)
{
    return (0 <= index) && (index < STR_LEN(self));
}

int _adapt_neg_index(int length, int index)
{
    return (index < 0) ? (index + length) : index;
}

String* _concat(char* a, int a_len, char* b, int b_len)
{

    REQUIRE_NON_NULL(a);
    REQUIRE_NON_NULL(b);

    int concat_length = a_len + b_len;
    char* concat_content = NEW_MULTIPLE(char, concat_length + 1);
    memcpy(concat_content, a, a_len);
    memcpy(&(concat_content[a_len]), b, b_len);
    concat_content[concat_length] = '\0';

    return String_new(concat_content);
    
}