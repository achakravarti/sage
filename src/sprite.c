#include <SDL2/SDL.h>
#include "../include/api.h"


struct sage_sprite_t {
    sage_texture_t      *tex; /* texture                */
    struct sage_frame_t tot;  /* total number of frames */
    struct sage_frame_t cur;  /* current frame          */
    SDL_Rect            clip; /* clip area              */
    struct sage_area_t  proj; /* projection area        */
};


extern sage_sprite_t *
sage_sprite_new(sage_id_t           texid, 
                struct sage_frame_t tot)
{
    sage_sprite_t *ctx = sage_heap_new(sizeof *ctx);

    ctx->tex = sage_texture_factory_spawn(texid);
    ctx->tot = tot;
    sage_sprite_reset(ctx);

    return ctx;
}


extern inline sage_sprite_t *
sage_sprite_move(sage_sprite_t *ctx);


extern inline const sage_sprite_t *
sage_sprite_link(const sage_sprite_t *ctx);


extern sage_sprite_t *
sage_sprite_copy(const sage_sprite_t *ctx)
{
    sage_assert(ctx);
    sage_sprite_t *cp = sage_sprite_new(sage_texture_id(ctx->tex), ctx->tot);

    cp->cur = ctx->cur;
    cp->clip = ctx->clip;
    cp->proj = ctx->proj;

    return cp;
}


extern void 
sage_sprite_free(sage_sprite_t **ctx)
{
    sage_sprite_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        sage_texture_free(&hnd->tex);
        sage_heap_free((void **) ctx);
    }
}


extern sage_id_t 
sage_sprite_id(const sage_sprite_t *ctx)
{
    return sage_texture_id(ctx->tex);
}


extern struct sage_area_t 
sage_sprite_area(const sage_sprite_t *ctx)
{
    return sage_texture_area(ctx->tex);
}


extern struct sage_area_t 
sage_sprite_area_frame(const sage_sprite_t *ctx)
{
    sage_assert (ctx);

    struct sage_area_t tot = sage_texture_area(ctx->tex);
    struct sage_area_t frm = {.w = tot.w / ctx->tot.c, .h = tot.h / ctx->tot.r};

    return frm;
}


extern void 
sage_sprite_frame(sage_sprite_t       *ctx, 
                  struct sage_frame_t frm)
{
    sage_assert(ctx);
    ctx->cur = frm;
}


extern struct sage_frame_t 
sage_sprite_frames(const sage_sprite_t *ctx)
{
    return ctx->tot;
}


extern void 
sage_sprite_clip(sage_sprite_t       *ctx, 
                 struct sage_point_t nw,
                 struct sage_area_t  clip)
{
    sage_assert (ctx);
    ctx->clip.x = (int) nw.x;
    ctx->clip.y = (int) nw.y;
    ctx->clip.w = clip.w;
    ctx->clip.h = clip.h;
}


extern void 
sage_sprite_scale(sage_sprite_t      *ctx, 
                  struct sage_area_t proj)
{
    sage_assert(ctx);
    ctx->proj = proj;
}


extern void 
sage_sprite_reset(sage_sprite_t *ctx)
{
    sage_assert(ctx);
    ctx->clip.x = ctx->clip.y = 0;
    ctx->cur.r = ctx->cur.c = 1;

    struct sage_area_t frm = sage_sprite_area_frame(ctx);
    ctx->clip.w = ctx->proj.w = frm.w;
    ctx->clip.h = ctx->proj.h = frm.h;
}


extern void 
sage_sprite_draw(const sage_sprite_t *ctx, 
                 struct sage_point_t dst)
{
    sage_assert(ctx);
    struct sage_area_t frm = sage_sprite_area_frame(ctx);
    struct sage_point_t nw = {.x = (float) (frm.w * (ctx->cur.c - 1) 
                                            + ctx->clip.x),
                              .y = (float) (frm.h * (ctx->cur.r - 1) 
                                            + ctx->clip.y)};
    struct sage_area_t clip = {.w = ctx->clip.w, .h = ctx->clip.h};

    sage_sprite_t *hnd = (sage_sprite_t *) ctx;
    sage_texture_clip(&hnd->tex, nw, clip);
    sage_texture_scale(&hnd->tex, ctx->proj);
    sage_texture_draw(ctx->tex, dst);
}

