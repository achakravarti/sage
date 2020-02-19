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
 * This is the sage/src/sage.h header file; it declares the API of the SAGE
 * Library, and is its only header file.
 ******************************************************************************/


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


struct sage_point_t {
    float x;
    float y;
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


#include "compiler.h"
#include "exception.h"
#include "heap.h"
#include "color.h"
#include "vector.h"
#include "screen.h"
#include "mouse.h"
#include "keyboard.h"
#include "event.h"
#include "id-map.h"
#include "texture.h"
#include "sprite.h"
#include "payload.h"
#include "entity.h"
#include "scene.h"
#include "stage.h"
#include "arena.h"
#include "game.h"

#if defined __cplusplus
    } /* extern "C" */
#endif

#endif /* SCHEME_ASSISTED_GAME_ENGINE */

