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


#include "../include/compiler.h"
#include "../include/exception.h"
#include "../include/heap.h"
#include "../include/color.h"
#include "../include/vector.h"
#include "../include/screen.h"
#include "../include/mouse.h"
#include "../include/keyboard.h"
#include "../include/event.h"
#include "../include/id-map.h"
#include "../include/texture.h"
#include "../include/sprite.h"
#include "../include/payload.h"
#include "../include/entity.h"
#include "../include/scene.h"
#include "../include/stage.h"
#include "../include/arena.h"
#include "../include/game.h"

#if defined __cplusplus
    } /* extern "C" */
#endif

#endif /* SCHEME_ASSISTED_GAME_ENGINE */

