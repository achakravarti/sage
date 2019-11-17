#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"

struct __sage_screen {
    SDL_Window *window;
    SDL_Surface *surface;
};




static void
sdl_init(void)
{
    static bool init = false;

    if (sage_likely (!init)) {
        sage_require (SDL_Init (SDL_INIT_VIDEO) >= 0);
        sage_require (IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG);
    }
}


static inline void
sdl_quit(void)
{
    IMG_Quit ();
    SDL_Quit ();
}


extern sage_screen_t*
sage_screen_new(const char *title, struct sage_area_t *resolution)
{
    sdl_init ();

    sage_screen_t *screen = malloc (sizeof *screen);
    sage_require (screen);
    
    screen->window = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED, resolution->w,
                                       resolution->h, SDL_WINDOW_SHOWN);
    sage_require (screen->window);

    sage_require ((screen->surface = SDL_GetWindowSurface (screen->window)));
    return screen;
}


extern sage_screen_t*
sage_screen_free(sage_screen_t *screen)
{
    if (sage_likely (screen)) {
        SDL_DestroyWindow (screen->window);
        free (screen);
    }

    sdl_quit ();
    return NULL;
}


extern SAGE_HOT void
sage_screen_render(sage_screen_t *screen)
{
    SDL_UpdateWindowSurface (screen->window);
}

