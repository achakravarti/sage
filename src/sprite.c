#include <SDL2/SDL.h>
#include "sage.h"


struct sage_sprite_t {
    sage_texture_t      *tex; /* texture                */
    struct sage_frame_t tot;  /* total number of frames */
    struct sage_frame_t cur;  /* current frame          */
    SDL_Rect            clip; /* clip area              */
    struct sage_area_t  proj; /* projection area        */
    size_t              nref; /* reference count        */
};


static sage_sprite_t *
cow(sage_sprite_t **ctx)
{
    sage_sprite_t *hnd;
    sage_assert(ctx && (hnd = *ctx) && hnd->nref >= 1);

    if (hnd->nref == 1)
        return hnd;

    hnd->nref--;
    sage_sprite_t *cp = sage_sprite_new(sage_texture_id(hnd->tex), hnd->tot);

    cp->cur = hnd->cur;
    cp->clip = hnd->clip;
    cp->proj = hnd->proj;

    return cp;
}


extern sage_sprite_t *
sage_sprite_new(sage_id_t           texid, 
                struct sage_frame_t tot)
{
    sage_sprite_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->nref = 1;

    ctx->tex = sage_texture_factory_spawn(texid);
    ctx->tot = tot;
    sage_sprite_reset(&ctx);

    return ctx;
}


extern sage_sprite_t *
sage_sprite_copy(const sage_sprite_t *ctx)
{
    /*sage_assert (ctx);
    sage_sprite_t *cp = sage_sprite_new (sage_texture_id (ctx->tex), ctx->tot);
    return cp;*/

    sage_assert(ctx);
    sage_sprite_t *cp = (sage_sprite_t *) ctx;
    cp->nref++;

    return cp;
}


extern void 
sage_sprite_free(sage_sprite_t **ctx)
{
    sage_sprite_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx)) && !--hnd->nref) {
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
sage_sprite_frame(sage_sprite_t       **ctx, 
                  struct sage_frame_t frm)
{
    sage_assert(ctx);
    sage_sprite_t *hnd = cow(ctx);

    hnd->cur = frm;
}


extern struct sage_frame_t 
sage_sprite_frames(const sage_sprite_t *ctx)
{
    return ctx->tot;
}


extern size_t
sage_sprite_refcount(const sage_sprite_t *ctx)
{
    return ctx->nref;
}


extern void 
sage_sprite_clip(sage_sprite_t       **ctx, 
                 struct sage_point_t nw,
                 struct sage_area_t  clip)
{
    sage_assert(ctx);
    sage_sprite_t *hnd = cow(ctx);

    hnd->clip.x = nw.x;
    hnd->clip.y = nw.y;
    hnd->clip.w = clip.w;
    hnd->clip.h = clip.h;
}


extern void 
sage_sprite_scale(sage_sprite_t      **ctx, 
                  struct sage_area_t proj)
{
    sage_assert(ctx);
    sage_sprite_t *hnd = cow(ctx);

    hnd->proj = proj;
}


extern void 
sage_sprite_reset(sage_sprite_t **ctx)
{
    sage_assert(ctx);
    sage_sprite_t *hnd = cow(ctx);

    hnd->clip.x = hnd->clip.y = 0;
    hnd->cur.r = hnd->cur.c = 1;

    struct sage_area_t frm = sage_sprite_area_frame(hnd);
    hnd->clip.w = hnd->proj.w = frm.w;
    hnd->clip.h = hnd->proj.h = frm.h;
}


extern void 
sage_sprite_draw(const sage_sprite_t *ctx, 
                 struct sage_point_t dst)
{
    sage_assert(ctx);

    struct sage_area_t frm = sage_sprite_area_frame(ctx);
    struct sage_point_t nw = {.x = frm.w * (ctx->cur.c - 1) + ctx->clip.x,
                              .y = frm.h * (ctx->cur.r - 1) + ctx->clip.y};
    struct sage_area_t clip = {.w = ctx->clip.w, .h = ctx->clip.h};

    sage_sprite_t *hnd = (sage_sprite_t *) ctx;
    sage_texture_clip(&hnd->tex, nw, clip);
    sage_texture_scale(&hnd->tex, ctx->proj);
    sage_texture_draw(ctx->tex, dst);
}

