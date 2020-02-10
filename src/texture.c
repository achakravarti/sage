#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"


    /* define the sage_texture_t struct */
struct sage_texture_t {
    sage_id_t          id;    /* texture ID        */
    SDL_Texture        *tex;  /* raw texture       */
    char               *path; /* texture file path */
    SDL_Rect           clip;  /* clip area         */
    struct sage_area_t proj;  /* projection area   */
};


    /* implement the sage_texture_new() interface function */
extern SAGE_HOT sage_texture_t *
sage_texture_new(const char *path, 
                 sage_id_t  id)
{
    sage_texture_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->id = id;

    sage_assert (path && *path);
    size_t len = strlen(path);
    ctx->path = sage_heap_new(len + 1);
    strncpy(ctx->path, path, len);

    sage_require (ctx->tex = IMG_LoadTexture(sage_screen_brush(), path));
    (void) sage_texture_reset(ctx);

    return ctx;
}


extern const sage_texture_t *
sage_texture_copy(const sage_texture_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}


    /* implement the sage_texture_copy() interface function */
extern SAGE_HOT sage_texture_t *
sage_texture_copy_deep(const sage_texture_t *ctx)
{
    sage_texture_t *cp = sage_texture_new(ctx->path, ctx->id);

    cp->clip = ctx->clip;
    cp->proj = ctx->proj;

    return cp;
}


    /* implement the sage_texture_free() interface function */
extern void 
sage_texture_free(sage_texture_t **ctx)
{
    sage_texture_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        SDL_DestroyTexture(hnd->tex);
        sage_heap_free((void **) &hnd->path);
        sage_heap_free((void **) ctx);
    }
}


    /* implement the sage_texture_size() interface function */
extern size_t 
sage_texture_size(void)
{
    return sizeof (struct sage_texture_t);
}


    /* implement the sage_texture_id() interface function */
extern sage_id_t 
sage_texture_id(const sage_texture_t *ctx)
{
    sage_assert (ctx);
    return ctx->id;
}


    /* implement the sage_texture_area() interface function */
extern SAGE_HOT struct sage_area_t 
sage_texture_area(const sage_texture_t *ctx)
{
    sage_assert (ctx);
    sage_assert (ctx->tex);

    int w, h;
    SDL_QueryTexture(ctx->tex, NULL, NULL, &w, &h);
    
    struct sage_area_t area = {.w = w, .h = h};
    return area;
}


    /* implement the sage_texture_clip() interface function */
extern void 
sage_texture_clip(sage_texture_t      *ctx, 
                  struct sage_point_t nw,
                  struct sage_area_t  clip)
{
    sage_assert (ctx);

    ctx->clip.x = (int) nw.x;
    ctx->clip.y = (int) nw.y;
    ctx->clip.w = clip.w;
    ctx->clip.h = clip.h;
}


    /* implement the sage_texture_scale() interface function */
extern void 
sage_texture_scale(sage_texture_t     *ctx, 
                   struct sage_area_t proj)
{
    sage_assert (ctx);
    ctx->proj = proj;
}


    /* implement the sage_texture_reset() interface function */
extern void 
sage_texture_reset(sage_texture_t *ctx)
{
    sage_assert (ctx);
    ctx->clip.x = ctx->clip.y = 0;

    sage_assert (ctx->tex);
    SDL_QueryTexture(ctx->tex, NULL, NULL, &ctx->clip.w, &ctx->clip.h);
    
    ctx->proj.w = ctx->clip.w;
    ctx->proj.h = ctx->clip.h;
}


    /* implement the sage_texture_draw() interface function */
extern SAGE_HOT void 
sage_texture_draw(const sage_texture_t *ctx, 
                  struct sage_point_t  dst)
{
    sage_assert (ctx);
    SDL_Rect to = {.x = (int) dst.x, .y = (int) dst.y, .w = ctx->proj.w, 
                   .h = ctx->proj.h};

    sage_assert (ctx->tex);
    SDL_RenderCopy(sage_screen_brush (), ctx->tex, &ctx->clip, &to);
}

