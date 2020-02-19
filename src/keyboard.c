/******************************************************************************
 *                           ____   __    ___  ____ 
 *                          / ___) / _\  / __)(  __)
 *                          \___ \/    \( (_ \ ) _) 
 *                          (____/\_/\_/ \___/(____)
 *
 * Schemable? Game Engine (SAGE) Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>.
 *
 * This code is released under the MIT License. See the accompanying
 * sage/LICENSE.md file or <http://opensource.org/licenses/MIT> for complete 
 * licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT THEM.
 *
 * This is the sage/src/keyboard.c source file; it implements the keyboard API 
 * of the SAGE Library.
 ******************************************************************************/


/*
 * The SDL2/SDL.h header file contains the declaration of the SDL2 API, and the
 * sage/include/api.h header file contains the declaration of the SAGE API.
 */
#include <SDL2/SDL.h>
#include "../include/api.h"


/*
 * The states pointer is a handle to the array of key states managed internally
 * by the SDL2 Library. It has been declared as const because the SDL2 API only
 * allows read access to the key state array through SDL_GetKeyboardState(). As
 * of now, SDL2 supports only one connected keyboard.
 */
static thread_local const uint8_t *states = NULL;


/*
 * The sage_keyboard_start() interface function initialises the keyboard
 * manager. We initialise the handle to the SDL2 key state array  by requesting
 * SDL2 to update the array. We're intentionally not checking whether the
 * keyboard manager has already been intitialised because it's safe to make
 * redundant calls to sage_keyboard_start().
 */
extern void 
sage_keyboard_start(void)
{
    sage_keyboard_state_update();
}


/*
 * The sage_keyboard_stop() interface function shuts down the keyboard manager.
 * As of now, it's essentially a dummy function provided for symmetry since it
 * doesn't need to perform any resource cleanup.
 */
extern void 
sage_keyboard_stop(void)
{
    states = NULL;
}


/*
 * The sage_keyboard_state() interface function gets the current key state of a
 * given keyboard key. We simply return the corresponding value in the SDL2 key
 * state array.
 */
extern SAGE_HOT enum sage_keyboard_state_t 
sage_keyboard_state(enum sage_keyboard_key_t key)
{
    return states[key];
}


/*
 * The sage_keyboard_state_update() interface function updates the current key
 * state of all keyboard keys. We do this by requesting SDL2 to update its
 * internal key state array.
 */
extern SAGE_HOT void 
sage_keyboard_state_update(void)
{
    sage_require(states = SDL_GetKeyboardState(NULL));
}


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 *
 ******************************************************************************/

