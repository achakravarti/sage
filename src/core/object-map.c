#include "core.h"


struct node {
    sage_id key;
    sage_object *val;
    struct node *next;
};


struct sage_object_map {
    size_t len;
    struct node **buck;
};


extern sage_object_map *sage_object_map_new(size_t buck)
{
    sage_object_map *ctx = sage_heap_new(sizeof *ctx);

    sage_assert (buck);
    ctx->len = buck;

    ctx->buck = sage_heap_new(sizeof *ctx->buck * ctx->len);
    for (register size_t i = 0; i < ctx->len; i++)
        ctx->buck[i] = NULL;

    return ctx;
}


extern void sage_object_map_free(sage_object_map **ctx)
{
    sage_object_map *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        struct node *buck, *next;

        for (register size_t i = 0; i < hnd->len; i++) {
            if ((buck = hnd->buck[i])) {
                do {
                    next = buck->next;
                    sage_object_free(&buck->val);
                    sage_heap_free((void **) &buck);
                    buck = next;
                } while (buck);
            }
        }

        sage_heap_free((void **) &hnd->buck);
        sage_heap_free((void **) ctx);
    }
}


extern size_t sage_object_map_hash(const sage_object_map *ctx, sage_id key)
{
    sage_assert (ctx && key);
    return key % ctx->len;
}


extern sage_object *sage_object_map_value(const sage_object_map *ctx, 
        sage_id key)
{
    sage_assert (ctx && key);
    struct node *itr= ctx->buck[sage_object_map_hash(ctx, key)];
    while (itr) {
        if (itr->key == key)
            return sage_object_copy(itr->val);

        itr = itr->next;
    }

    // TODO: key not found, so abort; think of cleaner code
    sage_assert (itr != NULL);
    return NULL; 
}


extern void sage_object_map_value_set(sage_object_map *ctx, sage_id key, 
        const sage_object *val)
{
    sage_assert (ctx && key);
    size_t hash = sage_object_map_hash(ctx, key);

    struct node *itr = ctx->buck[hash];
    while (itr) {
        if (itr->key == key) {
            sage_object_free(&itr->val);
            itr->val = sage_object_copy(val);
            return;
        }
    }

    itr = sage_heap_new(sizeof *itr);
    itr->key = key;
    itr->val = sage_object_copy(val);
    itr->next = ctx->buck[hash];
    ctx->buck[hash] = itr;
}

