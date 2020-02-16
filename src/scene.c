#include "./sage.h"


struct sage_scene_t {
    sage_id_t                  id;
    sage_entity_list_t         *ents;
    sage_payload_t             *cdata;
    struct sage_scene_vtable_t vt;
};


void start_default(sage_scene_t *ctx)
{
    (void) ctx;
}


void stop_default(sage_scene_t *ctx)
{
    (void) ctx;
}


void update_default(sage_scene_t *ctx)
{
    sage_assert (ctx);
    sage_entity_list_update(ctx->ents);
}


void draw_default(const sage_scene_t *ctx)
{
    sage_assert (ctx);
    sage_entity_list_draw(ctx->ents);
}


extern sage_scene_t *
sage_scene_new(sage_id_t                        id,
               sage_payload_t                   *cdata,
               const struct sage_scene_vtable_t *vt)
{
    sage_scene_t *ctx = sage_heap_new(sizeof *ctx);

    ctx->id = id;
    ctx->ents = sage_entity_list_new();
    ctx->cdata = sage_payload_copy_deep(cdata);

    sage_assert (vt);
    ctx->vt.start = sage_likely (vt->start) ? vt->start : start_default;
    ctx->vt.stop = sage_likely (vt->stop) ? vt->stop : stop_default;
    ctx->vt.update = sage_likely (vt->update) ? vt->update : update_default;
    ctx->vt.draw = sage_likely (vt->draw) ? vt->draw : draw_default;

    return ctx;
}


extern inline sage_scene_t *
sage_scene_move(sage_scene_t *ctx);


extern inline const sage_scene_t *
sage_scene_link(const sage_scene_t *ctx);


extern sage_scene_t *
sage_scene_copy(const sage_scene_t *ctx)
{
    sage_assert (ctx);
    sage_scene_t *cp = sage_scene_new(ctx->id, ctx->cdata, &ctx->vt);
    cp->ents = sage_entity_list_copy_deep(ctx->ents);

    return cp;
}


extern void
sage_scene_free(sage_scene_t **ctx)
{
    sage_scene_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        sage_entity_list_free(&hnd->ents);
        sage_payload_free(&hnd->cdata);
        sage_heap_free((void **) ctx);
    }
}


extern size_t
sage_scene_size(void)
{
    return sizeof (struct sage_scene_t);
}


extern sage_id_t
sage_scene_id(const sage_scene_t *ctx)
{
    sage_assert (ctx);
    return ctx->id;
}

    
extern const sage_entity_t *
sage_scene_entity(const sage_scene_t *ctx,
                  sage_id_t          id)
{
    sage_assert (ctx);
    return sage_entity_list_get(ctx->ents, id);
}


extern void
sage_scene_entity_set(sage_scene_t       *ctx,
                     sage_id_t           id,
                     const sage_entity_t *ent)
{
    sage_assert (ctx);
    sage_entity_list_set(ctx->ents, id, ent);
}


extern void
sage_scene_entity_push(sage_scene_t *ctx,
                       sage_id_t    cls,
                       sage_id_t    id)
{
    sage_assert (ctx);
    sage_entity_list_push(ctx->ents, cls, id);
}


extern void
sage_scene_entity_pop(sage_scene_t *ctx,
                      sage_id_t    id)
{
    sage_assert (ctx);
    sage_entity_list_pop(ctx->ents, id);
}


extern const sage_payload_t *
sage_scene_payload(const sage_scene_t *ctx)
{
    sage_assert (ctx);
    return ctx->cdata;
}


extern void 
sage_scene_start(sage_scene_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.start(ctx);
}


extern void 
sage_scene_stop(sage_scene_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.stop(ctx);
}


extern void 
sage_scene_update(sage_scene_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.update(ctx);
}


extern void 
sage_scene_draw(const sage_scene_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.draw(ctx);
}

