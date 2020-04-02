#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>

#include "../includes/plugins.h"
#include "../includes/util/misc.h"
#include "../includes/util/error.h"
#include "../includes/util/str.h"
#include "../includes/util/list.h"

/* --- declaration of private helpers */
int _only_so_files(const struct dirent *d);
typedef struct dirent** PluginsEntries;
int _discover_plugins(char* directory, PluginsEntries* plugins_entries);
void _AI_init(AI* ai);
Plugin* _Plugin_new(char* name);
Exception load_entry_point(void* handle, char* name, OUTPUT_ANY_FUNC(output));
Exception open_plugin(char* path, char* name, OUTPUT_ANY_OBJECT(output));
Exception load_plugin(Plugin* plugin, String* path);

LIST_OF(Plugin) Plugin_load_all(char* directory)
{

    printf("Starting plugins auto-loading from directory '%s'.\n", directory);
    
    PluginsEntries plugins_entries;
    int n = _discover_plugins("plugins", &plugins_entries);
    printf("Potential plugins found : %d\n", n);

    LIST_OF(Plugin) plugins = List_new();

    RANGE(i, 0, n, 1)
    {

        /* get full path and plugin name */
        String* file_name = String_new((plugins_entries[i]) -> d_name);
        String* full_path = String_concat_raw_str(directory, file_name);
        String* plugin_name = String_sub(file_name, 0, -3);
        String_free(file_name);

        Plugin* plugin = _Plugin_new(STR(plugin_name));
        printf("Loading plugin '%s'...\n", plugin -> name);
        
        /* try to load the current plugin */
        Exception error = load_plugin(plugin, full_path);
        if (HAS_ERROR(error))
        {
            Exception_log(error, FALSE);
            continue;
        }

        /* add the plugin to the list of successfully loaded ones */
        printf("Plugin '%s' has been successfully loaded !\n", STR(plugin_name));
        plugin -> ok = TRUE;
        List_append(plugins, plugin);

    }

    return plugins;

}

/* --- private helpers */

int _only_so_files(const struct dirent *d)
{

    char* extension = strrchr(d -> d_name, '.');
    if (NULL == extension)
        return 0;
    
    return 0 == strcmp(".so", extension);

}

int _discover_plugins(char* directory, PluginsEntries* plugins_entries)
{
    return scandir(directory, plugins_entries, _only_so_files, alphasort);
}

void _AI_init(AI* ai)
{

    ai -> take_decision = NULL;
    ai -> init = NULL;
    ai -> clean = NULL;

}

Plugin* _Plugin_new(char* name)
{

    REQUIRE_NON_NULL(name);

    Plugin* self = NEW(Plugin);
    self -> name = name;
    self -> starship = NULL;
    _AI_init(&(self -> ai));
    self -> ok = FALSE;
    self -> selected = FALSE;

    return self;

}

Exception load_entry_point(void* handle, char* name, OUTPUT_ANY_FUNC(output))
{

    /* try to load the entry-point from opened handle */
    void* entry_point = dlsym(handle, name);
    char* error = dlerror();

    /* if no error occurred then entry-point is saved
       into output and no error is thrown */
    if (IS_NULL(error))
    {
        SAVE_OUTPUT(output, entry_point);
        LEAVE_WITHOUT_ERROR;
    }

    /* safely close the handle and propagate the caught error */
    dlclose(handle);
    THROW(error, "Could not load entry-point '%s' for plugin '%s'.", name, "test");

}

Exception open_plugin(char* path, char* name, OUTPUT_ANY_OBJECT(output))
{

    /* try to open the dynamic lib */
    void* handle = dlopen(path, RTLD_LAZY);
    char* error = dlerror();

    /* if no error occurred then handle is saved
       into output and no error is thrown */
    if (IS_NULL(error))
    {
        SAVE_OUTPUT(output, handle);
        LEAVE_WITHOUT_ERROR;
    }

    /* propagate the caught error */
    THROW(error, "Could not load plugin '%s'.", name);

}

Exception load_plugin(Plugin* plugin, String* path)
{

    AI* ai = &(plugin -> ai);
    Any handle;
    Exception error;
    
    /* open the plugin to be able to load each AI entry-point */
    error = open_plugin(STR(path), plugin -> name, OUTPUT_TO(handle));
    PROPAGATE_IF_THROWN(error);

    /* load the decidor entry-point */
    error = load_entry_point(handle, DECIDOR_ENTRY_POINT, OUTPUT_TO_ANY_FUNC(ai -> take_decision));
    PROPAGATE_IF_THROWN(error);
    
    /* load the initializor entry-point */
    error = load_entry_point(handle, INITIALIZOR_ENTRY_POINT, OUTPUT_TO_ANY_FUNC(ai -> init));
    PROPAGATE_IF_THROWN(error);
    
    /* load the terminator entry-point */
    error = load_entry_point(handle, TERMINATOR_ENTRY_POINT, OUTPUT_TO_ANY_FUNC(ai -> clean));
    PROPAGATE_IF_THROWN(error);

    /* reach this point means that no error occurred */
    LEAVE_WITHOUT_ERROR;

}
