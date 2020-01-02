#include <SDL2/SDL.h>
#include "../src/sage.h"


enum {
    TEX_SAMPLE,
    TEX_COUNT
};


static void
factory_start(void)
{
    //sage_texture_factory_start (TEX_COUNT);
    sage_texture_factory_start ();

    sage_texture_t *tex = sage_texture_new ("test/res/sample.png", TEX_SAMPLE);
    sage_texture_factory_register (tex);
}


int
main(int argc, char *argv[])
{

    (void) argc;
    (void) argv;

    register struct sage_area_t res = {.w = 640, .h = 480};
    sage_screen_start ("Sage Test", res);
    factory_start ();

    sage_texture_t *tex = sage_texture_factory_spawn (TEX_SAMPLE);
   
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
    sage_texture_factory_stop ();

    return 0;
}

