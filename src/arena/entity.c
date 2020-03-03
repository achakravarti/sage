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


/*
 * The sage_entity_t struct was forward declared in the sage/src/sage.h header
 * file. We're defining the struct here with its attributes. 
 *
 * The entity class is a unique identifier for the specific type of entity; all
 * entities of the same type have the same class. The arena ID of the entity is 
 * a unique identifier allocated to an entity instance by the arena in which 
 * it has been spawned; the arena ID is mutable depending on the context. 
 *
 * The position vector holds the current position of the entity. The sprite 
 * attribute holds the sprite sheet used for animating the entity. The v-table 
 * attribute holds the callback functions that are used to polymorphically alter
 * the behaviour of each class of entity in relation to the custom data payload
 * attribute.
 */
struct sage_entity_t {
    sage_id_t                   cls;    /* entity class         */
    sage_id_t                   id;     /* arena ID             */
    sage_vector_t               *vec;   /* position vector      */
    sage_sprite                 *spr;   /* sprite               */
    sage_payload_t              *cdata; /* custom data payload  */
    struct sage_entity_vtable_t vt;     /* v-table of callbacks */
};


/*
 * The draw_default() helper function is the default draw callback for entity
 * instances. This is the default draw function that is called for an entity in
 * case it has not been initialised with a draw callback. This function simply
 * draws the current frame of entity's sprite sheet at the current position of
 * the entity, provided that the entity is visible on the screen.
 */
static inline void 
draw_default(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    if (sage_likely (sage_vector_visible(ctx->vec)))
        sage_sprite2_draw(ctx->spr, sage_vector_point(ctx->vec));
}


/*
 * The update_default() helper function is the default update callback for
 * entity instances. Just as in the case of the free_default() helper function,
 * this function has been provided in order to improve performance, even though
 * it does nothing.
 */
static inline void 
update_default(sage_entity_t *ctx)
{
    (void) ctx;
}


/*
 * The sage_entity_new() interface function creates a new instance of an entity.
 * We initialise the class, sprite, custom data payload and v-table attributes
 * of the entity through the arguments supplied, and the position vector and
 * arena ID attributes to their default values. If any of the callback functions
 * in the v-table are not defined, then the default callback functions (defined 
 * above) are used.
 */
extern sage_entity_t *
sage_entity_new(sage_id_t                         cls, 
                sage_id_t                         texid,
                struct sage_frame_t               frm, 
                const sage_payload_t              *cdata,
                const struct sage_entity_vtable_t *vt)
{
    sage_entity_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->cls = cls;
    ctx->id = (sage_id_t) 0;
    ctx->vec = sage_vector_new_zero();
    ctx->spr = sage_sprite2_new(texid, frm);
    ctx->cdata = sage_payload_copy(cdata);
  
    sage_assert (vt); 
    ctx->vt.update = vt->update ? vt->update : &update_default;
    ctx->vt.draw = vt->draw ? vt->draw : &draw_default;

    return ctx;
}


/*
 * The sage_entity_new_default() interface function overrides sage_entity_new(),
 * creating a default entity instance. The only difference between this function
 * and sage_entity_new() is that this function sets the v-table attributes to
 * the default callback functions, and the custom data payload to null.
 */
extern sage_entity_t *
sage_entity_new_default(sage_id_t           cls,
                        sage_id_t           texid,
                        struct sage_frame_t frm)
{
    sage_entity_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->cls = cls;
    ctx->id = (sage_id_t) 0;
    ctx->vec = sage_vector_new_zero();
    ctx->spr = sage_sprite2_new(texid, frm);
    ctx->cdata = NULL;

    ctx->vt.update = &update_default;
    ctx->vt.draw = &draw_default;

    return ctx;
}


extern inline sage_entity_t *
sage_entity_move(sage_entity_t *ctx);


extern inline const sage_entity_t *
sage_entity_link(const sage_entity_t *ctx);


/*
 * The sage_entity_copy_deep() interface function creates a deep copy of an
 * entity. We create a new entity instance with the same attributes as the
 * contextual instance, and return the handle to the newly created entity.
 */
extern sage_entity_t *
sage_entity_copy(const sage_entity_t *ctx)
{
    sage_entity_t *cp = sage_heap_new(sizeof *ctx);

    cp->cls = ctx->cls;
    cp->id = ctx->id;
    cp->vec = sage_vector_copy(ctx->vec);
    cp->spr = sage_sprite2_copy(ctx->spr);
    cp->cdata = sage_likely (ctx->cdata) ? sage_payload_copy(ctx->cdata) : NULL;

    cp->vt.update = ctx->vt.update;
    cp->vt.draw = ctx->vt.draw;

    return cp;
}


