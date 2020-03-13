#include "arena.h"


struct cdata {
    sage_object *payload;
    sage_entity_list *ents;
    struct sage_scene_vtable vt;
};


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
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    sage_entity *ent;
    for (register size_t i = 1; i <= sage_entity_list_len(cd->ents); i++) {
        ent = sage_entity_list_get(cd->ents, i);
        sage_entity_update(&ent);
    }
}


static inline void draw_default(const sage_scene *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    for (register size_t i = 1; i <= sage_entity_list_len(cd->ents); i++)
        sage_entity_draw(sage_entity_list_get(cd->ents, i));
}




static struct cdata *cdata_new(sage_object *payload,
        const struct sage_scene_vtable *vt)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);

    ctx->ents = sage_entity_list_new();
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

    struct cdata *cp = cdata_new(hnd->payload, &hnd->vt);
    cp->ents = sage_entity_list_copy(hnd->ents);

    return cp;
}




static void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);

    sage_entity_list_free(&hnd->ents);
    sage_object_free(&hnd->payload);
    sage_heap_free((void **) ctx);
}




extern sage_scene *sage_scene_new(sage_id id, sage_object *payload,
        const struct sage_scene_vtable *vt)
{
    struct sage_object_vtable objvt = {
        .copy = &cdata_copy,
        .free = &cdata_free
    };

    sage_assert (id);
    return sage_object_new(id, cdata_new(payload, vt), &objvt);
}


extern inline sage_scene *sage_scene_copy(const sage_scene *ctx);


extern inline void sage_scene_free(sage_scene **ctx);


extern inline sage_id sage_scene_id(const sage_scene *ctx);

    
extern sage_entity *sage_scene_entity(const sage_scene *ctx, sage_id guid)
{
    sage_assert (ctx);
    const struct cdata * cd = sage_object_cdata(ctx);

    sage_assert (guid);
    return sage_entity_list_get(cd->ents, guid);
}

extern void sage_scene_entity_set(sage_scene **ctx, sage_id guid, 
        sage_entity *ent)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    sage_assert (guid);
    sage_entity_list_set(&cd->ents, guid, ent);
}


extern void sage_scene_entity_push(sage_scene **ctx, sage_id entid, 
        sage_id guid)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    
    sage_entity *ent = sage_entity_factory_clone(entid);
    sage_entity_guid_set(&ent, guid);
    sage_entity_list_push(&cd->ents, ent);
}


extern void sage_scene_entity_pop(sage_scene **ctx, sage_id guid)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    sage_entity_list_pop(&cd->ents, guid);
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
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->vt.start(ctx);
}


extern void sage_scene_stop(sage_scene **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->vt.stop(ctx);
}



extern void sage_scene_update(sage_scene **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->vt.update(ctx);
}


extern void sage_scene_draw(const sage_scene *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    cd->vt.draw(ctx);
}

