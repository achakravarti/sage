#include <string.h>
#include "sage.h"


struct sage_entity_t {
    sage_id_t cls;
    sage_id_t id;
    sage_vector_t *vec;
    sage_sprite_t *spr;
    struct sage_entity_vtable_t vt;
    /*sage_entity_update_f *upd;
    sage_entity_free_f *free;
    sage_entity_draw_f *draw;*/
};


static void draw_default(const sage_entity_t *ctx)
{
    sage_assert (ctx);

    if (sage_likely (sage_vector_visible (ctx->vec)))
        sage_sprite_draw (ctx->spr, sage_vector_point (ctx->vec));
}


static void free_default(sage_entity_t *ctx)
{
    (void) ctx;
}


static void update_default(sage_entity_t *ctx)
{
    (void) ctx;
}


extern sage_entity_t *sage_entity_new(sage_id_t cls, sage_id_t texid,
    struct sage_frame_t frm, const struct sage_entity_vtable_t *vt)
{
    sage_entity_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->cls = cls;
    ctx->id = (sage_id_t) 0;
    ctx->vec = sage_vector_new_zero ();
    ctx->spr = sage_sprite_new (texid, frm);
    
    ctx->vt.update = vt->update ? vt->update : &update_default;
    ctx->vt.draw = vt->draw ? vt->draw : &draw_default;
    ctx->vt.free = vt->free ? vt->free : &free_default;

    return ctx;
}


extern sage_entity_t *sage_entity_copy(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    sage_entity_t *cp = sage_entity_new (ctx->cls, sage_sprite_id (ctx->spr),
        sage_sprite_frames (ctx->spr), &ctx->vt);

    return cp;
}


extern void sage_entity_free(sage_entity_t **ctx)
{
    sage_entity_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        hnd->vec = sage_vector_free (hnd->vec);
        sage_sprite_free (&hnd->spr);
        hnd->vt.free (hnd);
        sage_heap_free ((void **) ctx);
    }
}


extern size_t sage_entity_size(void)
{
    return sizeof (struct sage_entity_t);
}


extern sage_id_t sage_entity_class(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx->cls;
}


extern sage_id_t sage_entity_id(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx->id;
}


extern void sage_entity_id_set(sage_entity_t *ctx, sage_id_t id)
{
    sage_assert (ctx);
    ctx->id = id;
}


extern sage_vector_t *sage_entity_vector(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return sage_vector_copy (ctx->vec);
}


extern void sage_entity_vector_set(sage_entity_t *ctx, const sage_vector_t *vec)
{
    sage_assert (ctx && vec);
    sage_vector_free (ctx->vec);
    ctx->vec = sage_vector_copy (vec);
}


extern void sage_entity_vector_move(sage_entity_t *ctx, 
    const sage_vector_t *vel)
{
    sage_assert (ctx && vel);
    sage_vector_add (ctx->vec, vel);
}


extern void sage_entity_frame(sage_entity_t *ctx, struct sage_frame_t frm)
{
    sage_assert (ctx);
    sage_sprite_frame (ctx->spr, frm);
}


extern void sage_entity_update(sage_entity_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.update (ctx);
}


extern void sage_entity_draw(sage_entity_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.draw (ctx);
}

