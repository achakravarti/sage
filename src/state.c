#include "./sage.h"


struct sage_state_t {
    sage_id_t                  id;
    sage_entity_list_t         *ents;
    sage_payload_t             *cdata;
    struct sage_state_vtable_t vt;
};


void start_default(sage_state_t *ctx)
{
    (void) ctx;
}


void stop_default(sage_state_t *ctx)
{
    (void) ctx;
}


void update_default(sage_state_t *ctx)
{
    sage_assert (ctx);
    sage_entity_list_update(ctx->ents);
}


void draw_default(const sage_state_t *ctx)
{
    sage_assert (ctx);
    sage_entity_list_draw(ctx->ents);
}


extern sage_state_t *
sage_state_new(sage_id_t                        id,
               sage_payload_t                   *cdata,
               const struct sage_state_vtable_t *vt)
{
    sage_state_t *ctx = sage_heap_new(sizeof *ctx);

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


extern const sage_state_t *
sage_state_copy(const sage_state_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


extern sage_state_t *
sage_state_copy_deep(const sage_state_t *ctx)
{
    sage_assert (ctx);
    sage_state_t *cp = sage_state_new(ctx->id, ctx->cdata, &ctx->vt);
    cp->ents = sage_entity_list_copy_deep(ctx->ents);

    return cp;
}


extern void
sage_state_free(sage_state_t **ctx)
{
    sage_state_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        sage_entity_list_free(&hnd->ents);
        sage_payload_free(&hnd->cdata);
        sage_heap_free((void **) ctx);
    }
}


extern size_t
sage_state_size(void)
{
    return sizeof (struct sage_state_t);
}


extern sage_id_t
sage_state_id(const sage_state_t *ctx)
{
    sage_assert (ctx);
    return ctx->id;
}


extern const sage_entity_list_t *
sage_state_entities(const sage_state_t *ctx)
{
    sage_assert (ctx);
    return sage_entity_list_copy(ctx->ents);
}


extern const sage_payload_t *
sage_state_payload(const sage_state_t *ctx)
{
    sage_assert (ctx);
    return ctx->cdata;
}


extern void 
sage_state_start(sage_state_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.start(ctx);
}


extern void 
sage_state_stop(sage_state_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.stop(ctx);
}


extern void 
sage_state_update(sage_state_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.update(ctx);
}


extern void 
sage_state_draw(const sage_state_t *ctx)
{
    sage_assert (ctx);
    ctx->vt.draw(ctx);
}

