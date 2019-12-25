#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./sage.h"


static thread_local struct {
    SDL_Window *wnd;
    SDL_Renderer *brush;
    struct sage_viewport_t *vp;
} *screen = NULL;


extern void
sage_screen_start(const char *title, struct sage_area_t res)
{
    if (sage_unlikely (screen))
        return;

    sage_require (SDL_Init (SDL_INIT_VIDEO) >= 0);
    sage_require (IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG);

    sage_require (screen = malloc (sizeof *screen));

    sage_require (screen->wnd = SDL_CreateWindow (
        title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, res.w, res.h,
        SDL_WINDOW_SHOWN));

    sage_require (screen->brush = SDL_CreateRenderer (
        screen->wnd, -1, SDL_RENDERER_ACCELERATED));
    SDL_SetRenderDrawColor (screen->brush, 0xFF, 0xFF, 0xFF, 0xFF);
}


extern void
sage_screen_stop(void)
{
    if (sage_likely (screen)) {
        SDL_DestroyRenderer (screen->brush);
        SDL_DestroyWindow (screen->wnd);
        free (screen);
    }

    IMG_Quit ();
    SDL_Quit ();
}


extern SAGE_HOT void*
sage_screen_brush(void)
{
    return screen->brush;
}


extern SAGE_HOT void
sage_screen_clear(const sage_colour_t *col)
{
    SDL_SetRenderDrawColor (screen->brush,
                            sage_colour_red (col),
                            sage_colour_green (col),
                            sage_colour_blue (col),
                            sage_colour_alpha (col));
    SDL_RenderClear (screen->brush);
}


extern SAGE_HOT struct sage_viewport_t *
sage_screen_viewport(void)
{
    struct sage_viewport_t *cp;
    sage_require (cp = malloc (sizeof *cp));

    cp->point = screen->vp->point;
    cp->area = screen->vp->area;

    return cp;
}


extern SAGE_HOT void
sage_screen_viewport_set(const struct sage_viewport_t *vp)
{
    free (screen->vp);
    sage_require (screen->vp = malloc (sizeof *screen->vp));

    screen->vp->point = vp->point;
    screen->vp->area = vp->area;

    SDL_Rect rvp = {.x = screen->vp->point.x,
                    .y = screen->vp->point.y,
                    .w = screen->vp->area.w,
                    .h = screen->vp->area.h};
    SDL_RenderSetViewport (screen->brush, &rvp);
}


extern SAGE_HOT void sage_screen_render(void)
{
    SDL_RenderPresent (screen->brush);
}

