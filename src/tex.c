#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "../include/api.h"


struct cdata {
    char *path;
    SDL_Texture *tex;
    SDL_Rect clip;
    struct sage_area_t proj;
};


static inline struct cdata *cdata_new(const char *path)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);

    size_t len = strlen(path);
    ctx->path = sage_heap_new(len + 1);
    strncpy(ctx->path, path, len);

    sage_require (ctx->tex = IMG_LoadTexture(sage_screen_brush(), path));
    ctx->clip.x = ctx->clip.y = 0;
    SDL_QueryTexture(ctx->tex, NULL, NULL, &ctx->clip.w, &ctx->clip.h);

    ctx->proj.w = ctx->clip.w;
    ctx->proj.h = ctx->clip.h;

    return ctx;
}


static inline void *cdata_copy(const void *ctx)
{
    const struct cdata *hnd = (const struct cdata *) ctx;

    struct cdata *cp = cdata_new(hnd->path);
    cp->clip = hnd->clip;
    cp->proj = hnd->proj;

    return cp;
}


static inline void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);
    sage_heap_free((void **) &hnd->path);
    SDL_DestroyTexture(hnd->tex);
}


extern sage_tex *sage_tex_new(sage_id id, const char *path)
{
    sage_assert (path);
    sage_assert (*path);

    struct sage_object_vtable vt = { .copy = &cdata_copy, .free = &cdata_free };
    return sage_object_new(sage_id_new_random(), cdata_new(path), &vt);
}


extern sage_tex *sage_tex_copy(const sage_tex *ctx)
{
    sage_assert (ctx);

    return sage_object_copy(ctx);
}


extern void sage_tex_free(sage_tex **ctx)
{
    sage_object_free(ctx);
}


extern sage_id sage_tex_id(const sage_tex *ctx)
{
    sage_assert (ctx);

    return sage_object_id(ctx);
}


extern struct sage_area_t sage_tex_area(const sage_tex *ctx)
{
    sage_assert (ctx);

    const struct cdata *cd = sage_object_cdata(ctx);
    sage_assert (cd->tex);

    int w, h;
    SDL_QueryTexture(cd->tex, NULL, NULL, &w, &h);

    struct sage_area_t area = { .w = w, .h = h };
    return area;
}


extern void sage_tex_scale(sage_tex **ctx, struct sage_area_t proj)
{
    sage_assert (ctx);
    struct cdata *cd = (struct cdata *) sage_object_cdata_mutate(ctx);
    cd->proj = proj;
}


extern void sage_tex_reset(sage_tex **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = (struct cdata *) sage_object_cdata_mutate(ctx);

    cd->clip.x = cd->clip.y = 0;
    sage_assert (cd->tex);
    SDL_QueryTexture(cd->tex, NULL, NULL, &cd->clip.w, &cd->clip.h);

    cd->proj.w = cd->clip.w;
    cd->proj.h = cd->clip.h;
}


extern void sage_tex_draw(const sage_tex *ctx, struct sage_point_t dst)
{
    sage_assert (ctx);
    const struct cdata *cd = (const struct cdata *) sage_object_cdata(ctx);

    SDL_Rect to = {
        .x = (int) dst.x,
        .y = (int) dst.y,
        .w = cd->proj.w,
        .h = cd->proj.h
    };

    sage_assert (cd->tex);
    SDL_RenderCopy(sage_screen_brush(), cd->tex, &cd->clip, &to);
}


