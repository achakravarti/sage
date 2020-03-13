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

typedef struct sage_object sage_entity;

struct sage_entity_vtable {
    void (*update)(sage_entity **ctx);
    void (*draw)(const sage_entity *ctx);
};

extern sage_entity *sage_entity_new(sage_id entid, sage_id texid, 
        struct sage_frame_t frm, const sage_object *payload, 
        const struct sage_entity_vtable *vt);

extern sage_entity *sage_entity_new_default(sage_id entid, sage_id texid,
        struct sage_frame_t frm);

inline sage_entity *sage_entity_copy(const sage_entity *ctx)
{
    sage_assert (ctx);
    return sage_object_copy(ctx);
}

inline void sage_entity_free(sage_entity **ctx)
{
    sage_object_free(ctx);
}

inline sage_id sage_entity_id(const sage_entity *ctx)
{
    sage_assert (ctx);
    return sage_object_id(ctx);
}

extern sage_id sage_entity_guid(const sage_entity *ctx);

extern void sage_entity_guid_set(sage_entity **ctx, sage_id guid);

extern sage_vector *sage_entity_position(const sage_entity *ctx);

extern void sage_entity_position_set(sage_entity **ctx, 
        const sage_vector *pos);

extern void sage_entity_move(sage_entity **ctx, const sage_vector *vel);

extern const sage_object *sage_entity_payload(const sage_entity *ctx);

extern sage_object *sage_entity_payload_mutable(sage_entity **ctx);

extern bool sage_entity_focused(const sage_entity *ctx);

extern void sage_entity_frame(sage_entity **ctx, struct sage_frame_t frm);

extern void sage_entity_update(sage_entity **ctx);

extern void sage_entity_draw(const sage_entity *ctx);

/********************************************/


/*
 * sage_entity_list - array list of entities
 */
typedef sage_object_list sage_entity_list;


/*
 * sage_entity_list_new() - create new entity list
 */
inline sage_entity_list *sage_entity_list_new(void)
{
    return sage_object_list_new();
}


/*
 * sage_entity_list_copy() - copy entity list
 */
inline sage_entity_list *sage_entity_list_copy(const sage_entity_list *ctx)
{
    sage_assert (ctx);
    return sage_object_list_copy(ctx);
}


/*
 * sage_entity_list_free() - release entity least from heap
 */
inline void sage_entity_list_free(sage_entity_list **ctx)
{
    sage_object_free(ctx);
}


/*
 * sage_entity_list_len() - get entity list length
 */
inline size_t sage_entity_list_len(const sage_entity_list *ctx)
{
    sage_assert (ctx);
    return sage_object_list_len(ctx);
}


/*
 * sage_entity_list_find() - find entity in entity list
 */
inline size_t sage_entity_list_find(const sage_entity_list *ctx, sage_id id)
{
    sage_assert (ctx && id);
    return sage_object_list_find(ctx, id);
}


/*
 * sage_entity_list_get() - get entity by ID from entity list
 */
inline sage_entity *sage_entity_list_get(const sage_entity_list *ctx, 
        sage_id id)
{
    sage_assert (ctx && id);
    return sage_object_list_get(ctx, id);
}


/*
 * sage_entity_list_get_at() - get entity by index from entity list
 */
inline sage_entity *sage_entity_list_get_at(const sage_entity_list *ctx,
        size_t idx)
{
    sage_assert (ctx && idx);
    return sage_object_list_get_at(ctx, idx);
}


/*
 * sage_entity_list_set() - set entity by ID in entity list
 */
inline void sage_entity_list_set(sage_entity_list **ctx, sage_id id,
        const sage_entity *obj)
{
    sage_assert (ctx && id && obj);
    sage_object_list_set(ctx, id, obj);
}


/*
 * sage_entity_list_set_at() - set entity by index in entity list
 */
inline void sage_entity_list_set_at(sage_entity_list **ctx, size_t idx,
        const sage_entity *obj)
{
    sage_assert (ctx && idx && obj);
    sage_object_list_set(ctx, idx, obj);
}


/*
 * sage_entity_list_push() - push entity into entity list
 */
inline void sage_entity_list_push(sage_entity_list **ctx, 
        const sage_entity *obj)
{
    sage_assert (ctx && obj);
    sage_object_list_push(ctx, obj);
}


/*
 * sage_entity_list_pop() - pop entity by ID from entity list
 */
inline void sage_entity_list_pop(sage_entity_list **ctx, sage_id id)
{
    sage_assert (ctx && id);
    sage_object_list_pop(ctx, id);
}


/*
 * sage_entity_list_pop() - pop entity by index from entity list
 */
inline void sage_entity_list_pop_at(sage_entity_list **ctx, size_t idx)
{
    sage_assert (ctx && idx);
    sage_object_list_pop_at(ctx, idx);
}


/*****************************************/


extern void sage_entity_factory_init(void);

extern void sage_entity_factory_exit(void);

extern void sage_entity_factory_register(const sage_entity *ent);

extern sage_entity *sage_entity_factory_clone(sage_id id);


extern void 
sage_arena_start(void);

extern void 
sage_arena_stop(void);

extern const sage_entity *
sage_arena_entity(size_t idx);

extern void
sage_arena_entity_set(size_t idx, const sage_entity *ent);

extern size_t
sage_arena_push(const sage_entity *ent);

extern void 
sage_arena_pop(size_t idx);

extern void 
sage_arena_update(void);

extern void 
sage_arena_draw(void);


typedef struct sage_object sage_scene;

struct sage_scene_vtable {
    void (*start)  (sage_scene **ctx);
    void (*stop)   (sage_scene **ctx);
    void (*update) (sage_scene **ctx);
    void (*draw)   (const sage_scene *ctx);
};


extern sage_scene *sage_scene_new(sage_id id, sage_object *payload,
        const struct sage_scene_vtable *vt);


inline sage_scene *sage_scene_copy(const sage_scene *ctx)
{
    sage_assert (ctx);
    return sage_object_copy(ctx);
}


inline void sage_scene_free(sage_scene **ctx)
{
    sage_object_free(ctx);
}


inline sage_id sage_scene_id(const sage_scene *ctx)
{
    sage_assert (ctx);
    return sage_object_id(ctx);
}


extern sage_entity *sage_scene_entity(const sage_scene *ctx, sage_id guid);

extern void sage_scene_entity_set(sage_scene **ctx, sage_id guid,
        sage_entity *ent);

extern void sage_scene_entity_push(sage_scene **ctx, sage_id entid, 
        sage_id guid);

extern void sage_scene_entity_pop(sage_scene **ctx, sage_id guid);

extern sage_object *sage_scene_payload(const sage_scene *ctx);

extern void sage_scene_start(sage_scene **ctx);

extern void sage_scene_stop(sage_scene **ctx);

extern void sage_scene_update(sage_scene **ctx);

extern void sage_scene_draw(const sage_scene *ctx);

extern void sage_stage_init(void);

extern void sage_stage_exit(void);

extern void sage_stage_segue(sage_scene *scn);

extern void sage_stage_interval(sage_scene *scn);

extern void sage_stage_restore(void);

extern void sage_stage_update(void);

extern void sage_stage_draw(void);



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

