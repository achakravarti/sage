#include "sage.h"


struct sage_entity_t {
    sage_vector_t *vec;
    sage_sprite_t *spr;
    sage_entity_f *upd;
    sage_entity_f *free;
    sage_entity_f *draw;
};


static void
draw_default(sage_entity_t *ctx)
{
    sage_assert (ctx);

    if (sage_likely (sage_vector_visible (ctx->vec)))
        sage_sprite_draw (ctx->spr, sage_vector_point (ctx->vec));
}


extern sage_entity_t *
sage_entity_new(sage_id_t sid,
                sage_entity_f *upd,
                sage_entity_f *free,
                sage_entity_f *draw)
{
    sage_assert (upd);

    sage_entity_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->vec = sage_vector_new_zero ();
    (void) sid; //TODO:fix
    ctx->spr = NULL;//sage_sprite_factory_spawn (sid);//TODO:fix
    ctx->upd = upd;
    ctx->free = free;
    ctx->draw = draw ? draw : draw_default;

    return ctx;
}


extern sage_entity_t *
sage_entity_copy(const sage_entity_t *src)
{
    sage_assert (src);

    sage_entity_t *cp;
    sage_require (cp = malloc (sizeof *cp));

    cp->vec = sage_vector_copy (src->vec);
    cp->spr = sage_sprite_copy (src->spr);
    cp->upd = src->upd;
    cp->free = src->free;
    cp->draw = src->draw;

    return cp;
}


extern sage_entity_t *
sage_entity_free(sage_entity_t *ctx)
{
    if (sage_likely (ctx)) {
        ctx->vec = sage_vector_free (ctx->vec);
        ctx->spr = sage_sprite_free (ctx->spr);

        if (ctx->free) ctx->free (ctx);
        free (ctx);
    }

    return NULL;
}


extern sage_vector_t *
sage_entity_vector(const sage_entity_t *ctx)
{
    sage_assert (ctx);

    return sage_vector_copy (ctx->vec);
}


extern void 
sage_entity_vector_set(sage_entity_t *ctx, const sage_vector_t *vec)
{
    sage_assert (ctx && vec);

    sage_vector_free (ctx->vec);
    ctx->vec = sage_vector_copy (vec);
}

/*
extern struct sage_frame_t 
sage_entity_frame(const sage_entity_t *ctx)
{
    sage_assert (ctx);

    return sage_sprite_frame (ctx->spr);
}
*/


extern void 
sage_entity_frame_set(sage_entity_t *ctx, struct sage_frame_t frm)
{
    sage_assert (ctx);

    sage_sprite_frame (ctx->spr, frm);
}


extern void 
sage_entity_move(sage_entity_t *ctx, const sage_vector_t *vel)
{
    sage_assert (ctx && vel);

    sage_vector_add (ctx->vec, vel);
}


extern void 
sage_entity_update(sage_entity_t *ctx)
{
    sage_assert (ctx);

    ctx->upd (ctx);
}


extern void 
sage_entity_draw(sage_entity_t *ctx)
{
    sage_assert (ctx);

    ctx->draw (ctx);
}

