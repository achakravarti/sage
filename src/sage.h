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


/**************************************************************************//**
 * @defgroup hint Compiler Hints Module
 * Compiler hints for optimisation.
 *
 * Compilers such as GCC have always focused aggressively on optimisation. Many
 * such optimisations have been introduced as compiler-specific extensions that
 * are @b not part of the C standards.
 *
 * The Compiler Hints Module abstracts such optimisations as conditional macros
 * so that they they can be taken advantage of in GCC and GCC-compatible 
 * compilers such as Clang, but at the same time degrade safely on other 
 * compilers.
 * @{
 */


/**
 * Hints that a function is called frequently.
 *
 * The @c SAGE_HOT macro is used to decorate a function declaration in order to
 * indicate that it is called frequently. This hint follows the GCC attribute
 * syntax, and is recommended to be placed in the function declaration just
 * before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see SAGE_COLD
 */
#if (sage_compiler_gnuex ())
#   define SAGE_HOT __attribute__((hot))
#else
#   define SAGE_HOT
#   warning "SAGE_HOT has no effect on non GCC-compatible compilers"
#endif


/**
 * Hints that a function is called infrequently.
 *
 * The @c SAGE_COLD macro is used to decorate a function declaration in order to
 * indicate that is called infrequently. This hint follows the GCC attribute
 * syntax, and is recommended to be placed in the function declaration just
 * before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see SAGE_HOT
 */
#if (sage_compiler_gnuex ())
#   define SAGE_COLD __attribute__((cold))
#else
#   define SAGE_COLD
#   warning "SAGE_COLD has no effect on non GCC-compatible compilers"
#endif


/**
 * Hints that a predicate is likely to be true.
 *
 * The @c sage_likely() macro indicates that a predicate expression @p p is @b
 * likely to evaluate to @c true. This hint has been modelled after the @c
 * likely() macro used in the Linux kernel source.
 *
 * @param p Predicate expected to be true.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see sage_unlikely()
 */
#if (sage_compiler_gnuex ())
#   define sage_likely(p) (__builtin_expect (!!(p), 1))
#else
#   define sage_likely(p) (p)
#   warning "sage_likely() has no effect on non GCC-compatible compilers"
#endif


/**
 * Hints that a predicate is likely to be false.
 *
 * The @c sage_unlikely() macro indicates that a predicate expression @p p is @b
 * unlikely to evaluate to @c false. This hint has been modelled after the @c
 * unlikely() macro used in the Linux kernel source.
 *
 * @param p Predicate expected to be false.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see sage_likely()
 */
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


/* https://htmlcolorcodes.com/color-names/ */
enum sage_hue_t {
    /* Red */
    SAGE_HUE_INDIANRED = 0xCD5C5C,
    SAGE_HUE_LIGHTCORAL = 0xF08080,
    SAGE_HUE_SALMON = 0xFA8072,
    SAGE_HUE_DARKSALMON = 0xE9967A,
    SAGE_HUE_LIGHTSALMON = 0xFFA07A,
    SAGE_HUE_CRIMSON = 0xDC143C,
    SAGE_HUE_RED = 0xFF0000,
    SAGE_HUE_FIREBRICK = 0xB22222,
    SAGE_HUE_DARKRED = 0x8B0000,

    /* Pink */
    SAGE_HUE_PINK = 0xFFC0CB,
    SAGE_HUE_LIGHTPINK = 0xFFB6C1,
    SAGE_HUE_HOTPINK = 0xFF69B4,
    SAGE_HUE_DEEPPINK = 0xFF1493,
    SAGE_HUE_MEDIUMVIOLETRED = 0xC71585,
    SAGE_HUE_PALEVIOLETRED = 0xDB7093,

    /* Orange */
    SAGE_HUE_CORAL = 0xFF7F50,
    SAGE_HUE_TOMATO = 0xFF6347,
    SAGE_HUE_ORANGERED = 0xFF4500,
    SAGE_HUE_DARKORANGE = 0xFF8C00,
    SAGE_HUE_ORANGE = 0xFFA500,

