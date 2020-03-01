#ifndef SAGE_SPRITE_H
#define SAGE_SPRITE_H

#include "../src/core/core.h"

/** SPRITE **/


struct sage_frame_t {
    uint16_t r;
    uint16_t c;
};


typedef struct sage_sprite_t sage_sprite_t;

extern sage_sprite_t *
sage_sprite_new(sage_id_t           texid, 
                struct sage_frame_t tot);

inline sage_sprite_t *
sage_sprite_move(sage_sprite_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

inline const sage_sprite_t *
sage_sprite_link(const sage_sprite_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

extern sage_sprite_t *
sage_sprite_copy(const sage_sprite_t *ctx);

extern void 
sage_sprite_free(sage_sprite_t **ctx);

extern sage_id_t 
sage_sprite_id(const sage_sprite_t *ctx);

extern struct sage_area_t 
sage_sprite_area(const sage_sprite_t *ctx);

extern struct sage_area_t 
sage_sprite_area_frame(const sage_sprite_t *ctx);

extern void 
sage_sprite_frame(sage_sprite_t       *ctx, 
                  struct sage_frame_t frm);

extern struct sage_frame_t 
sage_sprite_frames(const sage_sprite_t *ctx);

extern void 
sage_sprite_clip(sage_sprite_t       *ctx, 
                 struct sage_point_t nw,
                 struct sage_area_t  clip);

extern void 
sage_sprite_scale(sage_sprite_t      *ctx, 
                  struct sage_area_t proj);

extern void 
sage_sprite_reset(sage_sprite_t *ctx);

extern void 
sage_sprite_draw(const sage_sprite_t *ctx, 
                 struct sage_point_t dst);


#endif /* SAGE_SPRITE_H */

