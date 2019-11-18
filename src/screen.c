#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"

struct __sage_screen {
    SDL_Window *window;
    SDL_Renderer *renderer;
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
sage_screen_new(const char *title, struct sage_area_t res)
{
    sdl_init ();

    sage_screen_t *scn = malloc (sizeof *scn);
    sage_require (scn != NULL);
    
    scn->window = SDL_CreateWindow (title, 
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, 
                                    res.w, 
                                    res.h,
                                    SDL_WINDOW_SHOWN);
    sage_require (scn->window != NULL);

    scn->renderer = SDL_CreateRenderer (scn->window, 
                                        -1, 
                                        SDL_RENDERER_ACCELERATED);
    sage_require (scn->renderer != NULL);
    SDL_SetRenderDrawColor (scn->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return scn;
}


extern sage_screen_t*
sage_screen_free(sage_screen_t *scn)
{
    if (sage_likely (scn)) {
        SDL_DestroyWindow (scn->window);
        free (scn);
    }

    sdl_quit ();
    return NULL;
}


extern SAGE_HOT void*
sage_screen_brush(sage_screen_t *scn)
{
    return scn->renderer;
}


extern SAGE_HOT void
sage_screen_clear(sage_screen_t *scn)
{
    SDL_RenderClear (scn->renderer);
}


extern SAGE_HOT void
sage_screen_render(sage_screen_t *scn)
{
    SDL_RenderPresent (scn->renderer);
}

