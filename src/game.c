#include <SDL2/SDL.h>
#include "sage.h"


static thread_local struct {
    bool run;
    SDL_Event event;
} *game = NULL;


static void event_listen(void)
{
    while (SDL_PollEvent (&game->event)) {
        switch (game->event.type) {
        case SDL_QUIT:
            game->run = false;
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            sage_keyboard_input ();
            break;

        default:
            break;
        }
    }
}


extern void sage_game_start(void)
{
    if (sage_likely (!game)) {
        sage_require (game = malloc (sizeof *game));
        game->run = true;

        sage_texture_factory_start ();
        sage_entity_factory_start ();
        sage_arena_start ();
    }
}


extern void sage_game_stop(void)
{
    sage_arena_stop ();
    sage_entity_factory_stop ();
    sage_texture_factory_stop ();

    free (game);
}


extern void sage_game_run(void)
{
    // TODO: problem with hues needs to be fixed
    sage_colour_t *black = sage_colour_new_hue (SAGE_HUE_BLACK);

    while (sage_likely (game->run)) {
        event_listen ();
        sage_arena_update ();

        sage_screen_clear (black);
        sage_arena_draw ();
        sage_screen_render ();
    }

    sage_colour_free (black);
}

