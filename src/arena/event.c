#include <SDL2/SDL.h>
#include "../hid/hid.h"
#include "arena.h"


static thread_local bool event_init = false;


extern void sage_event_start(void)
{
    event_init = true;
}


extern void sage_event_stop(void)
{
    event_init = false;
}


extern SAGE_HOT void sage_event_run(void)
{
    SDL_Event e;
    while (SDL_PollEvent (&e)) {
        switch (e.type) {
        case SDL_QUIT:
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            sage_keyboard_state_update();
            break;

        default:
            break;
        }
    }
}

