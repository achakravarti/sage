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
 * This is the sage/src/mouse.c source file; it implements the mouse API of the
 * SAGE Library.
 ******************************************************************************/


/*
 * The sage/include/api.h header file contains the declaration of the API of the
 * SAGE Library.
 */
#include "hid.h"


/*
 * The mouse singleton reifies the mouse attached to the computer on which the
 * SAGE Library is running. The mouse singleton holds the current state of each
 * mouse button, and the current position vector of the cursor. The mouse
 * singleton assumes that there is only one connected mouse since the SDL2 API
 * does not yet support multiple mouse devices.
 */
static thread_local struct {
    enum sage_mouse_state_t states[SAGE_MOUSE_BUTTON_COUNT];
    sage_vector_t           *pos;
} mouse;


/*
 * The sage_mouse_start() interface function initialises the mouse manager. We
 * initialise the fields of the mouse singleton to their default values.
 */
extern void
sage_mouse_start(void)
{
    for (register size_t i = 0; i < SAGE_MOUSE_BUTTON_COUNT; i++)
        mouse.states[i] = SAGE_MOUSE_STATE_UP;

    mouse.pos = sage_vector_new_zero();
}


/*
 * The sage_mouse_stop() interface function shuts down the mouse manager. We
 * simply release the position vector field of the mouse singleton. We don't
 * need to check whether the position vector field has been initialised because
 * it's safe to call sage_vector_free() even with uninitialised instances.
 */
extern void
sage_mouse_stop(void)
{
    sage_vector_free(&mouse.pos);
}


/*
 * The sage_mouse_state() interface function returns the current state of a
 * given mouse button. The current state is retrieved from the mouse singleton.
 * We indirectly assert that the mouse singleton has been intialised by checking
 * whether its position field has been allocated.
 */
extern enum sage_mouse_state_t
sage_mouse_state(enum sage_mouse_button_t btn)
{
    sage_assert(mouse.pos);
    return mouse.states[btn];
}


/*
 * The sage_mouse_state_update() interface function updates the current state of
 * a given mouse button. This is done by setting the states field of the mouse
 * singleton appropriately after asserting indirectly that the mouse singleton
 * has been initialised.
 */
extern void
sage_mouse_state_update(enum sage_mouse_button_t btn,
                        enum sage_mouse_state_t  state)
{
    sage_assert(mouse.pos);
    mouse.states[btn] = state;
}


/*
 * The sage_mouse_vector() returns the current position vector of the mouse. We
 * return a handle to the position vector field of the mouse singleton since it
 * saves on having to make another copy on the heap. TODO: return a lazy copy
 * once copy on write is implemented in the vector interface.
 */
extern const sage_vector_t *
sage_mouse_vector(void)
{
    sage_assert(mouse.pos);
    return mouse.pos;
}


/*
 * The sage_mouse_vector_update() interface function updates the current
 * position vector of the mouse. We do so by updating the position vector field
 * of the mouse singleton, after indirectly asserting that the mouse singleton
 * has been initialised.
 */
extern void
sage_mouse_vector_update(float x, float y)
{
    sage_assert(mouse.pos);
    sage_vector_x_set(mouse.pos, x);
    sage_vector_y_set(mouse.pos, y);
}


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 *
 ******************************************************************************/

