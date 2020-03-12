/******************************************************************************
 *                           ____   __    ___  ____ 
 *                          / ___) / _\  / __)(  __)
 *                          \___ \/    \( (_ \ ) _) 
 *                          (____/\_/\_/ \___/(____)
 *
 * Schemable? Game Engine (SAGE) Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>.
 *
 * This code is released under the MIT License. See the accompanying
 * sage/LICENSE.md file or <http://opensource.org/licenses/MIT> for complete 
 * licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT THEM.
 *
 * This is the sage/src/entity.c source file; it implements the entity API of 
 * the SAGE Library.
 ******************************************************************************/


/*
 * The sage/include/api.h header file contains the declaration of the API of the
 * SAGE Library.
 */
#include "../core/core.h"
#include "../graphics/graphics.h"
#include "../hid/hid.h"
#include "arena.h"


struct cdata {
    sage_id entid;
    sage_id scnid;
    sage_vector *pos;
    sage_sprite *spr;
    sage_object *payload;
    struct sage_entity_vtable vt;
};


static inline void update_default2(sage_entity **ctx)
{
    (void) ctx;
}


static inline void draw_default2(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    if (sage_likely (sage_vector_visible(cd->pos)))
        sage_sprite_draw(cd->spr, sage_vector_point(cd->pos));
}


static struct cdata *cdata_new(sage_id entid, sage_id texid, 
        struct sage_frame_t frm, const sage_object *payload, 
        const struct sage_entity_vtable *vt)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);

    ctx->entid = entid;
    ctx->scnid = (sage_id) 0;
    ctx->pos = sage_vector_new_zero();
    ctx->spr = sage_sprite_new(texid, frm);
    ctx->payload = sage_likely (payload) ? sage_object_copy(payload) : NULL;

    ctx->vt.update = sage_likely (vt->update) ? vt->update : &update_default2;
    ctx->vt.draw = sage_likely (vt->draw) ? vt->draw : &draw_default2;

    return ctx;
}


static void *cdata_copy(const void *ctx)
{
    const struct cdata *hnd = (const struct cdata *) ctx;

    struct cdata *cp = sage_heap_new(sizeof *cp);

    cp->entid = hnd->entid;
    cp->scnid = hnd->scnid;
    cp->pos = sage_vector_copy(hnd->pos);
    cp->spr = sage_sprite_copy(hnd->spr);
    cp->payload = sage_likely (hnd->payload) ? sage_object_copy(hnd->payload)
        : NULL;

    cp->vt.update = hnd->vt.update;
    cp->vt.draw = hnd->vt.draw;

    return cp;
}


static void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);

    sage_object_free(&hnd->payload);
    sage_vector_free(&hnd->pos);
    sage_sprite_free(&hnd->spr);
}


extern sage_entity *sage_entity_new(sage_id entid, sage_id texid, 
        struct sage_frame_t frm, const sage_object *payload, 
        const struct sage_entity_vtable *vt)
{
    struct sage_object_vtable objvt = {
        .copy = &cdata_copy,
        .free = &cdata_free
    };
    
    sage_assert (entid && texid && vt);
    return sage_object_new(SAGE_OBJECT_ID_ENTITY, cdata_new(entid, texid, frm,
            payload, vt), &objvt);
}


extern sage_entity *sage_entity_new_default(sage_id entid, sage_id texid,
        struct sage_frame_t frm)
{
    struct sage_object_vtable objvt = { 
        .copy = &cdata_copy, 
        .free = &cdata_free 
    };
    struct sage_entity_vtable entvt = { .update = NULL, .draw = NULL };

    sage_assert (entid && texid);
    return sage_object_new(SAGE_OBJECT_ID_ENTITY, cdata_new(entid, texid, frm,
            NULL, &entvt), &objvt);
}


extern inline sage_entity *sage_entity_copy(const sage_entity *ctx);


extern inline void sage_entity_free(sage_entity **ctx);


extern sage_id sage_entity_id(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return cd->entid;
}


extern inline enum sage_object_id sage_entity_id_object_(
        const sage_entity *ctx);


extern sage_id sage_entity_id_scene(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return cd->scnid;
}


extern void sage_entity_id_scene_set(sage_entity **ctx, sage_id id)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->scnid = id;
}


extern sage_vector *sage_entity_position(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return sage_vector_copy(cd->pos);
}


extern void sage_entity_position_set(sage_entity **ctx, 
        const sage_vector *pos)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    sage_assert (pos);
    sage_vector_free(&cd->pos);
    cd->pos = sage_vector_copy(pos);
}


extern void sage_entity_move(sage_entity **ctx, const sage_vector *vel)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    sage_assert (vel);
    sage_vector_add(&cd->pos, vel);
}


extern const sage_object *sage_entity_payload(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return cd->payload;
}


extern sage_object *sage_entity_payload_mutable(sage_entity **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    return cd->payload;
}


extern bool sage_entity_focused(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    struct sage_area_t frm = sage_sprite_area_frame(cd->spr);
    sage_vector *add = sage_vector_new((float) frm.w, (float) frm.h);

    sage_vector *se = sage_vector_copy(cd->pos);
    sage_vector_add(&se, add);
    sage_vector_free(&add);

    sage_vector *aim = sage_mouse_vector();
    bool focused = sage_vector_gteq(aim, cd->pos) && sage_vector_lteq(aim, se);

    sage_vector_free(&aim);
    sage_vector_free(&se);

    return focused;
}


extern void sage_entity_frame(sage_entity **ctx, struct sage_frame_t frm)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    sage_sprite_frame(&cd->spr, frm);
}


extern void sage_entity_update(sage_entity **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->vt.update(ctx);
}


extern void sage_entity_draw(const sage_entity *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    cd->vt.draw(ctx);
}


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 ******************************************************************************/

