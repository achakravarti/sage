#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"


struct __sage_texture {
    SDL_Texture *img;
    SDL_Renderer *brush;
    SDL_Rect dim;
};


extern SAGE_HOT sage_texture_t *sage_texture_new(const char *path)
{
    sage_texture_t *tex;
    sage_require (tex = malloc (sizeof *tex));

    sage_require (tex->img = IMG_LoadTexture (sage_screen_brush (), path));
    tex->dim.x = tex->dim.y = 0;
    SDL_QueryTexture (tex->img, NULL, NULL, &tex->dim.w, &tex->dim.h);

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

