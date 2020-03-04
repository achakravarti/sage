#include <stdio.h>
#include <SDL2/SDL.h>
#include "../src/arena/arena.h"


enum {
    ENT_SAMPLE = 1
};


enum {
    TEX_SAMPLE = 1,
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
    struct sage_frame_t frm = { .r = 1, .c = 1 };
    sage_entity *ent = sage_entity_new_default(ENT_SAMPLE, TEX_SAMPLE, frm);
    sage_entity_factory_register(ent);
    sage_entity_free(&ent);
}


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    register struct sage_area_t res = {.w = 640, .h = 480};
    sage_screen_start("Sage Test", res);

    sage_game_start();
    texture_register();
    entity_register();

    sage_entity *ent = sage_entity_factory_clone (ENT_SAMPLE);
    (void) sage_arena_push (ent);
    sage_entity_free(&ent);

    sage_game_run();
    sage_game_stop();

    return 0;
}

