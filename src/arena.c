#include "sage.h"


static thread_local struct {
    sage_entity_t **lst;
    size_t len;
    size_t cap;
} *players = NULL;


extern void 
sage_arena_start(void)
{
    sage_require (players = malloc (sizeof *players));

    players->len = 0;
    players->cap = 4;

    size_t sz = sizeof *(players->lst) * players->cap;
    sage_require (players->lst = malloc (sz));
}


extern void 
sage_arena_stop(void)
{
    if (sage_likely (players)) {
        for (register size_t i = 0; i < players->len; i++)
            sage_entity_free (players->lst [i]);

        free (players->lst);
        free (players);
    }
}


extern sage_entity_t *
sage_arena_entity(size_t idx)
{
    return sage_entity_copy (players->lst [idx]);
}


extern void
sage_arena_entity_set(size_t idx, const sage_entity_t *ent)
{
    sage_entity_free (players->lst [idx]);
    players->lst [idx] = sage_entity_copy (ent);
}


extern size_t
sage_arena_push(const sage_entity_t *ent)
{
    if (sage_unlikely (players->len == players->cap)) {
        players->cap *= 2;
        size_t sz = sizeof *players->lst * players->cap;
        sage_require (players->lst = realloc (players->lst, sz));
    }

    players->lst [++players->len] = sage_entity_copy (ent);
    return players->len;
}


extern void 
sage_arena_pop(size_t idx)
{
    sage_entity_free (players->lst [idx]);
    players->lst [idx] = players->lst [players->len];
    players->lst [players->len--] = NULL;
}


extern void 
sage_arena_update(void)
{
    for (register size_t i = 0; i < players->len; i++)
        sage_entity_update (players->lst [i]);
}


extern void 
sage_arena_draw(void)
{
    for (register size_t i = 0; i < players->len; i++)
        sage_entity_draw (players->lst [i]);
}

