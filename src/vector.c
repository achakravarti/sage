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
 * This is the sage/src/vector.c source file; it implements the vector API of 
 * the SAGE Library.
 ******************************************************************************/


/*
 * The sage/include/api.h header file contains the declaration of the API of the
 * SAGE Library. The standard float.h and math.h header files are required for
 * floating point computations.
 */
#include <float.h>
#include <math.h>
#include "utility.h"


/*
 * The sage_vector_t struct was forward declared in the sage/src/sage.h header
 * file. Here we're defining the struct with its attributes. This struct holds
 * the x and y coordinates that represent a vector.
 */
struct sage_vector_t {
    float x; /* x coordinate */
    float y; /* y coordinate */
};


/*
 * The float_lt() helper function checks whether a given floating point number
 * is less than another one. The LHS floating point number is passed through the
 * lhs parameter, and the RHS floating point number is passed through the rhs
 * parameter. This function uses Donald Knuth's algorithm; see the answer posted 
 * by user mch at https://stackoverflow.com/questions/17333.
 */
static inline bool
float_lt(float lhs, 
         float rhs)
{
    double lfabs = fabs(lhs), rfabs = fabs(rhs);
    return (rhs - lhs) > ((lfabs < rfabs ? rfabs : lfabs) * FLT_EPSILON);
}


/*
 * The float_eq() helper function checks whether a given floating point number
 * is equal to another. Similar to the float_lt() helper function above, this
 * function receives the LHS floating point number through the lhs parameter and
 * the RHS floating point number through the rhs parameter. This function also
 * uses Donald Knuth's algorithm as answered by user mch to the question posted
 * at https://stackoverflow.com/questions/17333.
 */
static inline bool
float_eq(float lhs, 
         float rhs)
{
    double lfabs = fabs(lhs), rfabs = fabs(rhs);
    return (lhs - rhs) <= ((lfabs < rfabs ? rfabs : lfabs) * FLT_EPSILON);
}


/*
 * The sage_vector_new() interface function creates a new vector instance from a
 * given set of coordinates. We allocate memory in the heap for the new instance
 * and set its fields appropriately.
 */
extern sage_vector_t *
sage_vector_new(float x, 
                float y)
{
    sage_vector_t *ctx = sage_heap_new(sizeof *ctx);
    ctx->x = x;
    ctx->y = y;

    return ctx;
}


/*
 * The sage_vector_new_zero() interface function creates a new vector instance
 * centred at the origin. We do so by invoking sage_vector_new() with the origin
 * coordinates.
 */
extern sage_vector_t *
sage_vector_new_zero(void)
{
    return sage_vector_new(0.0f, 0.0f);
}


extern inline sage_vector_t *
sage_vector_move(sage_vector_t *ctx);


extern inline const sage_vector_t *
sage_vector_link(const sage_vector_t *ctx);


/*
 * The sage_vector_copy() interface function creates a deep copy of a vector
 * instance. We use sage_vector_new() to force the creation of a new instance
 * with the same attributes as the instance being copied.
 */
extern sage_vector_t *
sage_vector_copy(const sage_vector_t *src)
{
    sage_assert (src);
    return sage_vector_new(src->x, src->y);
}


/*
 * The sage_vector_free() interface function releases a vector instance from
 * heap memory. We simply make a call to sage_vector_free() in order to release
 * the instance. We don't make any checks on the instance since it's safe to
 * call sage_heap_free() even with null pointers.
 */
extern void
sage_vector_free(sage_vector_t **ctx)
{
    sage_heap_free((void **) ctx);
}


/*
 * The sage_vector_x() interface function gets the x coordinate of a given
 * vector instance. The x coordinate is held in the x attribute of the vector.
 */
extern float 
sage_vector_x(const sage_vector_t *ctx)
{
    sage_assert (ctx);
    return ctx->x;
}


/*
 * The sage_vector_x_set() interface function sets the x coordinate of a given
 * vector instance. The x coordinate is held in the x attribute of the vector.
 */
extern void 
sage_vector_x_set(sage_vector_t *ctx, 
                  float         x)
{
    sage_assert (ctx);
    ctx->x = x;
}


/*
 * The sage_vector_y() interface function gets the y coordinate of a given
 * vector instance. The y coordinate is held in the y attribute of the vector.
 */
extern float 
sage_vector_y(const sage_vector_t *ctx)
{
    sage_assert (ctx);
    return ctx->y;
}


/*
 * The sage_vector_y_set() interface function sets the y coordinate of a given
 * vector instance. The y coordinate is held in the y attribute of the vector.
 */
extern void 
sage_vector_y_set(sage_vector_t *ctx, 
                  float         y)
{
    sage_assert (ctx);
    ctx->y = y;
}


/*
 * The sage_vector_point() inteface function gets the coordinates of a given
 * vector instance represented as a point structure. We choose to return the
 * point structure by value instead of reference since it's only 64 bits wide.
 */
extern struct sage_point_t
sage_vector_point(const sage_vector_t *ctx)
{
    sage_assert (ctx);
    struct sage_point_t pt = {.x = ctx->x, .y = ctx->y};

    return pt;
}


/*
 * The sage_vector_len() interface function gets the length of a given vector
 * instance. The length of a vector is computed as the sum of the squares of its
 * coordinates.
 */
