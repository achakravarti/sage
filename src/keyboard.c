#include <SDL2/SDL.h>
#include "./sage.h"


static thread_local const uint8_t *key_states = NULL;


extern void sage_keyboard_start(void)
{
    if (sage_likely (!key_states))
        sage_require (key_states = SDL_GetKeyboardState (NULL));
}


extern void sage_keyboard_stop(void)
{
    key_states = NULL;
}


extern SAGE_HOT enum sage_keyboard_state_t sage_keyboard_state(
    enum sage_keyboard_key_t key)
{
    return key_states [key];
}


extern SAGE_HOT void sage_keyboard_input(void)
{
    sage_require (key_states = SDL_GetKeyboardState (NULL));
}

