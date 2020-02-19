#include "../include/api.h"


struct __sage_object_t {
    struct sage_object_vtable_t vt;
    sage_id_t                   oid;
    sage_id_t                   iid;
    size_t                      nref;
    void                        *cdata;
};


static void copy_on_write(sage_object_t *ctx)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    sage_object_t hnd = *ctx;
    if (hnd->nref > 1) {
        sage_object_free(ctx);
        *ctx = sage_object_new(hnd->oid, hnd->iid, hnd->vt.copy(hnd->cdata),
                &hnd->vt);
    }
}


extern sage_object_t sage_object_new(const sage_id_t oid, const sage_id_t iid, 
        void *cdata, const struct sage_object_vtable_t *vt)
{
    sage_assert (vt);
    sage_assert (vt->copy);

    sage_object_t ctx = sage_heap_new(sizeof *ctx);

    ctx->oid = oid;
    ctx->iid = iid;
    ctx->nref = 1;
    ctx->cdata = cdata;

    ctx->vt.copy = vt->copy;
    ctx->vt.free = vt->free;

    return ctx;
}


extern sage_object_t sage_object_copy(const sage_object_t ctx)
{
    sage_assert (ctx);

    sage_object_t cp = (sage_object_t) ctx;
    cp->nref++;

    return cp;
}


extern void sage_object_free(sage_object_t *ctx)
{
    sage_object_t hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        if (!--hnd->nref && hnd->vt.free)
            hnd->vt.free(hnd->cdata);
    }

    *ctx = NULL;
}


extern sage_id_t sage_object_oid(const sage_object_t ctx)
{
    sage_assert (ctx);

    return ctx->oid;
}


extern sage_id_t sage_object_iid(const sage_object_t ctx)
{
    sage_assert (ctx);

    return ctx->iid;
}


extern void sage_object_iid_set(sage_object_t *ctx, const sage_id_t iid)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    copy_on_write(ctx);
    (*ctx)->iid = iid;
}


extern const void *sage_object_cdata(const sage_object_t ctx)
{
    sage_assert (ctx);

    return ctx->cdata;
}


extern void *sage_object_cdata_rw(sage_object_t *ctx)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    copy_on_write(ctx);
    return (*ctx)->cdata;
}