/*
 * The sage_entity_free() interface function releases the heap memory allocated
 * to an entity. We first free the custom data payload, and then free the base
 * attributes before releasing the entity itself.
 */
extern void 
sage_entity_free(sage_entity_t **ctx)
{
    sage_entity_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        sage_payload_free(&hnd->cdata);
        sage_vector_free(&hnd->vec);
        sage_sprite2_free(&hnd->spr);
        sage_heap_free((void **) ctx);
    }
}


/*
 * The sage_entity_size() interface function gets the current size of an entity.
 * We need this function because sage_entity_t is an abstract data type, and so
 * can't be dereferenced by client code.
 */
extern size_t 
sage_entity_size(void)
{
    return sizeof (struct sage_entity_t);
}


/*
 * The sage_entity_class() interface function gets the class of an entity. The
 * class is held in the cls attribute of the entity.
 */
extern sage_id_t 
sage_entity_class(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx->cls;
}


/*
 * The sage_entity_id() interface function gets the arena ID of an entity. The
 * arena ID is held in the id attribute of the entity.
 */
extern sage_id_t 
sage_entity_id(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx->id;
}


/*
 * The sage_entity_id_set() interface function sets the ID of an entity. We do
 * so by updating the id attribute of the entity.
 */
extern void 
sage_entity_id_set(sage_entity_t *ctx, 
                   sage_id_t     id)
{
    sage_assert (ctx);
    ctx->id = id;
}


/*
 * The sage_entity_vector() interface function gets the current position vector
 * of an entity. The current position is held in the vec attribute of the
 * entity.
 */
extern const sage_vector_t *
sage_entity_vector(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return sage_vector_copy(ctx->vec);
}


/*
 * The sage_entity_vector_set() interface function sets the current position
 * vector of an entity. We do so by updating the vec attribute of the entity
 * through a deep copy.
 */
extern void 
sage_entity_vector_set(sage_entity_t       *ctx, 
                       const sage_vector_t *vec)
{
    sage_assert (ctx && vec);
    sage_vector_free(&ctx->vec);
    ctx->vec = sage_vector_copy(vec);
}


/*
 * The sage_entity_vector_move() interface function moves the current entity by
 * a given velocity vector. We do so by adding the velocity vector to the
 * current position of the entity.
 */
extern void 
sage_entity_vector_move(sage_entity_t       *ctx, 
                        const sage_vector_t *vel)
{
    sage_assert (ctx && vel);
    sage_vector_add(&ctx->vec, vel);
}


/*
 * The sage_entity_payload() interface function gets a handle to the payload of
 * an entity instance. We return a shallow copy of the custom data attribute.
 */
extern const sage_payload_t *
sage_entity_payload(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx->cdata;
}


/*
 * The sage_entity_focused() interface function checks whether an entity has the
 * current mouse focus. We know that the mouse is focused within the entity if
 * the current position of the mouse lies between the current NW and SE points
 * of the entity. The NW point is obtained from the current position vector of
 * the entity, and SE point is computed accordingly based on the dimensions of
 * the current sprite frame. TODO: account for alpha channel pixels
 */
extern bool
sage_entity_focused(const sage_entity_t *ctx)
{
    sage_assert(ctx);

    struct sage_area_t frame = sage_sprite2_area_frame(ctx->spr);
    sage_vector_t *add = sage_vector_new((float) frame.w, (float) frame.h);

    sage_vector_t *se = sage_vector_copy(ctx->vec);
    sage_vector_add(&se, add);
    sage_vector_free(&add);

    sage_vector_t *aim = sage_mouse_vector();
    bool focused = sage_vector_gteq(aim, ctx->vec) && sage_vector_lteq(aim, se);

    sage_vector_free(&aim);
    sage_vector_free(&se);

    return focused;
}


/*
 * The sage_entity_frame() interface function sets the current sprite frame of
 * an entity. We do so by updating the spr field of the entity.
 */
extern void 
sage_entity_frame(sage_entity_t       *ctx, 
                  struct sage_frame_t frm)
{
    sage_assert (ctx);
    sage_sprite2_frame(&ctx->spr, frm);
}


/*
 * The sage_entity_update() interface function updates the state of an entity.
 * We do so by calling the update callback function in the entity's v-table.
 */
extern void 
sage_entity_update(sage_entity_t *ctx)
{
    sage_assert(ctx);
    ctx->vt.update(ctx);
}


/*
 * The sage_entity_draw() interface function draws an entity. We do so by
 * calling the draw callback function in the entity's v-table.
 */
extern void 
sage_entity_draw(const sage_entity_t *ctx)
{
    sage_assert(ctx);
    ctx->vt.draw(ctx);
}


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 ******************************************************************************/

