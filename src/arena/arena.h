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
 * This is the sage/src/arena/arena.h header file; it declares the interface of
 * the SAGE Library arena.
 ******************************************************************************/


#ifndef SAGE_ARENA_API
#define SAGE_ARENA_API


#include "../core/core.h"
#include "../graphics/graphics.h"


typedef struct sage_entity_t sage_entity_t;


/**
 *      The sage_entity_vtable_t structure holds the v-table of callback
 *      functions for a sage_entity_t instance. There are three callback
 **/
struct sage_entity_vtable_t {
    void (*update) (sage_entity_t *ctx);
    void (*draw)   (const sage_entity_t *ctx);
};


/**
 * Function: sage_entity_new()
 *      Create a new entity instance.
 *
 * Scope: Entity
 *
 * Parameters:
 *      cls   - entity class
 *      texid - texture ID
 *      frm   - frames
 *      vt    - v-table of callbacks
 *
 * Return:
 *      The new entity instance.
 **/
extern sage_entity_t *
sage_entity_new(sage_id_t                         cls, 
                sage_id_t                         texid,
                struct sage_frame_t               frm, 
                const sage_payload_t              *cust,
                const struct sage_entity_vtable_t *vt);

extern sage_entity_t *
sage_entity_new_default(sage_id_t           cls,
                        sage_id_t           texid,
                        struct sage_frame_t frm);


inline sage_entity_t *
sage_entity_move(sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


inline const sage_entity_t *
sage_entity_link(const sage_entity_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


extern sage_entity_t *
sage_entity_copy(const sage_entity_t *ctx);

extern void 
sage_entity_free(sage_entity_t **ctx);

extern size_t 
sage_entity_size(void);

extern sage_id_t 
sage_entity_class(const sage_entity_t *ctx);

extern sage_id_t 
sage_entity_id(const sage_entity_t *ctx);

extern void 
sage_entity_id_set(sage_entity_t *ctx,
                   sage_id_t     id);

extern const sage_vector_t *
sage_entity_vector(const sage_entity_t *ctx);

extern void 
sage_entity_vector_set(sage_entity_t       *ctx, 
                       const sage_vector_t *vec);

extern void 
sage_entity_vector_move(sage_entity_t       *ctx, 
                        const sage_vector_t *vel);

extern const sage_payload_t *
sage_entity_payload(const sage_entity_t *ctx);

extern bool
sage_entity_focused(const sage_entity_t *ctx);

extern void 
sage_entity_frame(sage_entity_t       *ctx, 
                  struct sage_frame_t frm);

extern void 
sage_entity_update(sage_entity_t *ctx);

extern void 
sage_entity_draw(const sage_entity_t *ctx);


extern void 
sage_entity_factory_start(void);

extern void 
sage_entity_factory_stop(void);

extern void 
sage_entity_factory_register(sage_id_t cls,
                             sage_id_t texid);

extern sage_entity_t *
sage_entity_factory_spawn(sage_id_t id);


extern void 
sage_arena_start(void);

extern void 
sage_arena_stop(void);

extern const sage_entity_t *
sage_arena_entity(size_t idx);

extern void
sage_arena_entity_set(size_t idx, const sage_entity_t *ent);

extern size_t
sage_arena_push(const sage_entity_t *ent);

extern void 
sage_arena_pop(size_t idx);

extern void 
sage_arena_update(void);

extern void 
sage_arena_draw(void);


typedef struct sage_scene_t sage_scene_t;

struct sage_scene_vtable_t {
    void (*start)  (sage_scene_t *ctx);
    void (*stop)   (sage_scene_t *ctx);
    void (*update) (sage_scene_t *ctx);
    void (*draw)   (const sage_scene_t *ctx);
};


extern sage_scene_t *
sage_scene_new(sage_id_t                        id,
               sage_payload_t                   *cdata,
               const struct sage_scene_vtable_t *vt);

inline sage_scene_t *
sage_scene_move(sage_scene_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


inline const sage_scene_t *
sage_scene_link(const sage_scene_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

extern sage_scene_t *
sage_scene_copy(const sage_scene_t *ctx);

extern void
sage_scene_free(sage_scene_t **ctx);

extern size_t
sage_scene_size(void);

extern sage_id_t
sage_scene_id(const sage_scene_t *ctx);

extern const sage_entity_t *
sage_scene_entity(const sage_scene_t *ctx,
                  sage_id_t          id);

extern void
sage_scene_entity_set(sage_scene_t *ctx,
                     sage_id_t     id,
                     sage_entity_t *ent);

extern void
sage_scene_entity_push(sage_scene_t *ctx,
                       sage_id_t    cls,
                       sage_id_t    id);

extern void
sage_scene_entity_pop(sage_scene_t *ctx,
                      sage_id_t    id);

extern const sage_payload_t *
sage_scene_payload(const sage_scene_t *ctx);

extern void 
sage_scene_start(sage_scene_t *ctx);

extern void 
sage_scene_stop(sage_scene_t *ctx);

extern void 
sage_scene_update(sage_scene_t *ctx);

extern void 
sage_scene_draw(const sage_scene_t *ctx);


extern void
sage_stage_init(void);

extern void
sage_stage_exit(void);

extern void
sage_stage_segue(sage_scene_t *scn);

extern void
sage_stage_interval(sage_scene_t *scn);

extern void
sage_stage_restore(void);

extern void
sage_stage_update(void);

extern void
sage_stage_draw(void);



/** EVENT **/


extern void sage_event_start(void);

extern void sage_event_stop(void);

extern SAGE_HOT void sage_event_run(void);


extern void 
sage_game_start(void);

extern void 
sage_game_stop(void);

extern void 
sage_game_run(void);



#endif /* SAGE_ARENA_API */


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 ******************************************************************************/

