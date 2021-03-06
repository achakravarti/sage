#include <SDL2/SDL.h>
#include "../graphics/graphics.h"
#include "../hid/hid.h"
#include "arena.h"


static thread_local struct {
    bool run;
    SDL_Event event;
} *game = NULL;


static void 
listen(void)
{
    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
            case SDL_QUIT:
                game->run = false;
                break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
                sage_keyboard_state_update();
                break;

            case SDL_MOUSEBUTTONUP:
                sage_mouse_state_update(game->event.button.button,
                                        SAGE_MOUSE_STATE_UP);
                break;

            case SDL_MOUSEBUTTONDOWN:
                sage_mouse_state_update(game->event.button.button,
                                        SAGE_MOUSE_STATE_DOWN);
                break;

            default:
                break;
         }
    }
}


extern void sage_game_start(void)
{
    if (sage_likely(!game)) {
        sage_heap_init();

        game = sage_heap_new(sizeof *game);
        game->run = true;

        sage_mouse_init();
        sage_keyboard_init();
        sage_texture_factory_init();
        sage_entity_factory_init();
        sage_arena_start();
        sage_stage_init();
    }
}


extern void sage_game_stop(void)
{
    sage_arena_stop();
    sage_stage_exit();
    sage_entity_factory_exit();
    sage_texture_factory_exit();
    sage_keyboard_exit();
    sage_mouse_exit();

    sage_heap_free((void **) &game);
    sage_heap_exit();
}


extern void sage_game_run(void)
{
    // TODO: problem with hues needs to be fixed
    sage_colour_t *black = sage_colour_new_hue (SAGE_HUE_BLACK);

    while (sage_likely(game->run)) {
        listen();
        sage_arena_update();

        sage_screen_clear(black);
        sage_arena_draw();
        sage_screen_render();
    }

    sage_colour_free(black);
}

