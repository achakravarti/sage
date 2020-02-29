#include <string.h>
#include "utility.h"


struct sage_string_t {
    char   *bfr;
    size_t len;
    size_t sz;
    size_t nref;
};


extern sage_string_t *sage_string_new(const char *str)
{
    sage_assert (str);

    sage_string_t *ctx = sage_heap_new(sizeof *ctx);

    ctx->len = strlen(str);
    ctx->sz = sizeof *ctx;
    ctx->nref = 1;

    ctx->bfr = sage_heap_new(ctx->len + 1);
    strncpy(ctx->bfr, str, ctx->len);

    return ctx;
}


extern sage_string_t *sage_string_copy(const sage_string_t *ctx)
{
    sage_assert (ctx);

    sage_string_t *cp = (sage_string_t *) ctx;
    cp->nref++;

    return cp;
}


extern void sage_string_free(sage_string_t **ctx)
{
    if (ctx) {
        sage_string_t *hnd = *ctx;
        if (hnd && !--hnd->nref) {
            sage_heap_free((void **) &hnd->bfr);
            sage_heap_free((void **) ctx);
        }
    }
}


extern size_t sage_string_len(const sage_string_t *ctx)
{
    sage_assert (ctx);

    return ctx->len;
}


extern size_t sage_string_size(const sage_string_t *ctx)
{
    sage_assert (ctx);

    return ctx->sz;
}


extern size_t sage_string_refcount(const sage_string_t *ctx)
{
    sage_assert (ctx);

    return ctx->nref;
}


extern const char *sage_string_cstr(const sage_string_t *ctx)
{
    sage_assert (ctx);

    return ctx->bfr;
}

