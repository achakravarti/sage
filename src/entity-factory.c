#include "sage.h"


static thread_local struct {
    sage_entity_t **lst;
    sage_id_t *map;
    size_t len;
    size_t cap;
} *entity_pool = NULL;


extern void 
sage_entity_factory_start(void)
{
    sage_require (entity_pool = malloc (sizeof *entity_pool));

    entity_pool->len = 0;
    entity_pool->cap = 4;

    size_t sz = sizeof *(entity_pool->lst) * entity_pool->cap;
    sage_require (entity_pool->lst = malloc (sz));

    sz = sizeof *entity_pool->map * entity_pool->cap;
    sage_require (entity_pool->map = malloc (sz));
}


extern void 
sage_entity_factory_stop(void)
{
    if (sage_likely (entity_pool)) {
        for (register size_t i = 0; i < entity_pool->cap; i++)
            free (entity_pool->lst [i]);

        free (entity_pool->lst);
        free (entity_pool->map);
        free (entity_pool);
    }
}


extern void 
sage_entity_factory_register(const sage_entity_t *ent)
{
    sage_assert (entity_pool && ent);

    if (sage_unlikely (entity_pool->len == entity_pool->cap)) {
        entity_pool->cap *= 2;

        size_t sz = sizeof *(entity_pool->lst) * entity_pool->cap;
        sage_require (entity_pool->lst = realloc (entity_pool->lst, sz));

        sz = sizeof *entity_pool->map * entity_pool->cap;
        sage_require (entity_pool->map = realloc (entity_pool->map, sz));
    }

    entity_pool->map [entity_pool->len] = sage_entity_id (ent);
    entity_pool->lst [entity_pool->len++] = sage_entity_copy (ent);
}


extern sage_entity_t *
sage_entity_factory_spawn(sage_id_t id)
{
    sage_assert (entity_pool && entity_pool->lst);
    sage_assert (id > 0 && id <= entity_pool->len);

    register size_t idx;
    for (idx = 0; idx < entity_pool->len; idx++)
        if (entity_pool->map [idx] == id) break;

    return sage_entity_copy (entity_pool->lst [idx]);
}

