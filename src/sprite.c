#include <SDL2/SDL.h>
#include "sage.h"


struct sage_sprite_t {
    sage_texture_t *tex;     /* texture                */
    struct sage_frame_t tot; /* total number of frames */
    struct sage_frame_t cur; /* current frame          */
    SDL_Rect clip;           /* clip area              */
    struct sage_area_t proj; /* projection area        */
};


extern sage_sprite_t *
sage_sprite_new(sage_id_t texid, struct sage_frame_t tot)
{
    sage_sprite_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->tex = sage_texture_factory_spawn (texid);
    ctx->tot = tot;
    sage_sprite_reset (ctx);

    return ctx;
}


extern sage_sprite_t *
sage_sprite_copy(const sage_sprite_t *src)
{
    sage_sprite_t *cp;
    sage_require (cp = malloc (sizeof *cp));
    sage_require (memcpy (cp, src, sizeof *cp));

    return cp;
}


extern sage_sprite_t *
sage_sprite_free(sage_sprite_t *ctx)
{
    if (sage_likely (ctx)) {
        ctx->tex = sage_texture_free (ctx->tex);
        free (ctx);
    }

    return NULL;
}


extern sage_id_t
sage_sprite_id(const sage_sprite_t *ctx)
{
    return sage_texture_id (ctx->tex);
}


extern struct sage_area_t
sage_sprite_area(const sage_sprite_t *ctx)
{
    return sage_texture_area (ctx->tex);
}


extern struct sage_area_t
sage_sprite_area_frame(const sage_sprite_t *ctx)
{
    struct sage_area_t tot = sage_texture_area (ctx->tex);
    struct sage_area_t frm = {.w = tot.w / ctx->tot.c, .h = tot.h / ctx->tot.r};

    return frm;
}


extern void 
sage_sprite_frame(sage_sprite_t *ctx, struct sage_frame_t frm)
{
    ctx->cur = frm;
}


extern void
sage_sprite_clip(sage_sprite_t *ctx,
                 struct sage_point_t nw,
                 struct sage_area_t clip)
{
    ctx->clip.x = nw.x;
    ctx->clip.y = nw.y;
    ctx->clip.w = clip.w;
    ctx->clip.h = clip.h;
}


extern void
sage_sprite_scale(sage_sprite_t *ctx, struct sage_area_t proj)
{
    ctx->proj = proj;
}


extern void
sage_sprite_reset(sage_sprite_t *ctx)
{
    ctx->clip.x = ctx->clip.y = 0;
    ctx->cur.r = ctx->cur.c = 1;

    struct sage_area_t frm = sage_sprite_area_frame (ctx);
    ctx->clip.w = frm.w;
    ctx->clip.h = frm.h;
}


extern void
sage_sprite_draw(const sage_sprite_t *ctx, struct sage_point_t dst)
{
    struct sage_area_t frm = sage_sprite_area_frame (ctx);
    struct sage_point_t nw = {.x = frm.w * (ctx->cur.c - 1) + ctx->clip.x,
                              .y = frm.h * (ctx->cur.r - 1) + ctx->clip.y};

    struct sage_area_t clip = {.w = ctx->clip.w, .h = ctx->clip.h};
    sage_texture_clip (ctx->tex, nw, clip);
    sage_texture_scale (ctx->tex, ctx->proj);
    sage_texture_draw (ctx->tex, dst);
}


#if 0
struct sage_sprite_t {
    sage_texture_t *tex;     /* texture                */
    struct sage_frame_t tot; /* total number of frames */
    struct sage_frame_t cur; /* current frame          */
};


extern sage_sprite_t *
sage_sprite_new(const sage_texture_t *src, struct sage_frame_t tot)
{
    sage_sprite_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->tex = sage_texture_copy (src);
    ctx->tot = tot;
    ctx->cur.r = ctx->cur.c = 1;

    return ctx;
}


extern sage_sprite_t *
sage_sprite_copy(const sage_sprite_t *src)
{
    sage_sprite_t *cp;
    sage_require (cp = malloc (sizeof *cp));
    sage_require (memcpy (cp, src, sizeof *cp));

    return cp;
}


extern sage_sprite_t *
sage_sprite_free(sage_sprite_t *ctx)
{
    if (sage_likely (ctx)) {
        ctx->tex = sage_texture_free (ctx->tex);
        free (ctx);
    }

    return NULL;
}


extern struct sage_area_t 
sage_sprite_area(const sage_sprite_t *ctx)
{
    struct sage_area_t area = {.w = ctx->tot.c, .h = ctx->tot.r};
    return area;
}


extern struct sage_frame_t 
sage_sprite_frame(const sage_sprite_t *ctx)
{
    return ctx->cur;
}


extern SAGE_HOT void 
sage_sprite_frame_set(sage_sprite_t *ctx, struct sage_frame_t frm)
{
    ctx->cur = frm;
}


extern size_t 
sage_sprite_frame_count(const sage_sprite_t *ctx)
{
    return ctx->tot.r * ctx->tot.c;
}


extern SAGE_HOT void 
sage_sprite_draw(const sage_sprite_t *ctx, struct sage_point_t dst)
{
    struct sage_area_t area = sage_texture_area (ctx->tex);
    struct sage_area_t clip = {.w = area.w / ctx->tot.c, 
                               .h = area.h / ctx->tot.r};

    struct sage_point_t nw = {.x = clip.w * (ctx->tot.c - 1), 
                              .y = clip.h * (ctx->tot.r - 1)};
    //sage_texture_draw_clipped (ctx->tex, dst, src, clip);

    sage_texture_clip (ctx->tex, nw, clip);
    sage_texture_draw (ctx->tex, dst);
}


extern SAGE_HOT void 
sage_sprite_draw_scaled(const sage_sprite_t *ctx,
                        struct sage_point_t dst, 
                        struct sage_area_t prj)
{
    struct sage_area_t area = sage_texture_area (ctx->tex);
    struct sage_area_t clip = {.w = area.w / ctx->tot.c, 
                               .h = area.h / ctx->tot.r};

    struct sage_point_t nw = {.x = clip.w * (ctx->tot.c - 1), 
                              .y = clip.h * (ctx->tot.r - 1)};
    //sage_texture_draw_scaled (ctx->tex, dst, prj, src, clip);

    sage_texture_clip (ctx->tex, nw, clip);
    sage_texture_scale (ctx->tex, prj);
    sage_texture_draw (ctx->tex, dst);
}
#endif

