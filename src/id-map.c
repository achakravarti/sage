#include <string.h>
#include "utility.h"


struct node_t {
    sage_id_t key;
    void *val;
    struct node_t *next;
};


struct sage_id_map_t {
    size_t len;
    size_t sz;
    struct sage_id_map_vtable_t vt;
    struct node_t **buck;
};


static inline void
free_default(void *ctx)
{
    (void) ctx;
}


extern sage_id_map_t *
sage_id_map_new(size_t buck,
                size_t sz,
                struct sage_id_map_vtable_t *vt)
{
    sage_id_map_t *ctx = sage_heap_new (sizeof *ctx);

    sage_assert (buck);
    ctx->len = buck;

    sage_assert (sz);
    ctx->sz = sz;

    sage_assert (vt && vt->copy && vt->free);
    ctx->vt.copy = vt->copy;
    ctx->vt.free = vt->free;

    ctx->buck = sage_heap_new (sizeof *ctx->buck * ctx->len);
    for (register size_t i = 0; i < ctx->len; i++) {
        ctx->buck [i] = NULL;
    }

    return ctx;
}


extern sage_id_map_t *
sage_id_map_free(sage_id_map_t *ctx)
{
    if (sage_unlikely (!ctx))
        return NULL;

    struct node_t *buck, *next;
    for (register size_t i = 0; i < ctx->len; i++) {
        if ((buck = ctx->buck [i])) {
            do {
                next = buck->next;
                ctx->vt.free (&buck->val);
                sage_heap_free ((void **) &buck);
                buck = next;
            } while (buck);
        }
    }

    sage_heap_free ((void **) &ctx->buck);
    free (ctx);

    return NULL;
}


extern size_t
sage_id_map_hash(const sage_id_map_t *ctx, sage_id_t key)
{
    return key % ctx->len;
}


extern void *
sage_id_map_value(sage_id_map_t *ctx, sage_id_t key)
{
    struct node_t *buck = ctx->buck [sage_id_map_hash (ctx, key)];
    struct node_t *itr = buck;
   
    while (itr) {
        if (itr->key == key) {
            return ctx->vt.copy (itr->val);
        }

        itr = itr->next;
    }

    // TODO: id not found, so abort; think of cleaner code
    sage_assert (itr != NULL);
    return NULL;
}


extern void
sage_id_map_value_set(sage_id_map_t *ctx, sage_id_t key, void *val)
{
    size_t hash = sage_id_map_hash (ctx, key);
    struct node_t *buck = ctx->buck [hash];

    struct node_t *itr = buck;
    while (itr) {
        if (itr->key == key) {
            ctx->vt.free (&itr->val);
            itr->val = val;
            return;
        }

        itr = itr->next;
    }

    struct node_t *add = sage_heap_new (sizeof *add);
    add->key = key;
    add->val = val;
    add->next = buck;
    ctx->buck [hash] = add;
}