extern float 
sage_vector_len(const sage_vector_t *ctx)
{
    double x = (double) ctx->x, y = (double) ctx->y;
    return (float) sqrt((x * x) + (y * y));
}


/*
 * The sage_vector_visible() interface function determines whether a given
 * vector instance is visible on the screen. A vector is considered to be
 * visible if both its x and y coordinates are positive.
 */
extern bool
sage_vector_visible(const sage_vector_t *ctx)
{
    return !float_lt(ctx->x, 0.0f) && !float_lt(ctx->y, 0.0f);
}


/*
 * The sage_vector_norm() interface function normalises a given vector instance.
 * A vector is normalised by multiplying it with the inverse of its length. Of
 * course, we need to ensure that we are not dividing by a zero length, and this
 * check is delegated to the sage_vector_div() interface function.
 */
extern void 
sage_vector_norm(sage_vector_t *ctx)
{
    sage_vector_div(ctx, sage_vector_len(ctx));
}


/*
 * The sage_vector_cmp() interface function compares two vector instances. The
 * vectors are compared based on their length.
 */
extern enum sage_compare_t 
sage_vector_cmp(const sage_vector_t *ctx, 
                const sage_vector_t *rhs)
{
    sage_assert (ctx && rhs);
    float clen = sage_vector_len(ctx), rlen = sage_vector_len(rhs);

    if (float_eq(clen, rlen)) 
        return SAGE_COMPARE_EQ;
    else if (float_lt(clen, rlen))
        return SAGE_COMPARE_LT;
    else 
        return SAGE_COMPARE_GT;
}


/*
 * The sage_vector_lt() interface function compares whether a given vector is
 * less than another one. We make the comparison through the sage_vector_cmp()
 * interface function.
 */
extern bool
sage_vector_lt(const sage_vector_t *ctx, 
               const sage_vector_t *rhs)
{
    sage_assert (ctx && rhs);
    return sage_vector_cmp(ctx, rhs) == SAGE_COMPARE_LT;
}


/*
 * The sage_vector_lteq() interface function compares whether a given vector is
 * less than or equal to another one. We make the comparison through the
 * sage_vector_cmp() interface function.
 */
extern bool
sage_vector_lteq(const sage_vector_t *ctx, 
                 const sage_vector_t *rhs)
{
    sage_assert (ctx && rhs);
    return !sage_vector_gt(ctx, rhs);
}


/*
 * The sage_vector_eq() interface function compares whether a given vector is
 * equal to another one. We make the comparison through the sage_vector_cmp()
 * interface function.
 */
extern bool
sage_vector_eq(const sage_vector_t *ctx, 
               const sage_vector_t *rhs)
{
    sage_assert (ctx && rhs);
    return sage_vector_cmp(ctx, rhs) == SAGE_COMPARE_EQ;
}


/*
 * The sage_vector_gteq() interface function compares whether a given vector is
 * greater than or equal to another one. We make the comparison through the
 * sage_vector_cmp() interface function.
 */
extern bool
sage_vector_gteq(const sage_vector_t *ctx, 
                 const sage_vector_t *rhs)
{
    sage_assert (ctx && rhs);
    return !sage_vector_lt(ctx, rhs);
}


/*
 * The sage_vector_gt() interface function compares whether a given vector is
 * greater than another one. We make the comparison by invoking the
 * sage_vector_cmp() interface function.
 */
extern bool
sage_vector_gt(const sage_vector_t *ctx, 
               const sage_vector_t *rhs)
{
    sage_assert (ctx && rhs);
    return sage_vector_cmp(ctx, rhs) == SAGE_COMPARE_GT;
}


/*
 * The sage_vector_add() interface function adds a given vector to another one.
 * We add two vectors by adding their individual coordinates.
 */
extern void 
sage_vector_add(sage_vector_t       *ctx, 
                const sage_vector_t *add)
{
    sage_assert (ctx && add);
    ctx->x += add->x;
    ctx->y += add->y;
}


/*
 * The sage_vector_sub() interface function subtracts a given vector from
 * another one. We subtract two vectors by subtracting their individual
 * coordinates.
 */
extern void 
sage_vector_sub(sage_vector_t       *ctx, 
                const sage_vector_t *sub)
{
    sage_assert (ctx && sub);
    ctx->x -= sub->x;
    ctx->y -= sub->y;
}


/*
 * The sage_vector_mul() interface function multiplies a given vector by a
 * scalar. We multiply a vector with a scalar my multiplying each of its
 * coordinates with the scalar.
 */
extern void 
sage_vector_mul(sage_vector_t *ctx, 
                float         mul)
{
    sage_assert (ctx);
    ctx->x *= mul;
    ctx->y *= mul;
}


/*
 * The sage_vector_div() interface function divides a given vector by a scalar.
 * We divide a vector by a saclar by dividing each of its coordiates with the
 * scalar value. We make a requirements check that we are not dividing through
 * by zero.
 */
extern void 
sage_vector_div(sage_vector_t *ctx, 
                const float   div)
{
    sage_require (!float_eq(div, 0.0f));
    ctx->x /= div;
    ctx->y /= div;
}


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 *
 ******************************************************************************/

