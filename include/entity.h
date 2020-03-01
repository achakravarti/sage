
#ifndef SAGE_ENTITY_H
#define SAGE_ENTITY_H

#include "../src/core/core.h"
#include "payload.h" /* sage_payload_t */

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

#endif /* SAGE_ENTITY_H */

