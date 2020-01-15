#include <SDL2/SDL.h>
#include "../src/sage.h"


enum {
    ENT_SAMPLE
};


enum {
    TEX_SAMPLE,
    TEX_COUNT
};


static void
texture_register(void)
{
    sage_texture_t *tex = sage_texture_new ("test/res/sample.png", TEX_SAMPLE);
    sage_texture_factory_register (tex);
    sage_texture_free (tex);
}


static void
entity_register(void)
{
    struct sage_frame_t frm = {.r = 1, .c = 1};
    sage_entity_t *ent = sage_entity_new (ENT_SAMPLE, TEX_SAMPLE, frm, NULL, 
        NULL, NULL);
    sage_entity_factory_register (ent);
    sage_entity_free (ent);
}



int
main(int argc, char *argv[])
{

    (void) argc;
    (void) argv;

    register struct sage_area_t res = {.w = 640, .h = 480};
    sage_screen_start ("Sage Test", res);

    sage_game_start ();
    texture_register ();
    entity_register ();

    //sage_arena_push (sage_entity_factory_spawn (ENT_SAMPLE));
    sage_game_run ();
   
#if 0
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
    sage_game_stop ();*/
    sage_texture_factory_stop ();
#endif

    return 0;
}

