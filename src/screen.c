#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"

struct __sage_screen {
    SDL_Window *window;
    SDL_Surface *surface;
};


extern sage_screen_t*
sage_screen_new(const char *title, struct sage_area_t *resolution)
{
    static bool init = false;

    if (sage_likely (!init)) {
        if (sage_unlikely (SDL_Init (SDL_INIT_VIDEO) < 0)) goto error;
        init = true;

        if (sage_unlikely (!(IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG))) 
            goto error;
    }

    sage_screen_t *screen = malloc (sizeof *screen);
    if (sage_unlikely (!screen)) goto error;
    
    screen->window = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED, resolution->w,
                                       resolution->h, SDL_WINDOW_SHOWN);
    if (sage_unlikely (!screen->window)) goto error;

    screen->surface = SDL_GetWindowSurface (screen->window);

error:
    printf ("failed to initialise window! %s\n", 
            init ? IMG_GetError () : SDL_GetError ());
    init = false;
    exit (EXIT_FAILURE);
}

extern sage_screen_t*
sage_screen_free(sage_screen_t *screen)
{
    SDL_DestroyWindow (screen->window);
    free (screen);

    IMG_Quit ();
    SDL_Quit ();

    return NULL;
}

extern SAGE_HOT void
sage_screen_render(sage_screen_t *screen)
{
}

