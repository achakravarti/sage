#include <string.h>
#include "sage.h"


struct sage_entity_t {
    sage_id_t id;
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


static void
free_default(sage_entity_t *ctx)
{
    (void) ctx;
}


static void
update_default(sage_entity_t *ctx)
{
    (void) ctx;
}


extern sage_entity_t *
sage_entity_new(sage_id_t id,
                sage_id_t texid,
                struct sage_frame_t frm,
                sage_entity_f *upd,
                sage_entity_f *free,
                sage_entity_f *draw)
{
    sage_assert (upd);

    sage_entity_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->id = id;
    ctx->vec = sage_vector_new_zero ();
    ctx->spr = sage_sprite_new (texid, frm);

    ctx->upd = upd ? upd : update_default;
    ctx->free = free ? free : free_default;
    ctx->draw = draw ? draw : draw_default;

    return ctx;
}


extern sage_entity_t *
sage_entity_copy(const sage_entity_t *ctx)
{
    sage_assert (ctx);

    sage_entity_t *cp;
    sage_require (cp = malloc (sizeof *cp));
    sage_require (memcpy (cp, ctx, sizeof *cp));

    return cp;
}


extern sage_entity_t *
sage_entity_free(sage_entity_t *ctx)
{
    if (sage_likely (ctx)) {
        ctx->vec = sage_vector_free (ctx->vec);
        ctx->spr = sage_sprite_free (ctx->spr);

        ctx->free (ctx);
        free (ctx);
    }

    return NULL;
}


extern sage_id_t
sage_entity_id(const sage_entity_t *ctx)
{
    return ctx->id;
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


extern void 
sage_entity_vector_move(sage_entity_t *ctx, const sage_vector_t *vel)
{
    sage_assert (ctx && vel);

    sage_vector_add (ctx->vec, vel);
}


extern void 
sage_entity_frame(sage_entity_t *ctx, struct sage_frame_t frm)
{
    sage_assert (ctx);

    sage_sprite_frame (ctx->spr, frm);
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

