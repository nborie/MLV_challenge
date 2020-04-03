#include "../../includes/util/list.h"
#include "../../includes/util/misc.h"
#include "../../includes/util/str.h"
#include "../../includes/util/error.h"

/* --- declaration of private helpers */
ListItem _ListItem_new(Any content);
void _ListItem_free(ListItem self, Boolean free_content, AnyConsumer custom_free);
void _List_free_recursive(ListItem item, Boolean free_content, AnyConsumer custom_free);

List List_new()
{
    
    List list = NEW(_List);
    HEAD(list) = NULL;
    LAST(list) = NULL;
    LIST_SIZE(list) = 0;

    return list;

}

void List_append(List self, Any content)
{

    REQUIRE_NON_NULL(self);

    ListItem item = _ListItem_new(content);

    if (LIST_EMPTY(self))
        HEAD(self) = item;
    else
        NEXT(LAST(self)) = item;

    LAST(self) = item;
    LIST_SIZE(self)++;

}

void List_append_at(List self, int position, Any content)
{

    REQUIRE_NON_NULL(self);

    position = MAX(0, MIN(LIST_SIZE(self), position));

    if (position == LIST_SIZE(self))
    {
        List_append(self, content);
        return;
    }

    if (position == 0)
    {
        List_push(self, content);
        return;
    }

    ListItem item = _ListItem_new(content);

    ListItem index = HEAD(self);
    for (; position > 1; position--, index = NEXT(index));
    NEXT(item) = NEXT(index);
    NEXT(index) = item;
    LIST_SIZE(self)++;

}

void List_push(List self, Any content)
{

    REQUIRE_NON_NULL(self);

    if (LIST_EMPTY(self))
    {
        List_append(self, content);
        return;
    }

    ListItem item = _ListItem_new(content);
    NEXT(item) = HEAD(self);
    HEAD(self) = item;
    LIST_SIZE(self)++;

}

void List_for_each(List self, AnyConsumer action)
{

    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(action);

    ListItem index = HEAD(self);
    for (; NON_NULL(index); index = NEXT(index))
        action(ITEM_CONTENT(index));

}

Any List_get(List self, int index)
{
    
    REQUIRE_NON_NULL(self);

    if (LIST_EMPTY(self) || index < 0 || LIST_SIZE(self) <= index)
        THROW_AND_KILL("Index out of bounds !", "Cannot get item %d of list with only %d items.", index, LIST_SIZE(self));

    if (index == 0)
        return ITEM_CONTENT(HEAD(self));

    if ((index + 1) == LIST_SIZE(self))
        return ITEM_CONTENT(LAST(self));

    ListItem current = HEAD(self);
    for (; index > 0; current = NEXT(current), index--);

    return ITEM_CONTENT(current);

}

Boolean List_empty(List self)
{
    
    REQUIRE_NON_NULL(self);

    return LIST_EMPTY(self);

}

void List_clear(List self, Boolean free_content, AnyConsumer custom_free)
{

    REQUIRE_NON_NULL(self);

    _List_free_recursive(HEAD(self), free_content, custom_free);
    HEAD(self) = NULL;
    LAST(self) = NULL;
    LIST_SIZE(self) = 0;

}

void List_free(List self, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(self);

    _List_free_recursive(HEAD(self), free_content, custom_free);
    FREE(self);

}

/* --- private helpers */

ListItem _ListItem_new(Any content)
{

    ListItem item = NEW(_ListItem);
    ITEM_CONTENT(item) = content;
    NEXT(item) = NULL;

    return item;

}

void _ListItem_free(ListItem self, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(self);

    if (free_content && NON_NULL(ITEM_CONTENT(self)))
        IF_NULL_DEFAULT(custom_free, FREE)(ITEM_CONTENT(self));
    
    FREE(self);

}

void _List_free_recursive(ListItem item, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(item);

    if (NON_NULL(NEXT(item)))
        _ListItem_free(NEXT(item), free_content, custom_free);

    _ListItem_free(item, free_content, custom_free);

}
