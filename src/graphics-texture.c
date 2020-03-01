#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/api.h"



    /* define the sage_texture_t struct */
struct sage_texture_t {
    sage_id_t          id;    /* texture ID        */
    SDL_Texture        *tex;  /* raw texture       */
    char               *path; /* texture file path */
    SDL_Rect           clip;  /* clip area         */
    struct sage_area_t proj;  /* projection area   */
    size_t nref;
};


static void copy_on_write(sage_texture_t **ctx)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    sage_texture_t *hnd = *ctx; 

    if (hnd->nref > 1) {
        sage_texture_free(ctx);
        sage_texture_t *cp = sage_texture_new(hnd->path, hnd->id);
        cp->clip = hnd->clip;
        cp->proj = hnd->proj;
        *ctx = cp;
    }
}



    /* implement the sage_texture_new() interface function */
extern SAGE_HOT sage_texture_t *
sage_texture_new(const char *path, 
                 sage_id_t  id)
{
    sage_texture_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->id = id;
    ctx->nref = 1;

    sage_assert (path && *path);
    size_t len = strlen(path);
    ctx->path = sage_heap_new(len + 1);
    strncpy(ctx->path, path, len);

    sage_require (ctx->tex = IMG_LoadTexture(sage_screen_brush(), path));
    (void) sage_texture_reset(&ctx);

    return ctx;
}


extern sage_texture_t *
sage_texture_copy(const sage_texture_t *ctx)
{
    sage_assert (ctx);

    sage_texture_t *cp = (sage_texture_t *) ctx;
    cp->nref++;

    return cp;
}



    /* implement the sage_texture_free() interface function */
extern void 
sage_texture_free(sage_texture_t **ctx)
{
    sage_texture_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        if (!--hnd->nref) {
            SDL_DestroyTexture(hnd->tex);
            sage_heap_free((void **) &hnd->path);
            sage_heap_free((void **) ctx);
        }
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
sage_texture_clip(sage_texture_t      **ctx, 
                  struct sage_point_t nw,
                  struct sage_area_t  clip)
{
    sage_assert (ctx);

    copy_on_write(ctx);
    sage_texture_t *hnd = *ctx;

    hnd->clip.x = (int) nw.x;
    hnd->clip.y = (int) nw.y;
    hnd->clip.w = clip.w;
    hnd->clip.h = clip.h;
}


    /* implement the sage_texture_scale() interface function */
extern void sage_texture_scale(sage_texture_t **ctx, struct sage_area_t proj)
{
    sage_assert (ctx);
    sage_assert (*ctx);

    copy_on_write(ctx);
    (*ctx)->proj = proj;
}


    /* implement the sage_texture_reset() interface function */
extern void 
sage_texture_reset(sage_texture_t **ctx)
{
    sage_assert (ctx);
    sage_assert (*ctx);
    sage_assert ((*ctx)->tex);

    sage_texture_t *hnd = *ctx;
    hnd->clip.x = hnd->clip.y = 0;
    SDL_QueryTexture(hnd->tex, NULL, NULL, &hnd->clip.w, &hnd->clip.h);
    hnd->proj.w = hnd->clip.w;
    hnd->proj.h = hnd->clip.h;
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

