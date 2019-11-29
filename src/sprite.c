#include <SDL2/SDL.h>
#include "sage.h"


struct sage_sprite_t {
    sage_texture_t *tex;     /* texture                */
    struct sage_frame_t tot; /* total number of frames */
    struct sage_frame_t cur; /* current frame          */
};


extern sage_sprite_t *sage_sprite_new(const char *path, struct sage_frame_t tot)
{
    sage_sprite_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));
    sage_require (ctx->tex = sage_texture_new (path));

    ctx->tot = tot;
    ctx->cur.r = ctx->cur.c = 1;

    return ctx;
}


extern sage_sprite_t *sage_sprite_copy(const sage_sprite_t *src)
{
    sage_sprite_t *cp;
    sage_require (cp = malloc (sizeof *cp));
    sage_require (memcpy (cp, src, sizeof *cp));

    return cp;
}


extern sage_sprite_t *sage_sprite_free(sage_sprite_t *ctx)
{
    if (sage_likely (ctx)) {
        ctx->tex = sage_texture_free (ctx->tex);
        free (ctx);
    }

    return NULL;
}


extern struct sage_area_t sage_sprite_area(const sage_sprite_t *ctx)
{
    struct sage_area_t area = {.w = ctx->tot.c, .h = ctx->tot.r};
    return area;
}


extern struct sage_frame_t sage_sprite_frame(const sage_sprite_t *ctx)
{
    return ctx->cur;
}


extern SAGE_HOT void sage_sprite_frame_set(sage_sprite_t *ctx,
                                           struct sage_frame_t frm)
{
    ctx->cur = frm;
}


extern size_t sage_sprite_frame_count(const sage_sprite_t *ctx)
{
    return ctx->tot.r * ctx->tot.c;
}


extern SAGE_HOT void sage_sprite_draw(const sage_sprite_t *ctx, 
                                      struct sage_point_t dst)
{
    struct sage_area_t area = sage_texture_area (ctx->tex);
    struct sage_area_t clip = {.w = area.w / ctx->tot.c, 
                               .h = area.h / ctx->tot.r};

    struct sage_point_t src = {.x = clip.w * (ctx->tot.c - 1), 
                               .y = clip.h * (ctx->tot.r - 1)};
    sage_texture_draw_clipped (ctx->tex, dst, src, clip);
}


extern SAGE_HOT void sage_sprite_draw_scaled(const sage_sprite_t *ctx,
                                             struct sage_point_t dst, 
                                             struct sage_area_t prj)
{
    struct sage_area_t area = sage_texture_area (ctx->tex);
    struct sage_area_t clip = {.w = area.w / ctx->tot.c, 
                               .h = area.h / ctx->tot.r};

    struct sage_point_t src = {.x = clip.w * (ctx->tot.c - 1), 
                               .y = clip.h * (ctx->tot.r - 1)};
    sage_texture_draw_scaled (ctx->tex, dst, prj, src, clip);
}

