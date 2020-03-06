#include "arena.h"


struct list {
    sage_entity **bfr; /* array buffer of entities */
    size_t      len;   /* length of list           */
    size_t      cap;   /* capacity of list         */
};


struct sage_scene {
    sage_id                  id;
    struct list              *ents;
    sage_payload_t             *cdata;
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


static inline void start_default(sage_scene *ctx)
{
    (void) ctx;
}


static inline void stop_default(sage_scene *ctx)
{
    (void) ctx;
}


static inline void update_default(sage_scene *ctx)
{
    sage_assert (ctx);
    for (register size_t i = 0; i < ctx->ents->len; i++)
        sage_entity_update(&ctx->ents->bfr[i]);
}


static inline void draw_default(const sage_scene *ctx)
{
    sage_assert (ctx);
    struct list *lst = ctx->ents;

    for (register size_t i = 0; i < lst->len; i++)
        sage_entity_draw(lst->bfr[i]);
}


extern sage_scene *sage_scene_new(sage_id id, sage_payload_t *cdata,
        const struct sage_scene_vtable *vt)
{
    sage_scene *ctx = sage_heap_new(sizeof *ctx);

    ctx->id = id;
    ctx->ents = list_new();
    ctx->cdata = sage_payload_copy(cdata);

    sage_assert (vt);
    ctx->vt.start = sage_likely (vt->start) ? vt->start : start_default;
    ctx->vt.stop = sage_likely (vt->stop) ? vt->stop : stop_default;
    ctx->vt.update = sage_likely (vt->update) ? vt->update : update_default;
    ctx->vt.draw = sage_likely (vt->draw) ? vt->draw : draw_default;

    return ctx;
}


extern inline sage_scene *sage_scene_move(sage_scene *ctx);


extern inline const sage_scene *sage_scene_link(const sage_scene *ctx);


extern sage_scene *sage_scene_copy(const sage_scene *ctx)
{
    sage_assert (ctx);
    sage_scene *cp = sage_scene_new(ctx->id, ctx->cdata, &ctx->vt);
    cp->ents = list_copy(ctx->ents);

    return cp;
}


extern void sage_scene_free(sage_scene **ctx)
{
    sage_scene *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        list_free(hnd->ents);
        sage_payload_free(&hnd->cdata);
        sage_heap_free((void **) ctx);
    }
}


extern size_t sage_scene_size(void)
{
    return sizeof (struct sage_scene);
}


extern sage_id sage_scene_id(const sage_scene *ctx)
{
    sage_assert (ctx);
    return ctx->id;
}

    
extern const sage_entity *sage_scene_entity(const sage_scene *ctx, sage_id id)
{
    sage_assert (ctx);
    struct list *lst = ctx->ents;

    register size_t i;
    sage_assert (id);
    for (i = 0; i < lst->len; i++) {
        if (sage_entity_id_scene(lst->bfr[i]) == id)
            return lst->bfr[i];
    }

    /* entity not found */
    sage_assert (i < lst->len);
    return NULL;
}


extern void sage_scene_entity_set(sage_scene *ctx, sage_id id, sage_entity *ent)
{
    sage_assert (ctx);
    sage_assert (id);
    sage_assert (ent);

    struct list *lst = ctx->ents;
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
    sage_assert (i < ctx->ents->len);
}


extern void sage_scene_entity_push(sage_scene *ctx, sage_id entid,
        sage_id scnid)
{
    sage_assert (ctx);
    list_push(ctx->ents, entid, scnid);
}


extern void sage_scene_entity_pop(sage_scene *ctx, sage_id id)
{
    sage_assert (ctx);
    sage_assert (id);

    struct list *lst = ctx->ents;
    register size_t i = 0;
    for (; i < lst->len; i++) {
        if (sage_entity_id_scene(lst->bfr[i]) == id)
            break;
    }

    sage_entity_free(&lst->bfr[i]);
    ctx->ents->bfr[i] = lst->bfr[lst->len];
    ctx->ents->bfr[lst->len--] = NULL;

    /* entity not found */
    sage_assert (i < lst->len);
}


extern const sage_payload_t *sage_scene_payload(const sage_scene *ctx)
{
    sage_assert (ctx);
    return ctx->cdata;
}


extern void sage_scene_start(sage_scene *ctx)
{
    sage_assert (ctx);
    ctx->vt.start(ctx);
}


extern void sage_scene_stop(sage_scene *ctx)
{
    sage_assert (ctx);
    ctx->vt.stop(ctx);
}


extern void sage_scene_update(sage_scene *ctx)
{
    sage_assert (ctx);
    ctx->vt.update(ctx);
}


extern void sage_scene_draw(const sage_scene *ctx)
{
    sage_assert (ctx);
    ctx->vt.draw(ctx);
}

