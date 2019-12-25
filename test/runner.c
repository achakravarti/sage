#include <SDL2/SDL.h>
#include "../src/sage.h"


int
main(int argc, char *argv[])
{

    (void) argc;
    (void) argv;

    register struct sage_area_t res = {.w = 640, .h = 480};
    sage_screen_start ("Sage Test", res);

    sage_texture_t *tex = sage_texture_new ("test/res/sample.png");
   
    bool quit = false;
    SDL_Event e;

    register struct sage_point_t loc = {.x = 0, .y = 0};
    sage_colour_t *blue = sage_colour_new_hue (SAGE_HUE_CORNFLOWERBLUE);
    while (!quit) {
        while (SDL_PollEvent (&e)) {
            quit = (e.type == SDL_QUIT);

            sage_screen_clear (blue);
            sage_texture_draw (tex, loc);
            sage_screen_render ();
        }
    }

    tex = sage_texture_free (tex);
    sage_screen_stop ();

    return 0;
}