    /* Yellow */
    SAGE_HUE_GOLD = 0xFFD700,
    SAGE_HUE_YELLOW = 0xFFFF00,
    SAGE_HUE_LIGHTYELLOW = 0xFFFFE0,
    SAGE_HUE_LEMONCHIFFON = 0xFFFACD,
    SAGE_HUE_LIGHTGOLDENRODYELLOW = 0xFAFAD2,
    SAGE_HUE_PAPAYAWHIP = 0xFFEFD5,
    SAGE_HUE_MOCCASIN = 0xFFE4B5,
    SAGE_HUE_PEACHPUFF = 0xFFDAB9,
    SAGE_HUE_PALEGOLDENROD = 0xEEE8AA,
    SAGE_HUE_KHAKI = 0xF0E68C,
    SAGE_HUE_DARKKHAKI = 0xBDB76B,

    /* Purple */
    SAGE_HUE_LAVENDER = 0xE6E6FA,
    SAGE_HUE_THISTLE = 0xD8BFD8,
    SAGE_HUE_PLUM = 0xDDA0DD,
    SAGE_HUE_VIOLET = 0xEE82EE,
    SAGE_HUE_ORCHID = 0xDA70D6,
    SAGE_HUE_FUCHSIA = 0xFF00FF,
    SAGE_HUE_MAGENTA = 0xFF00FF,
    SAGE_HUE_MEDIUMORCHID = 0xBA55D3,
    SAGE_HUE_MEDIUMPURPLE = 0x9370DB,
    SAGE_HUE_REBECCAPURPLE = 0x663399,
    SAGE_HUE_BLUEVIOLET = 0x8A2BE2,
    SAGE_HUE_DARKVIOLET = 0x9400D3,
    SAGE_HUE_DARKORCHID = 0x9932CC,
    SAGE_HUE_DARKMAGENTA = 0x8B008B,
    SAGE_HUE_PURPLE = 0x800080,
    SAGE_HUE_INDIGO = 0x4B0082,
    SAGE_HUE_SLATEBLUE = 0x6A5ACD,
    SAGE_HUE_DARKSLATEBLUE = 0x483D8B,
    SAGE_HUE_MEDIUMSLATEBLUE = 0x7B68EE,

    /* Green */
    SAGE_HUE_GREENYELLOW = 0xADFF2F,
    SAGE_HUE_CHARTREUSE = 0x7FFF00,
    SAGE_HUE_LAWNGREEN = 0x7CFC00,
    SAGE_HUE_LIME = 0x00FF00,
    SAGE_HUE_LIMEGREEN = 0x32CD32,
    SAGE_HUE_PALEGREEN = 0x98FB98,
    SAGE_HUE_LIGHTGREEN = 0x90EE90,
    SAGE_HUE_MEDIUMSPRINGGREEN = 0x00FA9A,
    SAGE_HUE_SPRINGGREEN = 0x00FF7F,
    SAGE_HUE_MEDIUMSEAGREEN = 0x3CB371,
    SAGE_HUE_SEAGREEN = 0x2E8B57,
    SAGE_HUE_FORESTGREEN = 0x228B22,
    SAGE_HUE_GREEN = 0x008000,
    SAGE_HUE_DARKGREEN = 0x006400,
    SAGE_HUE_YELLOWGREEN = 0x9ACD32,
    SAGE_HUE_OLIVEDRAB = 0x6B8E23,
    SAGE_HUE_OLIVE = 0x808000,
    SAGE_HUE_DARKOLIVEGREEN = 0x556B2F,
    SAGE_HUE_MEDIUMAQUAMARINE = 0x66CDAA,
    SAGE_HUE_DARKSEAGREEN = 0x8FBC8B,
    SAGE_HUE_LIGHTSEAGREEN = 0x20B2AA,
    SAGE_HUE_DARKCYAN = 0x008B8B,
    SAGE_HUE_TEAL = 0x008080,

    /* Blue */
    SAGE_HUE_AQUA = 0x00FFFF,
    SAGE_HUE_CYAN = 0x00FFFF,
    SAGE_HUE_LIGHTCYAN = 0xE0FFFF,
    SAGE_HUE_PALETURQUOISE = 0xAFEEEE,
    SAGE_HUE_AQUAMARINE = 0x7FFFD4,
    SAGE_HUE_TURQUOISE = 0x40E0D0,
    SAGE_HUE_MEDIUMTURQUOISE = 0x48D1CC,
    SAGE_HUE_DARKTURQUOISE = 0x00CED1,
    SAGE_HUE_CADETBLUE = 0x5F9EA0,
    SAGE_HUE_STEELBLUE = 0x4682B4,
    SAGE_HUE_LIGHTSTEELBLUE = 0xB0C4DE,
    SAGE_HUE_POWDERBLUE = 0xB0E0E6,
    SAGE_HUE_LIGHTBLUE = 0xADD8E6,
    SAGE_HUE_SKYBLUE = 0x87CEEB,
    SAGE_HUE_LIGHTSKYBLUE = 0x87CEFA,
    SAGE_HUE_DEEPSKYBLUE = 0x00BFFF,
    SAGE_HUE_DODGERBLUE = 0x1E90FF,
    SAGE_HUE_CORNFLOWERBLUE = 0x6495ED,
    SAGE_HUE_ROYALBLUE = 0x4169E1,
    SAGE_HUE_BLUE = 0x0000FF,
    SAGE_HUE_MEDIUMBLUE = 0x0000CD,
    SAGE_HUE_DARKBLUE = 0x00008B,
    SAGE_HUE_NAVY = 0x000080,
    SAGE_HUE_MIDNIGHTBLUE = 0x191970,
	        
