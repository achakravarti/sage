#include "sage.h"


static thread_local struct {
    sage_entity_t **pool;
    sage_id_t *map;
    size_t len;
    size_t cap;
} *factory = NULL;


extern void 
sage_entity_factory_start(void)
{
    sage_require (factory = malloc (sizeof *factory));

    factory->len = 0;
    factory->cap = 4;

    size_t sz = sizeof *(factory->pool) * factory->cap;
    sage_require (factory->pool = malloc (sz));

    sz = sizeof *factory->map * factory->cap;
    sage_require (factory->map = malloc (sz));
}


extern void 
sage_entity_factory_stop(void)
{
    if (sage_likely (factory)) {
        for (register size_t i = 0; i < factory->cap; i++)
            free (factory->pool [i]);

        free (factory->pool);
        free (factory->map);
        free (factory);
    }
}


extern void 
sage_entity_factory_register(const sage_entity_t *ent)
{
    sage_assert (factory && ent);

    if (sage_unlikely (factory->len == factory->cap)) {
        factory->cap *= 2;

        size_t sz = sizeof *(factory->pool) * factory->cap;
        sage_require (factory->pool = realloc (factory->pool, sz));

        sz = sizeof *factory->map * factory->cap;
        sage_require (factory->map = realloc (factory->map, sz));
    }

    factory->map [factory->len] = sage_entity_id (ent);
    factory->pool [factory->len++] = sage_entity_copy (ent);

    /* TODO: Replace with hash map implementation e.g.:
     * https://web.archive.org/web/20160329102146/http://elliottback.com/wp/hashmap-implementation-in-c/ and 
     * http://www.kaushikbaruah.com/posts/data-structure-in-c-hashmap/ and
     * https://chromium.googlesource.com/aosp/platform/system/core/+/refs/heads/master/libcutils/hashmap.c */
}


extern sage_entity_t *
sage_entity_factory_spawn(sage_id_t id)
{
    sage_assert (factory && factory->pool);

    register size_t idx;
    for (idx = 0; idx < factory->len; idx++)
        if (factory->map [idx] == id) break;

    return sage_entity_copy (factory->pool [idx]);
}

