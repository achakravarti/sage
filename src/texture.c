#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"


struct sage_texture_t {
    SDL_Texture *img;
    SDL_Renderer *brush;
    //SDL_Rect dim;
    SDL_Rect clip;
    struct sage_area_t proj;
    //SDL_Rect dst;
};


extern SAGE_HOT sage_texture_t *sage_texture_new(const char *path)
{
    sage_texture_t *tex;
    sage_require (tex = malloc (sizeof *tex));

    sage_require (tex->img = IMG_LoadTexture (sage_screen_brush (), path));
    sage_texture_reset (tex);
    //tex->dim.x = tex->dim.y = 0;
    //SDL_QueryTexture (tex->img, NULL, NULL, &tex->dim.w, &tex->dim.h);

    return tex;
}


extern sage_texture_t*
sage_texture_free(sage_texture_t *tex)
{
    if (sage_likely (tex)) {
        SDL_DestroyTexture (tex->img);
        free (tex);
    }

    return NULL;
}


extern SAGE_HOT struct sage_area_t sage_texture_area(const sage_texture_t *tex)
{
    //struct sage_area_t area = {.w = tex->dim.w, .h = tex->dim.h};
    int w, h;
    SDL_QueryTexture (tex->img, NULL, NULL, &w, &h);
    
    struct sage_area_t area = {.w = w, .h = h};
    return area;
}


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


extern void
sage_texture_scale(sage_texture_t *ctx, struct sage_area_t proj)
{
    ctx->proj = proj;
}


extern void
sage_texture_reset(sage_texture_t *ctx)
{
    ctx->clip.x = ctx->clip.y = 0;
    SDL_QueryTexture (ctx->img, NULL, NULL, &ctx->clip.w, &ctx->clip.h);
    
    ctx->proj.w = ctx->clip.w;
    ctx->proj.h = ctx->clip.h;
}


extern SAGE_HOT void
sage_texture_draw(sage_texture_t *ctx, struct sage_point_t dst)
{
    SDL_Rect to = {.x = dst.x, .y = dst.y, .w = ctx->proj.w, .h = ctx->proj.h};
    SDL_RenderCopy (sage_screen_brush (), ctx->img, &ctx->clip, &to);
}


#if 0
extern SAGE_HOT void
sage_texture_draw(sage_texture_t *tex, struct sage_point_t dst)
{
    SDL_Rect r = {.x = dst.x, .y = dst.y, .w = tex->dim.w, .h = tex->dim.h};
    SDL_RenderCopy (sage_screen_brush (), tex->img, NULL, &r);
}


extern SAGE_HOT void
sage_texture_draw_clipped(sage_texture_t *tex, 
                          struct sage_point_t dst,
                          struct sage_point_t src,
                          struct sage_area_t clip)
{
    SDL_Rect rsrc = {.x = src.x, .y = src.y, .w = clip.w, .h = clip.h};
    SDL_Rect rdst = {.x = dst.x, .y = dst.y, .w = clip.w, .h = clip.h};
    SDL_RenderCopy (sage_screen_brush (), tex->img, &rsrc, &rdst);
}


extern SAGE_HOT void
sage_texture_draw_scaled(sage_texture_t *tex, 
                         struct sage_point_t dst,
                         struct sage_area_t proj,
                         struct sage_point_t src,
                         struct sage_area_t clip)
{
    SDL_Rect rsrc = {.x = src.x, .y = src.y, .w = clip.w, .h = clip.h};
    SDL_Rect rdst = {.x = dst.x, .y = dst.y, .w = proj.w, .h = proj.h};
    SDL_RenderCopy (sage_screen_brush (), tex->img, &rsrc, &rdst);
}
#endif
