#include "sage.h"


static thread_local struct {
    sage_entity_t **lst;
    size_t len;
    size_t cap;
} *players = NULL;


extern void sage_arena_start(void)
{
    sage_require (players = malloc (sizeof *players));

    players->len = 1;
    players->cap = 4;

    size_t sz = sizeof *(players->lst) * players->cap;
    sage_require (players->lst = malloc (sz));
}


extern void sage_arena_stop(void)
{
    if (sage_likely (players)) {
        for (register size_t i = 0; i < players->cap; i++)
            free (players->lst [i]);

        free (players);
    }
}


extern sage_id_t sage_arena_push(sage_id_t id)
{
    if (sage_unlikely (players->len == players->cap)) {
        players->cap *= 2;
        size_t sz = sizeof *players->lst * players->cap;
        sage_require (players->lst = realloc (players->lst, sz));
    }

    players->lst [++players->len] = sage_entity_factory_spawn (id);
    return players->len;
}


extern void sage_arena_pop(sage_id_t id)
{
    sage_entity_free (players->lst [id]);
    players->lst [id] = players->lst [players->len];
    players->lst [players->len--] = NULL;
}


extern void sage_arena_update(void)
{
    for (register size_t i = 1; i <= players->len; i++)
        sage_entity_update (players->lst [i]);
}


extern void sage_arena_draw(void)
{
    for (register size_t i = 1; i <= players->len; i++)
        sage_entity_draw (players->lst [i]);
}

