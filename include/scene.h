#ifndef SAGE_SCENE_H
#define SAGE_SCENE_H

#include "../src/core/core.h"
#include "entity.h"

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

#endif /* SAGE_SCENE_H */

