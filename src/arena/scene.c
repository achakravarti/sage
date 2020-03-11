#include "arena.h"


struct list {
    sage_entity **bfr; /* array buffer of entities */
    size_t      len;   /* length of list           */
    size_t      cap;   /* capacity of list         */
};


struct sage_scene {
    sage_id                  id;
    struct list              *ents;
    sage_object             *payload;
    struct sage_scene_vtable vt;
};


struct cdata {
    sage_id scnid;
    sage_object *payload;
    struct list *ents;
    struct sage_scene_vtable vt;
};


static struct list *list_new(void)
{
    struct list *ctx = sage_heap_new(sizeof *ctx);
    ctx->len = 0;
    ctx->cap = 4;

    ctx->bfr = sage_heap_new(sizeof *(ctx->bfr) * ctx->cap);
    for (register size_t i = 0; i < ctx->cap; i++)
        ctx->bfr[i] = NULL;

    return ctx;
}


static void list_push(struct list *ctx, sage_id entid, sage_id scnid)
{
    sage_assert (ctx);
    if (sage_unlikely (ctx->len == ctx->cap)) {
        ctx->cap *= 2;
        ctx->bfr = sage_heap_resize(ctx->bfr, sizeof *ctx->bfr * ctx->cap);
    }

    ctx->bfr[ctx->len] = sage_entity_factory_clone(entid);
    sage_entity_id_scene_set(&ctx->bfr[ctx->len], scnid);
}


static struct list *list_copy(const struct list *ctx)
{
    struct list *cp = list_new();

    sage_assert (ctx);
    sage_entity *hnd;
    for (register size_t i = 0; i < ctx->len; i++) {
        hnd = ctx->bfr[i];
        list_push(cp, sage_entity_id(hnd), sage_entity_id_scene(hnd));
    }

    return cp;
}


static void list_free(struct list *ctx)
{
    if (sage_likely (ctx)) {
        for (register size_t i = 0; i < ctx->cap; i++)
            sage_entity_free(&ctx->bfr[i]);

        sage_heap_free((void **) &ctx->bfr);
        sage_heap_free((void **) &ctx);
    }
}


static inline void start_default(sage_scene **ctx)
{
    (void) ctx;
}


static inline void stop_default(sage_scene **ctx)
{
    (void) ctx;
}


static inline void update_default(sage_scene **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);

    for (register size_t i = 0; i < cd->ents->len; i++)
        sage_entity_update(&cd->ents->bfr[i]);
}


static inline void draw_default(const sage_scene *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    
    struct list *lst = cd->ents;
    for (register size_t i = 0; i < lst->len; i++)
        sage_entity_draw(lst->bfr[i]);
}




static struct cdata *cdata_new(sage_id scnid, sage_object *payload,
        const struct sage_scene_vtable *vt)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);

    ctx->scnid = scnid;
    ctx->ents = list_new();
    ctx->payload = sage_likely (payload) ? sage_object_copy(payload) : NULL;

    if (sage_likely (vt)) {
        ctx->vt.start = sage_likely (vt->start) ? vt->start : start_default;
        ctx->vt.stop = sage_likely(vt->stop) ? vt->stop : stop_default;
        ctx->vt.update = sage_unlikely (vt->update) 
                ? vt->update : update_default;
        ctx->vt.draw = sage_unlikely (vt->draw) ? vt->draw : draw_default;
    } else {
        ctx->vt.start = start_default;
        ctx->vt.stop = stop_default;
        ctx->vt.update = update_default;
        ctx->vt.draw = draw_default;
    }

    return ctx;
}




static void *cdata_copy(const void *ctx)
{
    sage_assert (ctx);
    const struct cdata *hnd = (const struct cdata *) ctx;

    struct cdata *cp = cdata_new(hnd->scnid, hnd->payload, &hnd->vt);
    cp->ents = list_copy(hnd->ents);

    return cp;
}




static void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);

    list_free(hnd->ents);
    sage_object_free(&hnd->payload);
    sage_heap_free((void **) ctx);
}




extern sage_scene *sage_scene_new(sage_id scnid, sage_object *payload,
        const struct sage_scene_vtable *vt)
{
    struct sage_object_vtable objvt = {
        .copy = &cdata_copy,
        .free = &cdata_free
    };

    sage_assert (scnid);
    return sage_object_new(SAGE_OBJECT_ID_SCENE, cdata_new(scnid, payload, vt),
            &objvt);
}


extern inline sage_scene *sage_scene_copy(const sage_scene *ctx);


extern inline void sage_scene_free(sage_scene **ctx);


extern size_t sage_scene_size(void)
{
    return sizeof (struct sage_scene);
}

extern sage_id sage_scene_id(const sage_scene *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return cd->scnid;
}

    
extern sage_entity *sage_scene_entity(const sage_scene *ctx, sage_id id)
{
    sage_assert (ctx);
    const struct cdata * cd = sage_object_cdata(ctx);

    struct list *lst = cd->ents;

    register size_t i;
    sage_assert (id);
    for (i = 0; i < lst->len; i++) {
        if (sage_entity_id_scene(lst->bfr[i]) == id)
            return sage_entity_copy(lst->bfr[i]);
    }

    /* entity not found */
    sage_assert (i < lst->len);
    return NULL;
}

extern void sage_scene_entity_set(sage_scene **ctx, sage_id id, 
        sage_entity *ent)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);
    struct list *lst = cd->ents;

    sage_assert (id && ent);
    register size_t i = 0;
    for (; i < lst->len; i++) {
        if (sage_entity_id_scene(lst->bfr[i]) == id) {
            sage_entity_free(&lst->bfr[i]);
            lst->bfr[i] = ent;
            sage_entity_id_scene_set(&lst->bfr[i], id);

            return;
        }
    }

    /* entity not found */
    sage_assert (i < cd->ents->len);
}


extern void sage_scene_entity_push(sage_scene **ctx, sage_id entid, sage_id id)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);
    list_push(cd->ents, entid, id);
}


extern void sage_scene_entity_pop(sage_scene **ctx, sage_id id)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);
    struct list *lst = cd->ents;

    sage_assert (id);
    register size_t i = 0;
    for (; i < lst->len; i++) {
        if (sage_entity_id_scene(lst->bfr[i]) == id)
            break;
    }

    sage_entity_free(&lst->bfr[i]);
    cd->ents->bfr[i] = lst->bfr[lst->len];
    cd->ents->bfr[lst->len--] = NULL;

    /* entity not found */
    sage_assert (i < lst->len);
}


extern sage_object *sage_scene_payload(const sage_scene *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return sage_object_copy(cd->payload);
}


extern void sage_scene_start(sage_scene **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);
    cd->vt.start(ctx);
}


extern void sage_scene_stop(sage_scene **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);
    cd->vt.stop(ctx);
}



extern void sage_scene_update(sage_scene **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutate(ctx);
    cd->vt.update(ctx);
}


extern void sage_scene_draw(const sage_scene *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    cd->vt.draw(ctx);
}

