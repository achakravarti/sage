#include <SDL2/SDL.h>
#include "../src/sage.h"


int
main(int argc, char *argv[])
{

    (void) argc;
    (void) argv;

    register struct sage_area_t res = {.w = 640, .h = 480};
    sage_screen_t *scn = sage_screen_new ("Sage Test", res);

    sage_image_t *img = sage_image_new ("test/res/sample.png", scn);
   
    bool quit = false;
    SDL_Event e;

    register struct sage_point_t loc = {.x = 0, .y = 0};
    while (!quit) {
        while (SDL_PollEvent (&e)) {
            quit = (e.type == SDL_QUIT);

            sage_image_render (img, loc, res);
            sage_screen_render (scn);
        }
    }

    sage_image_free (img);
    sage_screen_free (scn);

    return 0;
}

