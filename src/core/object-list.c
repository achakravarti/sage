#include "core.h"


struct cdata {
    sage_object **lst;
    size_t len;
    size_t cap;
};


static struct cdata *cdata_new(void)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);

    ctx->len = 0;
    ctx->cap = 4;

    ctx->lst = sage_heap_new(sizeof *(ctx->lst) * ctx->cap);
    for (register size_t i = 0; i < ctx->cap; i++)
        ctx->lst[i] = NULL;

    return ctx;
}


static void *cdata_copy(const void *ctx)
{
    sage_assert (ctx);
    const struct cdata *hnd = (const struct cdata *) ctx;

    struct cdata *cp = cdata_new();
    
    for (register size_t i = 0; i < hnd->len; i++) {
        if (sage_unlikely (cp->len == cp->cap)) {
            cp->cap *= 2;
            cp->lst = sage_heap_resize(cp->lst, sizeof *cp->lst * cp->cap);
        }

        cp->lst[i] = sage_object_copy(hnd->lst[i]);
    }

    return cp;
}


static void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);

    for (register size_t i = 0; i < hnd->cap; i++)
        sage_object_free(&hnd->lst[i]);

    sage_heap_free((void **) &hnd->lst);
    sage_heap_free((void **) &ctx);
}


extern sage_object_list *sage_object_list_new(void)
{
    struct sage_object_vtable vt = { .copy = &cdata_copy, .free = &cdata_free };
    return sage_object_new(0, cdata_new(), &vt);
}


extern inline sage_object_list *sage_object_list_copy(
        const sage_object_list *ctx);


extern inline void sage_object_list_free(sage_object_list **ctx);


extern size_t sage_object_list_len(const sage_object_list *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return cd->len;
}


extern size_t sage_object_list_find(const sage_object_list *ctx, sage_id id)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    sage_assert (id);
    for (register size_t i = 0; i < cd->len; i++) {
        if (sage_object_id(cd->lst[i]) == id)
            return i + 1;
    }

    return 0;
}


extern sage_object *sage_object_list_get(const sage_object_list *ctx, 
        sage_id id)
{
    sage_assert (ctx && id);
    size_t idx = sage_object_list_find(ctx, id);
    
    sage_require (idx);
    return sage_object_list_get_at(ctx, idx);
}


extern sage_object *sage_object_list_get_at(const sage_object_list *ctx,
        size_t idx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    sage_assert (idx && idx <= cd->len);
    return sage_object_copy(cd->lst[idx - 1]);
}


extern void sage_object_list_set(sage_object_list **ctx, sage_id id,
        const sage_object *obj)
{
    sage_assert (ctx && id);
    size_t idx = sage_object_list_find(*ctx, id);

    sage_require (idx);
    sage_object_list_set_at(ctx, idx, obj);
}


extern void sage_object_list_set_at(sage_object_list **ctx, size_t idx,
        const sage_object *obj)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    sage_assert (idx <= cd->len);
    size_t index = idx - 1;
    sage_object_free(&cd->lst[index]);

    sage_assert (obj);
    cd->lst[index] = sage_object_copy(obj);
}


extern void sage_object_list_push(sage_object_list **ctx, 
        const sage_object *obj)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    if (sage_unlikely (cd->len == cd->cap)) {
        cd->cap *= 2;
        cd->lst = sage_heap_resize(cd->lst, sizeof *cd->lst * cd->cap);
    }

    sage_assert (obj && sage_object_id(obj));
    cd->lst[cd->len] = sage_object_copy(obj);
}


extern void sage_object_list_pop(sage_object_list **ctx, sage_id id)
{
    sage_assert (ctx && id);
    size_t idx = sage_object_list_find(*ctx, id);

    sage_require (idx);
    sage_object_list_pop_at(ctx, idx);
}


extern void sage_object_list_pop_at(sage_object_list **ctx, size_t idx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    sage_assert (idx);
    sage_object_free(&cd->lst[--idx]);
    cd->lst[idx] = cd->lst[cd->len];
    cd->lst[cd->len--] = NULL;
}

