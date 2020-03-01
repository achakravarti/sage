#include "core.h"


struct sage_object {
    struct sage_object_vtable vt;
    sage_id id;
    size_t nref;
    void *cdata;
};


static void copy_on_write(sage_object **ctx)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    sage_object *hnd = *ctx;

    if (hnd->nref > 1) {
        sage_object *cp = sage_object_new(hnd->id, hnd->vt.copy(hnd->cdata),
                &hnd->vt);
        sage_object_free(ctx);
        *ctx = cp;
    }
}


extern sage_object *sage_object_new(sage_id id, void *cdata,
        const struct sage_object_vtable *vt)
{
    sage_assert (vt);
    sage_assert (vt->copy);
    sage_assert (vt->free);

    sage_object *ctx = sage_heap_new(sizeof *ctx);

    ctx->id = sage_id_copy(id);
    ctx->nref = 1;
    ctx->cdata = cdata;

    ctx->vt.copy = vt->copy;
    ctx->vt.free = vt->free;

    return ctx;
}


extern sage_object *sage_object_copy(const sage_object *ctx)
{
    sage_assert (ctx);

    sage_object *cp = (sage_object *) ctx;
    cp->nref++;

    return cp;
}


extern void sage_object_free(sage_object **ctx)
{
    sage_object *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        if (!--hnd->nref) {
            hnd->vt.free(&hnd->cdata);
            sage_heap_free((void **) ctx);
        }
    }
}


extern size_t sage_object_size(void)
{
    return sizeof (struct sage_object);
}


extern sage_id sage_object_id(const sage_object *ctx)
{
    sage_assert (ctx);
    return sage_id_copy(ctx->id);
}


extern void sage_object_id_set(sage_object **ctx, sage_id id)
{
    sage_assert (ctx && *ctx);
    copy_on_write(ctx);
    (*ctx)->id = sage_id_copy(id);
}


extern const void *sage_object_cdata(const sage_object *ctx)
{
    sage_assert (ctx);
    return ctx->cdata;
}


extern void *sage_object_cdata_mutate(sage_object **ctx)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    copy_on_write(ctx);
    return (*ctx)->cdata;
}

