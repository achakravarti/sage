#include "./sage.h"


struct sage_payload_t {
    void                         *data;
    size_t                       sz;
    struct sage_payload_vtable_t vt;
};


extern sage_payload_t *
sage_payload_new(const void                         *data,
                 size_t                             sz,
                 const struct sage_payload_vtable_t *vt)
{
    sage_payload_t *ctx = sage_heap_new(sizeof *ctx);

    sage_assert (vt && vt->copy_deep && vt->free);
    ctx->vt.copy_deep = vt->copy_deep;
    ctx->vt.free = vt->free;

    sage_assert (sz && data);
    ctx->sz = sz;
    ctx->data = vt->copy_deep(data);

    return ctx;
}


extern const sage_payload_t *
sage_payload_copy(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


extern sage_payload_t *
sage_payload_copy_deep(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return sage_payload_new(ctx->data, ctx->sz, &ctx->vt);
}


extern void 
sage_payload_free(sage_payload_t **ctx)
{
    sage_assert (ctx && *ctx);
    sage_payload_t *hnd = *ctx;
    hnd->vt.free(&hnd->data);
    sage_heap_free((void **) ctx);
}


extern size_t
sage_payload_size(void)
{
    return sizeof (struct sage_payload_t);
}


extern size_t 
sage_payload_size_data(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx->sz;
}


extern const void *
sage_payload_data(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx->data;
}


extern void 
sage_payload_data_set(sage_payload_t *ctx,
                      const void     *data)
{
    sage_assert (ctx && data);
    ctx->vt.free(&ctx->data);
    ctx->data = ctx->vt.copy_deep(data);
}

