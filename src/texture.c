#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"


    /* define the sage_texture_t struct */
struct sage_texture_t {
    sage_id_t id;            /* texture ID      */
    SDL_Texture *tex;        /* raw texture     */
    SDL_Rect clip;           /* clip area       */
    struct sage_area_t proj; /* projection area */
};


    /* implement the sage_texture_new() interface function */
extern SAGE_HOT sage_texture_t *
sage_texture_new(const char *path, sage_id_t id)
{
    sage_texture_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    sage_require (ctx->tex = IMG_LoadTexture (sage_screen_brush (), path));
    sage_texture_reset (ctx);
    ctx->id = id;

    return ctx;
}


    /* implement the sage_texture_copy() interface function */
extern SAGE_HOT sage_texture_t *
sage_texture_copy(const sage_texture_t *ctx)
{
    sage_texture_t *cp;
    sage_require (cp = malloc (sizeof *cp));
    sage_require (memcpy (cp, ctx, sizeof *cp));

    return cp;
}


    /* implement the sage_texture_free() interface function */
extern sage_texture_t*
sage_texture_free(sage_texture_t *ctx)
{
    if (sage_likely (ctx)) {
        SDL_DestroyTexture (ctx->tex);
        free (ctx);
    }

    return NULL;
}


    /* implement the sage_texture_id() interface function */
extern sage_id_t
sage_texture_id(const sage_texture_t *ctx)
{
    return ctx->id;
}


    /* implement the sage_texture_area() interface function */
extern SAGE_HOT struct sage_area_t 
sage_texture_area(const sage_texture_t *ctx)
{
    int w, h;
    SDL_QueryTexture (ctx->tex, NULL, NULL, &w, &h);
    
    struct sage_area_t area = {.w = w, .h = h};
    return area;
}


    /* implement the sage_texture_clip() interface function */
extern void
sage_texture_clip(sage_texture_t *ctx, 
                  struct sage_point_t nw,
                  struct sage_area_t clip)
{
    ctx->clip.x = nw.x;
    ctx->clip.y = nw.y;
    ctx->clip.w = clip.w;
    ctx->clip.h = clip.h;
}


    /* implement the sage_texture_scale() interface function */
extern void
sage_texture_scale(sage_texture_t *ctx, struct sage_area_t proj)
{
    ctx->proj = proj;
}


    /* implement the sage_texture_reset() interface function */
extern void
sage_texture_reset(sage_texture_t *ctx)
{
    ctx->clip.x = ctx->clip.y = 0;
    SDL_QueryTexture (ctx->tex, NULL, NULL, &ctx->clip.w, &ctx->clip.h);
    
    ctx->proj.w = ctx->clip.w;
    ctx->proj.h = ctx->clip.h;
}


    /* implement the sage_texture_draw() interface function */
extern SAGE_HOT void
sage_texture_draw(const sage_texture_t *ctx, struct sage_point_t dst)
{
    SDL_Rect to = {.x = dst.x, .y = dst.y, .w = ctx->proj.w, .h = ctx->proj.h};
    SDL_RenderCopy (sage_screen_brush (), ctx->tex, &ctx->clip, &to);
}