	/* Brown */
    SAGE_HUE_CORNSILK = 0xFFF8DC,
    SAGE_HUE_BLANCHEDALMONDS = 0xFFEBCD,
    SAGE_HUE_BISQUE = 0xFFE4C4,
    SAGE_HUE_NAVAJOWHITE = 0xFFDEAD,
    SAGE_HUE_WHEAT = 0xF5DEB3,
    SAGE_HUE_BURLYWOOD = 0xDEB887,
    SAGE_HUE_TAN = 0xD2B48C,
    SAGE_HUE_ROSYBROWN = 0xBC8F8F,
    SAGE_HUE_SANDYBROWN = 0xF4A460,
    SAGE_HUE_GOLDENROD = 0xDAA520,
    SAGE_HUE_DARKGOLDENROD = 0xB8860B,
    SAGE_HUE_PERU = 0xCD853F,
    SAGE_HUE_CHOCOLATE = 0xD2691E,
    SAGE_HUE_SADDLEBROWN = 0x8B4513,
    SAGE_HUE_SIENNA = 0xA0522D,
    SAGE_HUE_BROWN = 0xA52A2A,
    SAGE_HUE_MAROON = 0x800000,
	    
    /* White */
    SAGE_HUE_WHITE = 0xFFFFFF,
    SAGE_HUE_SNOW = 0xFFFAFA,
    SAGE_HUE_HONEYDEW = 0xF0FFF0,
    SAGE_HUE_MINTCREAM = 0xF5FFFA,
    SAGE_HUE_AZURE = 0xF0FFFF,
    SAGE_HUE_ALICEBLUE = 0xF0F8FF,
    SAGE_HUE_GHOSTWHITE = 0xF8F8FF,
    SAGE_HUE_WHITESMOKE = 0xF5F5F5,
    SAGE_HUE_SEASHELL = 0xFFF5EE,
    SAGE_HUE_BEIGE = 0xF5F5DC,
    SAGE_HUE_OLDLACE = 0xFDF5E6,
    SAGE_HUE_FLORALWHITE = 0xFFFAF0,
    SAGE_HUE_IVORY = 0xFFFFF0,
    SAGE_HUE_ANTIQUEWHITE = 0xFAEBD7,
    SAGE_HUE_LINEN = 0xFAF0E6,
    SAGE_HUE_LAVENDERBLUSH = 0xFFF0F5,
    SAGE_HUE_MISTYROSE = 0xFFE4E1,
	    
    /* Gray */
    SAGE_HUE_GAINSBORO = 0xDCDCDC,
    SAGE_HUE_LIGHTGRAY = 0xD3D3D3,
    SAGE_HUE_SILVER = 0xC0C0C0,
    SAGE_HUE_DARKGRAY = 0xA9A9A9,
    SAGE_HUE_GRAY = 0x808080,
    SAGE_HUE_DIMGRAY = 0x696969,
    SAGE_HUE_LIGHTSLATEGRAY = 0x778899,
    SAGE_HUE_SLATEGRAY = 0x708090,
    SAGE_HUE_DARKSLATEGRAY = 0x2F4F4F,
    SAGE_HUE_SAGE_HUE_BLACK = 0x000000
};


typedef struct sage_colour_t sage_colour_t;

