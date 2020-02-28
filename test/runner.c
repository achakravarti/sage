#include <SDL2/SDL.h>
#include "../include/api.h"


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
    sage_texture_factory_register(TEX_SAMPLE, "test/res/sample.png");
}


static void
entity_register(void)
{
    sage_entity_factory_register(ENT_SAMPLE, TEX_SAMPLE);
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

    sage_entity_t *ent = sage_entity_factory_spawn (ENT_SAMPLE);
    (void) sage_arena_push (ent);
    sage_entity_free (&ent);

    sage_game_run ();
    sage_game_stop ();

    return 0;
}

