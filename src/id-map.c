#include <string.h>
#include "sage.h"


struct node_t {
    sage_id_t key;
    void *val;
    struct node_t *next;
};


struct sage_id_map_t {
    size_t len;
    size_t sz;
    sage_id_map_free_f *free;
    struct node_t **buck;
};


static inline void
free_default(void *ctx)
{
    (void) ctx;
}


extern sage_id_map_t *
sage_id_map_new(size_t buck, size_t sz, sage_id_map_free_f *free)
{
    sage_id_map_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->len = buck;
    ctx->sz = sz;
    ctx->free = free ? free : free_default;

    sage_require (ctx->buck = malloc (sizeof *ctx->buck * ctx->len));
    for (register size_t i = 0; i < ctx->len; i++)
        ctx->buck [i] = NULL;

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
                ctx->free (buck->val);
                free (buck);
                buck = next;
            } while (buck);
        }
    }

    free (ctx->buck);
    free (ctx);

    return NULL;
}


extern size_t
sage_id_map_hash(const sage_id_map_t *ctx, sage_id_t key)
{
    return key % ctx->len;
}


static void *
value_copy(const void *val, size_t sz)
{
    void *cp;
    sage_require (cp = malloc (sz));
    sage_require (memcpy (cp, val, sz));
    return cp;
}


extern void *
sage_id_map_value(sage_id_map_t *ctx, sage_id_t key)
{
    struct node_t *buck = ctx->buck [sage_id_map_hash (ctx, key)];
    struct node_t *itr = buck;
    
    while (itr) {
        if (itr->key == key)
            return value_copy(itr->val, ctx->sz);

        itr = itr->next;
    }

    // TODO: id not found, so abort; think of cleaner code
    sage_assert (itr != NULL);
    return NULL;
}


extern void
sage_id_map_value_set(sage_id_map_t *ctx, sage_id_t key, const void *val)
{
    size_t hash = sage_id_map_hash (ctx, key);
    struct node_t *buck = ctx->buck [hash];

    struct node_t *itr = buck;
    while (itr) {
        if (itr->key == key) {
            ctx->free (itr->val);
            itr->val = value_copy (val, ctx->sz);
            return;
        }

        itr = itr->next;
    }

    struct node_t *add;
    sage_require (add = malloc (sizeof *add));
    add->key = key;
    add->val = value_copy (val, ctx->sz);
    add->next = buck;
    ctx->buck [hash] = add;
}

