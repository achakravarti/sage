#include "./sage.h"


struct sage_entity_list_t {
    sage_entity_t **bfr;
    size_t        len;
    size_t        cap;
};


extern sage_entity_list_t *
sage_entity_list_new(void)
{
    sage_entity_list_t *ctx = sage_heap_new(sizeof *ctx);

    ctx->len = 0;
    ctx->cap = 4;
    
    ctx->bfr = sage_heap_new(sizeof *(ctx->bfr) * ctx->cap);
    for (register size_t i = 0; i < ctx->cap; i++)
        ctx->bfr[i] = NULL;

    return ctx;
}


extern const sage_entity_list_t *
sage_entity_list_copy(const sage_entity_list_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


extern sage_entity_list_t *
sage_entity_list_copy_deep(const sage_entity_list_t *ctx)
{
    sage_entity_list_t *cp = sage_entity_list_new();

    sage_assert (ctx);
    for (register size_t i = 0; i < ctx->len; i++)
        sage_entity_list_push(cp, sage_entity_class(ctx->bfr[i]), 
                              sage_entity_id(ctx->bfr[i]));

    return cp;
}


extern void
sage_entity_list_free(sage_entity_list_t **ctx)
{
    sage_entity_list_t *hnd;

    if sage_likely (ctx && (hnd = *ctx)) {
        for (register size_t i = 0; i < hnd->cap; i++)
            sage_entity_free(&hnd->bfr[i]);

        sage_heap_free((void **) hnd->bfr);
        sage_heap_free((void **) ctx);
    }
}


extern size_t
sage_entity_list_length(const sage_entity_list_t *ctx)
{
    sage_assert (ctx);
    return ctx->len;
}


extern size_t
sage_entity_list_capacity(const sage_entity_list_t *ctx)
{
    sage_assert (ctx);
    return ctx->cap;
}


extern const sage_entity_t *
sage_entity_list_get(const sage_entity_list_t *ctx,
                     sage_id_t                id)
{
    sage_assert (ctx && id);
    register size_t i;
    for (i = 0; i < ctx->len; i++) {
        if (sage_entity_id(ctx->bfr[i]) == id)
            return ctx->bfr[i];
    }

    /* entity not found */
    sage_assert (i < ctx->len);
    return NULL;
}


extern void
sage_entity_list_set(sage_entity_list_t  *ctx,
                     sage_id_t           id,
                     const sage_entity_t *ent)
{
    sage_assert (ctx && id && ent);
    register size_t i;
    for (i = 0; i < ctx->len; i++) {
        if (sage_entity_id(ctx->bfr[i]) == id) {
            sage_entity_free(&ctx->bfr[i]);
            ctx->bfr[i] = sage_entity_copy(ent);
            sage_entity_id_set(ctx->bfr[i], id);

            return;
        }
    }

    /* entity not found */
    sage_assert (i < ctx->len);
}


extern void
sage_entity_list_push(sage_entity_list_t *ctx,
                      sage_id_t          cls,
                      sage_id_t          id)
{
    sage_assert (ctx);
    if (sage_unlikely (ctx->len == ctx->cap)) {
        ctx->cap *= 2;
        ctx->bfr = sage_heap_resize(ctx->bfr, sizeof *ctx->bfr * ctx->cap);
    }

    ctx->bfr[ctx->len] = sage_entity_factory_spawn(cls);
    sage_entity_id_set(ctx->bfr[ctx->len], id);
}


extern void
sage_entity_list_pop(sage_entity_list_t *ctx,
                     sage_id_t          id)
{
    sage_assert (ctx);

    register size_t i;
    for (i = 0; i < ctx->len; i++) {
        if (sage_entity_id(ctx->bfr[i]) == id)
            break;
    }

    sage_entity_free(&ctx->bfr[i]);
    ctx->bfr[i] = ctx->bfr[ctx->len];
    ctx->bfr[ctx->len--] = NULL;
}


extern void
sage_entity_list_update(sage_entity_list_t *ctx)
{
    sage_assert (ctx);
    for (register size_t i = 0; i < ctx->len; i++)
        sage_entity_update(ctx->bfr[i]);
}


extern void
sage_entity_list_draw(const sage_entity_list_t *ctx)
{
    sage_assert (ctx);
    for (register size_t i = 0; i < ctx->len; i++)
        sage_entity_draw(ctx->bfr[i]);
}

