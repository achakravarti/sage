#include "./sage.h"

static thread_local enum sage_mouse_state_t states[SAGE_MOUSE_BUTTON_COUNT];


static thread_local struct {
    enum sage_mouse_state_t states[SAGE_MOUSE_BUTTON_COUNT];
    sage_vector_t *vec;
} mouse;

extern void
sage_mouse_start(void)
{
    for (register size_t i = 0; i < SAGE_MOUSE_BUTTON_COUNT; i++)
        mouse.states[i] = SAGE_MOUSE_STATE_UP;

    mouse.vec = sage_vector_new_zero();
}


extern void
sage_mouse_stop(void)
{
    for (register size_t i = 0; i < SAGE_MOUSE_BUTTON_COUNT; i++)
        mouse.states[i] = SAGE_MOUSE_STATE_UP;

    sage_vector_free(&mouse.vec);
}


extern enum sage_mouse_state_t
sage_mouse_state(enum sage_mouse_button_t btn)
{
    sage_assert(mouse.vec);
    return states[btn];
}


extern void
sage_mouse_state_update(enum sage_mouse_button_t btn,
                        enum sage_mouse_state_t  state)
{
    sage_assert(mouse.vec);
    states[btn] = state;
}


extern const sage_vector_t *
sage_mouse_vector(void)
{
    sage_assert(mouse.vec);
    return mouse.vec;
}

extern void
sage_mouse_vector_update(float x, float y)
{
    sage_assert(mouse.vec);
    sage_vector_x_set(mouse.vec, x);
    sage_vector_y_set(mouse.vec, y);
}

