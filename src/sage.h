#if (!defined SCHEME_ASSISTED_GAME_ENGINE)
#   define SCHEME_ASSISTED_GAME_ENGINE


#include <stdbool.h>
#include <inttypes.h>
#include <threads.h>
#include <stdio.h>
#include <stdlib.h>


#if defined __cplusplus
    extern "C" {
#endif


#define SAGE_COMPILER_GCC (0)
#define SAGE_COMPILER_CLANG (1)

/* https://stackoverflow.com/questions/38499462/ */
#if (defined __GNUC__ && !defined __clang__)
#   define sage_compiler() SAGE_COMPILER_GCC
#elif (defined __clang__)
#   define sage_compiler() SAGE_COMPILER_CLANG
#else
#   error "sage_compiler(): unsupported C compiler"
#endif


#define sage_compiler_gnuex()              \
    (sage_compiler () == SAGE_COMPILER_GCC \
     || sage_compiler () == SAGE_COMPILER_CLANG)


#if (sage_compiler_gnuex ())
#   define SAGE_PURE __attribute__((pure))
#else
#   define SAGE_PURE 
#   warning "SAGE_PURE has no effect on non GCC-compatible compilers" 
#endif


#if (sage_compiler_gnuex ())
#   define SAGE_HOT __attribute__((hot))
#else
#   define SAGE_HOT
#   warning "SAGE_HOT has no effect on non GCC-compatible compilers"
#endif


#if (sage_compiler_gnuex ())
#   define SAGE_COLD __attribute__((cold))
#else
#   define SAGE_COLD
#   warning "SAGE_COLD has no effect on non GCC-compatible compilers"
#endif


#if (sage_compiler_gnuex ())
#   define sage_likely(p) (__builtin_expect (!!(p), 1))
#else
#   define sage_likely(p) (p)
#   warning "sage_likely() has no effect on non GCC-compatible compilers"
#endif


#if (sage_compiler_gnuex ())
#   define sage_unlikely(p) (__builtin_expect (!!(p), 0))
#else
#   define sage_unlikely(p) (p)
#   warning "sage_unlikely() has no effect on non GCC-compatible compilers"
#endif


#if !(defined NDEBUG)
#   define sage_assert(c)                                                \
    do {                                                                 \
        if (sage_unlikely (!(c))) {                                      \
            printf ("sage_assert() condition failed: %s [%s, %s, %d]\n", \
                    #c, __func__, __FILE__, __LINE__);                   \
            abort ();                                                    \
        }                                                                \
    } while (0)
#else
#   define sage_assert(c)
#endif


#define sage_require(c)                                                 \
do {                                                                    \
    if (sage_unlikely (!(c))) {                                         \
        printf ("sage_require() condition failed @ %s() [%s:%d]: %s\n", \
                __func__, __FILE__, __LINE__, #c);                      \
        exit (EXIT_FAILURE);                                            \
    }                                                                   \
} while (0)


struct sage_point_t {
    int16_t x;
    int16_t y;
};


struct sage_area_t {
    uint16_t h;
    uint16_t w;
};


enum sage_compare_t {
    SAGE_COMPARE_LT = -1,
    SAGE_COMPARE_EQ = 0,
    SAGE_COMPARE_GT = 1
};


/** VECTOR **/

typedef struct sage_vector_t sage_vector_t;

extern sage_vector_t *
sage_vector_new(float x, float y);

extern sage_vector_t *
sage_vector_new_zero(void);

extern sage_vector_t *
sage_vector_copy(const sage_vector_t *src);

extern sage_vector_t *
sage_vector_free(sage_vector_t *ctx);

extern float 
sage_vector_x(const sage_vector_t *ctx);

extern float 
sage_vector_y(const sage_vector_t *ctx);

extern void 
sage_vector_x_set(sage_vector_t *ctx, float x);

extern void 
sage_vector_y_set(sage_vector_t *ctx, float y);

extern float 
sage_vector_len(const sage_vector_t *ctx);

extern void 
sage_vector_norm(sage_vector_t *ctx);

extern enum sage_compare_t 
sage_vector_cmp(const sage_vector_t *lhs, const sage_vector_t *rhs);

extern void 
sage_vector_add(sage_vector_t *ctx, const sage_vector_t *add);

extern void 
sage_vector_sub(sage_vector_t *ctx, const sage_vector_t *sub);

extern void 
sage_vector_mul(sage_vector_t *ctx, float mul);

extern void 
sage_vector_div(sage_vector_t *ctx, const float div);


/** SCREEN **/


extern void
sage_screen_start(const char *title, struct sage_area_t res);

extern void
sage_screen_stop(void);

extern SAGE_HOT void*
sage_screen_brush(void);

extern SAGE_HOT void
sage_screen_clear(void);

extern SAGE_HOT void sage_screen_render(void);


/** KEYBOARD **/


enum sage_keyboard_key_t {
    SAGE_KEYBOARD_KEY_NONE = 0x00,     /* No key pressed */
    SAGE_KEYBOARD_KEY_OVERFLOW = 0x01, /* Too many keys pressed */

    SAGE_KEYBOARD_KEY_A = 0x04, /* Keyboard a and A */
    SAGE_KEYBOARD_KEY_B = 0x05, /* Keyboard b and B */
    SAGE_KEYBOARD_KEY_C = 0x06, /* Keyboard c and C */
    SAGE_KEYBOARD_KEY_D = 0x07, /* Keyboard d and D */
    SAGE_KEYBOARD_KEY_E = 0x08, /* Keyboard e and E */
    SAGE_KEYBOARD_KEY_F = 0x09, /* Keyboard f and F */
    SAGE_KEYBOARD_KEY_G = 0x0a, /* Keyboard g and G */
    SAGE_KEYBOARD_KEY_H = 0x0b, /* Keyboard h and H */
    SAGE_KEYBOARD_KEY_I = 0x0c, /* Keyboard i and I */
    SAGE_KEYBOARD_KEY_J = 0x0d, /* Keyboard j and J */
    SAGE_KEYBOARD_KEY_K = 0x0e, /* Keyboard k and K */
    SAGE_KEYBOARD_KEY_L = 0x0f, /* Keyboard l and L */
    SAGE_KEYBOARD_KEY_M = 0x10, /* Keyboard m and M */
    SAGE_KEYBOARD_KEY_N = 0x11, /* Keyboard n and N */
    SAGE_KEYBOARD_KEY_O = 0x12, /* Keyboard o and O */
    SAGE_KEYBOARD_KEY_P = 0x13, /* Keyboard p and P */
    SAGE_KEYBOARD_KEY_Q = 0x14, /* Keyboard q and Q */
    SAGE_KEYBOARD_KEY_R = 0x15, /* Keyboard r and R */
    SAGE_KEYBOARD_KEY_S = 0x16, /* Keyboard s and S */
    SAGE_KEYBOARD_KEY_T = 0x17, /* Keyboard t and T */
    SAGE_KEYBOARD_KEY_U = 0x18, /* Keyboard u and U */
    SAGE_KEYBOARD_KEY_V = 0x19, /* Keyboard v and V */
    SAGE_KEYBOARD_KEY_W = 0x1a, /* Keyboard w and W */
    SAGE_KEYBOARD_KEY_X = 0x1b, /* Keyboard x and X */
    SAGE_KEYBOARD_KEY_Y = 0x1c, /* Keyboard y and Y */
    SAGE_KEYBOARD_KEY_Z = 0x1d, /* Keyboard z and Z */

    SAGE_KEYBOARD_KEY_1 = 0x1e, /* Keyboard 1 and ! */
    SAGE_KEYBOARD_KEY_2 = 0x1f, /* Keyboard 2 and @ */
    SAGE_KEYBOARD_KEY_3 = 0x20, /* Keyboard 3 and # */
    SAGE_KEYBOARD_KEY_4 = 0x21, /* Keyboard 4 and $ */
    SAGE_KEYBOARD_KEY_5 = 0x22, /* Keyboard 5 and % */
    SAGE_KEYBOARD_KEY_6 = 0x23, /* Keyboard 6 and ^ */
    SAGE_KEYBOARD_KEY_7 = 0x24, /* Keyboard 7 and & */
    SAGE_KEYBOARD_KEY_8 = 0x25, /* Keyboard 8 and * */
    SAGE_KEYBOARD_KEY_9 = 0x26, /* Keyboard 9 and ( */
    SAGE_KEYBOARD_KEY_0 = 0x27, /* Keyboard 0 and ) */

    SAGE_KEYBOARD_KEY_ENTER = 0x28,      /* Keyboard Enter */
    SAGE_KEYBOARD_KEY_ESC = 0x29,        /* Keyboard Escape */
    SAGE_KEYBOARD_KEY_BKSPACE = 0x2a,    /* Keyboard Backspace */
    SAGE_KEYBOARD_KEY_TAB = 0x2b,        /* Keyboard Tab */
    SAGE_KEYBOARD_KEY_SPACE = 0x2c,      /* Keyboard Spacebar */
    SAGE_KEYBOARD_KEY_MINUS = 0x2d,      /* Keyboard - and _ */
    SAGE_KEYBOARD_KEY_EQUAL = 0x2e,      /* Keyboard = and + */
    SAGE_KEYBOARD_KEY_LBRACE = 0x2f,     /* Keyboard [ and { */
    SAGE_KEYBOARD_KEY_RBRACE = 0x30,     /* Keyboard ] and } */
    SAGE_KEYBOARD_KEY_BKSLASH = 0x31,    /* Keyboard \ and | */
    SAGE_KEYBOARD_KEY_HASHTILDE = 0x32,  /* Keyboard Non-US # and ~ */
    SAGE_KEYBOARD_KEY_SEMICOLON = 0x33,  /* Keyboard ; and : */
    SAGE_KEYBOARD_KEY_APOSTROPHE = 0x34, /* Keyboard ' and " */
    SAGE_KEYBOARD_KEY_GRAVE = 0x35,      /* Keyboard ` and ~ */
    SAGE_KEYBOARD_KEY_COMMA = 0x36,      /* Keyboard , and < */
    SAGE_KEYBOARD_KEY_DOT = 0x37,        /* Keyboard . and > */
    SAGE_KEYBOARD_KEY_SLASH = 0x38,      /* Keyboard / and ? */
    SAGE_KEYBOARD_KEY_CAPSLOCK = 0x39,   /* Keyboard Caps Lock */

    SAGE_KEYBOARD_KEY_F1 = 0x3a,  /* Keyboard F1 */
    SAGE_KEYBOARD_KEY_F2 = 0x3b,  /* Keyboard F2 */
    SAGE_KEYBOARD_KEY_F3 = 0x3c,  /* Keyboard F3 */
    SAGE_KEYBOARD_KEY_F4 = 0x3d,  /* Keyboard F4 */
    SAGE_KEYBOARD_KEY_F5 = 0x3e,  /* Keyboard F5 */
    SAGE_KEYBOARD_KEY_F6 = 0x3f,  /* Keyboard F6 */
    SAGE_KEYBOARD_KEY_F7 = 0x40,  /* Keyboard F7 */
    SAGE_KEYBOARD_KEY_F8 = 0x41,  /* Keyboard F8 */
    SAGE_KEYBOARD_KEY_F9 = 0x42,  /* Keyboard F9 */
    SAGE_KEYBOARD_KEY_F10 = 0x43, /* Keyboard F10 */
    SAGE_KEYBOARD_KEY_F11 = 0x44, /* Keyboard F11 */
    SAGE_KEYBOARD_KEY_F12 = 0x45, /* Keyboard F12 */

    SAGE_KEYBOARD_KEY_SYSRQ = 0x46,      /* Keyboard Print Screen */
    SAGE_KEYBOARD_KEY_SCROLLLOCK = 0x47, /* Keyboard Scroll Lock */
    SAGE_KEYBOARD_KEY_PAUSE = 0x48,      /* Keyboard Pause */
    SAGE_KEYBOARD_KEY_INSERT = 0x49,     /* Keyboard Insert */
    SAGE_KEYBOARD_KEY_HOME = 0x4a,       /* Keyboard Home */
    SAGE_KEYBOARD_KEY_PAGEUP = 0x4b,     /* Keyboard Page Up */
    SAGE_KEYBOARD_KEY_DELETE = 0x4c,     /* Keyboard Delete */
    SAGE_KEYBOARD_KEY_END = 0x4d,        /* Keyboard End */
    SAGE_KEYBOARD_KEY_PAGEDOWN = 0x4e,   /* Keyboard Page Down */
    SAGE_KEYBOARD_KEY_RIGHT = 0x4f,      /* Keyboard Right Arrow */
    SAGE_KEYBOARD_KEY_LEFT = 0x50,       /* Keyboard Left Arrow */
    SAGE_KEYBOARD_KEY_DOWN = 0x51,       /* Keyboard Down Arrow */
    SAGE_KEYBOARD_KEY_UP = 0x52,         /* Keyboard Up Arrow */

    SAGE_KEYBOARD_KEY_NUMLOCK = 0x53,    /* Keyboard Num Lock and Clear */
    SAGE_KEYBOARD_KEY_KPSLASH = 0x54,    /* Keypad / */
    SAGE_KEYBOARD_KEY_KPASTERISK = 0x55, /* Keypad * */
    SAGE_KEYBOARD_KEY_KPMINUS = 0x56,    /* Keypad - */
    SAGE_KEYBOARD_KEY_KPPLUS = 0x57,     /* Keypad + */
    SAGE_KEYBOARD_KEY_KPENTER = 0x58,    /* Keypad Enter */
    SAGE_KEYBOARD_KEY_KP1 = 0x59,        /* Keypad 1 and End */
    SAGE_KEYBOARD_KEY_KP2 = 0x5a,        /* Keypad 2 and Down Arrow */
    SAGE_KEYBOARD_KEY_KP3 = 0x5b,        /* Keypad 3 and PageDn */
    SAGE_KEYBOARD_KEY_KP4 = 0x5c,        /* Keypad 4 and Left Arrow */
    SAGE_KEYBOARD_KEY_KP5 = 0x5d,        /* Keypad 5 */
    SAGE_KEYBOARD_KEY_KP6 = 0x5e,        /* Keypad 6 and Right Arrow */
    SAGE_KEYBOARD_KEY_KP7 = 0x5f,        /* Keypad 7 and Home */
    SAGE_KEYBOARD_KEY_KP8 = 0x60,        /* Keypad 8 and Up Arrow */
    SAGE_KEYBOARD_KEY_KP9 = 0x61,        /* Keypad 9 and Page Up */
    SAGE_KEYBOARD_KEY_KP0 = 0x62,        /* Keypad 0 and Insert */
    SAGE_KEYBOARD_KEY_KPDOT = 0x63,      /* Keypad . and Delete */

    SAGE_KEYBOARD_KEY_102ND = 0x64,      /* Keyboard Non-US \ and | */
    SAGE_KEYBOARD_KEY_COMPOSE = 0x65,    /* Keyboard Application */
    SAGE_KEYBOARD_KEY_POWER = 0x66,      /* Keyboard Power */
    SAGE_KEYBOARD_KEY_OPEN = 0x74,       /* Keyboard Execute */
    SAGE_KEYBOARD_KEY_HELP = 0x75,       /* Keyboard Help */
    SAGE_KEYBOARD_KEY_PROPS = 0x76,      /* Keyboard Menu */
    SAGE_KEYBOARD_KEY_FRONT = 0x77,      /* Keyboard Select */
    SAGE_KEYBOARD_KEY_STOP = 0x78,       /* Keyboard Stop */
    SAGE_KEYBOARD_KEY_AGAIN = 0x79,      /* Keyboard Again */
    SAGE_KEYBOARD_KEY_UNDO = 0x7a,       /* Keyboard Undo */
    SAGE_KEYBOARD_KEY_CUT = 0x7b,        /* Keyboard Cut */
    SAGE_KEYBOARD_KEY_COPY = 0x7c,       /* Keyboard Copy */
    SAGE_KEYBOARD_KEY_PASTE = 0x7d,      /* Keyboard Paste */
    SAGE_KEYBOARD_KEY_FIND = 0x7e,       /* Keyboard Find */
    SAGE_KEYBOARD_KEY_MUTE = 0x7f,       /* Keyboard Mute */
    SAGE_KEYBOARD_KEY_VOLUMEUP = 0x80,   /* Keyboard Volume Up */
    SAGE_KEYBOARD_KEY_VOLUMEDOWN = 0x81, /* Keyboard Volume Down */

    SAGE_KEYBOARD_KEY_LCTRL = 0xe0,  /* Keyboard Left Control */
    SAGE_KEYBOARD_KEY_LSHIFT = 0xe1, /* Keyboard Left Shift */
    SAGE_KEYBOARD_KEY_LALT = 0xe2,   /* Keyboard Left Alt */
    SAGE_KEYBOARD_KEY_LMETA = 0xe3,  /* Keyboard Left GUI */
    SAGE_KEYBOARD_KEY_RCTRL = 0xe4,  /* Keyboard Right Control */
    SAGE_KEYBOARD_KEY_RSHIFT = 0xe5, /* Keyboard Right Shift */
    SAGE_KEYBOARD_KEY_RALT = 0xe6,   /* Keyboard Right Alt */
    SAGE_KEYBOARD_KEY_RMETA = 0xe7,  /* Keyboard Right GUI */
};


enum sage_keyboard_state_t {
        SAGE_KEYBOARD_STATE_UP = 0,
        SAGE_KEYBOARD_STATE_DOWN
};


extern void sage_keyboard_start(void);

extern void sage_keyboard_stop(void);

extern SAGE_HOT enum sage_keyboard_state_t sage_keyboard_state(
    enum sage_keyboard_key_t key);

extern SAGE_HOT void sage_keyboard_input(void);


/** EVENT **/


extern void sage_event_start(void);

extern void sage_event_stop(void);

extern SAGE_HOT void sage_event_run(void);


/** TEXTURE **/


typedef struct __sage_texture sage_texture_t;

extern SAGE_HOT sage_texture_t *sage_texture_new(const char *path);

extern sage_texture_t*
sage_texture_free(sage_texture_t *tex);

extern SAGE_HOT struct sage_area_t sage_texture_area(const sage_texture_t *tex);

extern SAGE_HOT void
sage_texture_draw(sage_texture_t *tex, struct sage_point_t dst);

extern SAGE_HOT void
sage_texture_draw_clipped(sage_texture_t *tex, 
                          struct sage_point_t dst,
                          struct sage_point_t src,
                          struct sage_area_t clip);

extern SAGE_HOT void
sage_texture_draw_scaled(sage_texture_t *tex, 
                         struct sage_point_t dst,
                         struct sage_area_t proj,
                         struct sage_point_t src,
                         struct sage_area_t clip);


/** SPRITE **/


struct sage_frame_t {
    uint16_t r;
    uint16_t c;
};


typedef struct sage_sprite_t sage_sprite_t;

extern sage_sprite_t *
sage_sprite_new(const char *path, struct sage_frame_t tot);

extern sage_sprite_t *
sage_sprite_copy(const sage_sprite_t *src);

extern sage_sprite_t *
sage_sprite_free(sage_sprite_t *ctx);

extern struct sage_area_t 
sage_sprite_area(const sage_sprite_t *ctx);

extern struct sage_frame_t 
sage_sprite_frame(const sage_sprite_t *ctx);

extern SAGE_HOT void 
sage_sprite_frame_set(sage_sprite_t *ctx, struct sage_frame_t frm);

extern size_t 
sage_sprite_frame_count(const sage_sprite_t *ctx);

extern SAGE_HOT void 
sage_sprite_draw(const sage_sprite_t *ctx, struct sage_point_t dst);

extern SAGE_HOT void 
sage_sprite_draw_scaled(const sage_sprite_t *ctx, 
                        struct sage_point_t dst,
                        struct sage_area_t prj);


extern void 
sage_sprite_factory_start(void);

extern void 
sage_sprite_factory_stop(void);

extern void 
sage_sprite_factory_register(const sage_sprite_t *spr);

extern sage_sprite_t *
sage_sprite_factory_spawn(size_t idx);


#if defined __cplusplus
    } /* extern "C" */
#endif

#endif /* SCHEME_ASSISTED_GAME_ENGINE */