extern sage_colour_t *
sage_colour_new(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

extern sage_colour_t *
sage_colour_new_hue(enum sage_hue_t hue);

extern sage_colour_t *
sage_colour_copy(const sage_colour_t *src);

extern sage_colour_t *
sage_colour_free(sage_colour_t *ctx);

extern uint8_t
sage_colour_red(const sage_colour_t *ctx);

extern void
sage_colour_red_set(sage_colour_t *ctx, uint8_t red);

extern uint8_t
sage_colour_green(const sage_colour_t *ctx);

extern void
sage_colour_green_set(sage_colour_t *ctx, uint8_t green);

extern uint8_t
sage_colour_blue(const sage_colour_t *ctx);

extern void
sage_colour_blue_set(sage_colour_t *ctx, uint8_t blue);

extern uint8_t
sage_colour_alpha(const sage_colour_t *ctx);

extern void
sage_colour_alpha_set(sage_colour_t *ctx, uint8_t alpha);


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

extern void 
sage_vector_x_set(sage_vector_t *ctx, float x);

extern float 
sage_vector_y(const sage_vector_t *ctx);

extern void 
sage_vector_y_set(sage_vector_t *ctx, float y);

extern struct sage_point_t
sage_vector_point(const sage_vector_t *ctx);

extern float 
sage_vector_len(const sage_vector_t *ctx);

extern bool
sage_vector_visible(const sage_vector_t *ctx);

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


struct sage_viewport_t {
    struct sage_point_t point;
    struct sage_area_t area;
};


extern void
sage_screen_start(const char *title, struct sage_area_t res);

extern void
sage_screen_stop(void);

extern SAGE_HOT void*
sage_screen_brush(void);

extern SAGE_HOT void
sage_screen_clear(void);

extern SAGE_HOT struct sage_viewport_t *
sage_screen_viewport(void);

extern SAGE_HOT void
sage_screen_viewport_set(const struct sage_viewport_t *vp);

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


typedef struct sage_texture_t sage_texture_t;

extern SAGE_HOT sage_texture_t *
sage_texture_new(const char *path);

extern sage_texture_t*
sage_texture_free(sage_texture_t *tex);

extern SAGE_HOT struct sage_area_t 
sage_texture_area(const sage_texture_t *tex);

extern void
sage_texture_clip(sage_texture_t *ctx, 
                  struct sage_point_t nw,
                  struct sage_area_t clip);
extern void
sage_texture_scale(sage_texture_t *ctx, struct sage_area_t proj);

extern void
sage_texture_reset(sage_texture_t *ctx);

extern SAGE_HOT void
sage_texture_draw(sage_texture_t *ctx, struct sage_point_t dst);

/*
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
*/


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


typedef size_t sage_id_t;


extern void 
sage_sprite_factory_start(void);

extern void 
sage_sprite_factory_stop(void);

extern void 
sage_sprite_factory_register(const sage_sprite_t *spr);

extern sage_sprite_t *
sage_sprite_factory_spawn(sage_id_t idx);


typedef struct sage_entity_t sage_entity_t;

typedef void (sage_entity_f)(sage_entity_t *ctx);

extern sage_entity_t *
sage_entity_new(sage_id_t sid,
                sage_entity_f *upd,
                sage_entity_f *free,
                sage_entity_f *draw);

extern sage_entity_t *
sage_entity_copy(const sage_entity_t *src);

extern sage_entity_t *
sage_entity_free(sage_entity_t *ctx);

extern sage_vector_t *
sage_entity_vector(const sage_entity_t *ctx);

extern void 
sage_entity_vector_set(sage_entity_t *ctx, const sage_vector_t *vec);

extern struct sage_frame_t 
sage_entity_frame(const sage_entity_t *ctx);

extern void 
sage_entity_frame_set(sage_entity_t *ctx, struct sage_frame_t frm);

extern void 
sage_entity_move(sage_entity_t *ctx, const sage_vector_t *vel);

extern void 
sage_entity_update(sage_entity_t *ctx);

extern void 
sage_entity_draw(sage_entity_t *ctx);


extern void 
sage_entity_factory_start(void);

extern void 
sage_entity_factory_stop(void);

extern void 
sage_entity_factory_register(const sage_entity_t *ent);

extern sage_entity_t *
sage_entity_factory_spawn(sage_id_t idx);


extern void sage_arena_start(void);

extern void sage_arena_stop(void);

extern sage_id_t sage_arena_push(const sage_id_t id);

extern void sage_arena_pop(sage_id_t id);

extern void sage_arena_update(void);

extern void sage_arena_draw(void);


#if defined __cplusplus
    } /* extern "C" */
#endif

#endif /* SCHEME_ASSISTED_GAME_ENGINE */

