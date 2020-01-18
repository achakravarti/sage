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
    sage_texture_free (&tex);
}


static void
entity_register(void)
{
    struct sage_frame_t frm = {.r = 1, .c = 1};
    struct sage_entity_vtable_t vt = {.update = NULL, .free = NULL, 
        .draw = NULL};
    sage_entity_t *ent = sage_entity_new (ENT_SAMPLE, TEX_SAMPLE, frm, &vt);
    sage_entity_factory_register (ent);
    sage_entity_free (&ent);
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

