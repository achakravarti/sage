#include "arena.h"


struct cdata {
    sage_entity **lst;
    size_t len;
    size_t cap;
};


struct cdata *cdata_new(void)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);

    ctx->len = 0;
    ctx->cap = 4;

    ctx->lst = sage_heap_new(sizeof *(ctx->lst) * ctx->cap);
    for (register size_t i = 0; i < ctx->cap; i++)
        ctx->lst[i] = NULL;

    return ctx;
}


void *cdata_copy(const void *ctx)
{
    sage_assert (ctx);
    const struct cdata *hnd = (const struct cdata *) ctx;

    struct cdata *cp = cdata_new();
    
    for (register size_t i = 0; i < hnd->len; i++) {
        if (sage_unlikely (cp->len == cp->cap)) {
            cp->cap *= 2;
            cp->lst = sage_heap_resize(cp->lst, sizeof *cp->lst * cp->cap);
        }

        cp->lst[i] = sage_entity_copy(hnd->lst[i]);
    }

    return cp;
}


void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);

    for (register size_t i = 0; i < hnd->cap; i++)
        sage_entity_free(&hnd->lst[i]);

    sage_heap_free((void **) &hnd->lst);
    sage_heap_free((void **) &ctx);
}


extern sage_entity_list *sage_entity_list_new(void)
{
    struct sage_object_vtable vt = { .copy = &cdata_copy, .free = &cdata_free };
    return sage_object_new(SAGE_OBJECT_ID_ENTITY_LIST, cdata_new(), &vt);
}


extern inline sage_entity_list *sage_entity_list_copy(
        const sage_entity_list *ctx);


extern inline void sage_entity_list_free(sage_entity_list **ctx);


extern inline enum sage_object_id sage_entity_list_id_object(
        const sage_entity_list *ctx);


extern void sage_entity_list_push(sage_entity_list **ctx, 
        const sage_entity *ent)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);

    if (sage_unlikely (cd->len == cd->cap)) {
        cd->cap *= 2;
        cd->lst = sage_heap_resize(cd->lst, sizeof *cd->lst * cd->cap);
    }

    sage_assert (ent && sage_entity_id_scene(ent));
    cd->lst[cd->len] = sage_entity_copy(ent);
}


extern void sage_entity_list_pop(sage_entity_list **ctx, sage_id eid)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);

    sage_assert (eid);
    register size_t i = 0;
    for (; i < cd->len; i++) {
        if (sage_entity_id_scene(cd->lst[i]) == eid) {
            sage_entity_free(&cd->lst[i]);
            cd->lst[i] = cd->lst[cd->len];
            cd->lst[cd->len--] = NULL;
            break;
        }
    }

    /* entity with instance id not found */
    sage_assert (i < cd->len);
}



