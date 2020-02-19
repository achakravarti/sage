
/** MOUSE **/

enum sage_mouse_button_t {
    SAGE_MOUSE_BUTTON_LEFT = 0,
    SAGE_MOUSE_BUTTON_MIDDLE,
    SAGE_MOUSE_BUTTON_RIGHT,

    SAGE_MOUSE_BUTTON_COUNT
};


enum sage_mouse_state_t {
    SAGE_MOUSE_STATE_UP = 0,
    SAGE_MOUSE_STATE_DOWN,

    SAGE_MOUSE_STATE_COUNT
};


extern void
sage_mouse_start(void);

extern void
sage_mouse_stop(void);

extern enum sage_mouse_state_t
sage_mouse_state(enum sage_mouse_button_t btn);

extern void
sage_mouse_state_update(enum sage_mouse_button_t btn,
                        enum sage_mouse_state_t  state);

extern const sage_vector_t *
sage_mouse_vector(void);

extern void
sage_mouse_vector_update(float x, float y);



