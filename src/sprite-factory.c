#include "sage.h"


static thread_local struct {
    sage_sprite_t **lst;
    size_t len;
    size_t cap;
} *sprite_pool = NULL;


extern void 
sage_sprite_factory_start(void)
{
    sage_require (sprite_pool = malloc (sizeof *sprite_pool));

    sprite_pool->len = 0;
    sprite_pool->cap = 4;

    size_t sz = sizeof *(sprite_pool->lst) * sprite_pool->cap;
    sage_require (sprite_pool->lst = malloc (sz));
}


extern void 
sage_sprite_factory_stop(void)
{
    if (sage_unlikely (!sprite_pool)) return;

    for (register size_t i = 0; i < sprite_pool->cap; i++) 
        free (sprite_pool->lst [i]);

    free (sprite_pool);
}


extern void 
sage_sprite_factory_register(const sage_sprite_t *spr)
{
    sage_assert (sprite_pool && spr);

    if (sage_unlikely (sprite_pool->len == sprite_pool->cap)) {
        sprite_pool->cap *= 2;
        size_t sz = sizeof *(sprite_pool->lst) * sprite_pool->cap;
        sage_require (sprite_pool->lst = realloc (sprite_pool->lst, sz));
    }

    sprite_pool->lst [sprite_pool->len++] = sage_sprite_copy (spr);
}


extern sage_sprite_t *
sage_sprite_factory_spawn(sage_id_t idx)
{
    sage_assert (sprite_pool && sprite_pool->lst);
    sage_assert (idx > 0 && idx <= sprite_pool->len);

    return sage_sprite_copy (sprite_pool->lst [idx]);
}

