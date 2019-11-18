#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "./sage.h"


struct __sage_image {
    SDL_Surface *image;
    SDL_Surface *screen;
};


extern SAGE_HOT sage_image_t*
sage_image_new(const char *path, sage_screen_t *scn)
{
    SDL_Surface *raw = IMG_Load (path);
    sage_require (raw);
    
    sage_image_t *img = malloc (sizeof *img);
    sage_require (img);

    img->screen = sage_screen_surface (scn);
    img->image = SDL_ConvertSurface (raw, img->screen->format, 0);
    sage_require (img);

    SDL_FreeSurface (raw);
    return img;
}


extern sage_image_t*
sage_image_free(sage_image_t *img)
{
    if (sage_likely (img)) {
        SDL_FreeSurface (img->image);
        free (img);
    }

    return NULL;
}


extern SAGE_HOT void
sage_image_render(sage_image_t *img, 
                  const struct sage_point_t loc, 
                  const struct sage_area_t scale)
{
    SDL_Rect r = {.x = loc.x, .y = loc.y, .w = scale.w, .h = scale.h}; 
    SDL_BlitScaled (img->image, NULL, img->screen, &r);
}

