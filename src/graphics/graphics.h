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
 * This is the sage/src/graphics.h header file; it declares the graphics
 * interface of the SAGE Library.
 ******************************************************************************/


#ifndef SCHEME_ASSISTED_GAME_ENGINE_GRAPHICS_HEADER
#define SCHEME_ASSISTED_GAME_ENGINE_GRAPHICS_HEADER


#include "../core/core.h"


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
    SAGE_HUE_BLACK = 0x000000
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
sage_screen_clear(const sage_colour_t *col);

extern SAGE_HOT struct sage_viewport_t *
sage_screen_viewport(void);

extern SAGE_HOT void
sage_screen_viewport_set(const struct sage_viewport_t *vp);

extern SAGE_HOT void sage_screen_render(void);




/******************************************************************************
 * TEXTURE
 */


typedef sage_object sage_texture;


/*
 * sage_texture_new() - create new texture.
 * See sage/src/graphics/texture.c for details.
 */
extern sage_texture *sage_texture_new(sage_id texid, const char *path);


/*
 * sage_texture_copy() - copy a texture.
 */
inline sage_texture *sage_texture_copy(const sage_texture *ctx)
{
    sage_assert (ctx);
    return sage_object_copy(ctx);
}


/*
 * sage_texture_free() - release texture from heap.
 */
inline void sage_texture_free(sage_texture **ctx)
{
    sage_object_free(ctx);
}


/*
 * sage_texture_id() - get ID of texture.
 */
inline sage_id sage_texture_id(const sage_texture *ctx)
{
    sage_assert (ctx);
    return sage_object_id(ctx);
}


/*
 * sage_texture_area() - get area of texture.
 * See sage/src/graphics/texture.c for details.
 */
extern struct sage_area_t sage_texture_area(const sage_texture *ctx);


/*
 * sage_texture_clip() - clip texture to an area.
 * See sage/src/graphics/texture.c for details.
 */
extern void sage_texture_clip(sage_texture **ctx, struct sage_point_t nw, 
        struct sage_area_t clip);


/*
 * sage_texture_scale() - scale texture to a projection.
 * See sage/src/graphics/texture.c for details.
 */
extern void sage_texture_scale(sage_texture **ctx, struct sage_area_t proj);


/*
 * sage_texture_reset() - reset texture to original state.
 * See sage/src/graphics/texture.c for details.
 */
extern void sage_texture_reset(sage_texture **ctx);


/*
 * sage_texture_draw() - draw texture to screen.
 * See sage/src/graphics/texture.c for details.
 */
extern void sage_texture_draw(const sage_texture *ctx, struct sage_point_t dst);


extern void sage_texture_factory_init(void);

extern void sage_texture_factory_exit(void);

extern void sage_texture_factory_register(sage_id id, const char *path);

extern sage_texture *sage_texture_factory_clone(sage_id id);

struct sage_frame_t {
    uint16_t r;
    uint16_t c;
};

typedef sage_object sage_sprite;

extern sage_sprite *sage_sprite_new(sage_id texid, struct sage_frame_t tot);

inline sage_sprite *sage_sprite_copy(const sage_sprite *ctx)
{
    return sage_object_copy(ctx);
}

inline void sage_sprite_free(sage_sprite **ctx)
{
    sage_object_free(ctx);
}

inline sage_id sage_sprite_id(const sage_sprite *ctx)
{
    sage_assert (ctx);
    return sage_object_id(ctx);
}

extern struct sage_area_t sage_sprite_area(const sage_sprite *ctx);

extern struct sage_area_t sage_sprite_area_frame(const sage_sprite *ctx);

extern void sage_sprite_frame(sage_sprite **ctx, struct sage_frame_t frm);

extern struct sage_frame_t sage_sprite_frames(const sage_sprite *ctx);

extern void sage_sprite_clip(sage_sprite **ctx, struct sage_point_t nw,
        struct sage_area_t clip);

extern void sage_sprite_scale(sage_sprite **ctx, struct sage_area_t proj);

extern void sage_sprite_reset(sage_sprite **ctx);

extern void sage_sprite_draw(const sage_sprite *ctx, struct sage_point_t dst);


#endif /* SCHEME_ASSISTED_GAME_ENGINE_GRAPHICS_HEADER */


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 ******************************************************************************/

