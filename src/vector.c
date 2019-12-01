#include <float.h>
#include <math.h>
#include "sage.h"


struct sage_vector_t {
    float x;
    float y;
};


static inline bool
float_lt(float lhs, float rhs)
{
    double lfabs = fabs (lhs), rfabs = fabs (rhs);
    return (rhs - lhs) > ((lfabs < rfabs ? rfabs : lfabs) * FLT_EPSILON);
}


static inline bool
float_eq(float lhs, float rhs)
{
    double lfabs = fabs (lhs), rfabs = fabs (rhs);
    return (lhs - rhs) <= ((lfabs < rfabs ? rfabs : lfabs) * FLT_EPSILON);
}


extern sage_vector_t *
sage_vector_new(float x, float y)
{
    sage_vector_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->x = x;
    ctx->y = y;

    return ctx;
}

extern sage_vector_t *
sage_vector_new_zero(void)
{
    return sage_vector_new (0.0f, 0.0f);
}


extern sage_vector_t *
sage_vector_copy(const sage_vector_t *src)
{
    return sage_vector_new (src->x, src->y);
}


extern sage_vector_t *
sage_vector_free(sage_vector_t *ctx)
{
    free (ctx);
    return NULL;
}


extern float 
sage_vector_x(const sage_vector_t *ctx)
{
    return ctx->x;
}


extern float 
sage_vector_y(const sage_vector_t *ctx)
{
    return ctx->y;
}


extern void 
sage_vector_x_set(sage_vector_t *ctx, float x)
{
    ctx->x = x;
}


extern void 
sage_vector_y_set(sage_vector_t *ctx, float y)
{
    ctx->y = y;
}


extern struct sage_point_t
sage_vector_point(const sage_vector_t *ctx)
{
    struct sage_point_t pt = {.x = (int16_t) ctx->x, .y = (int16_t) ctx->y};
    return pt;
}

extern float 
sage_vector_len(const sage_vector_t *ctx)
{
    double x = (double) ctx->x, y = (double) ctx->y;
    return (float) sqrt ((x * x) + (y * y));
}


extern bool
sage_vector_visible(const sage_vector_t *ctx)
{
    return !float_lt (ctx->x, 0.0f) && !float_lt (ctx->y, 0.0f);
}


extern void 
sage_vector_norm(sage_vector_t *ctx)
{
    float len = sage_vector_len (ctx);
    sage_require (!float_eq (len, 0.0f));

    sage_vector_mul (ctx, (1.0 / len));
}


extern enum sage_compare_t 
sage_vector_cmp(const sage_vector_t *ctx, const sage_vector_t *rhs)
{
    float clen = sage_vector_len (ctx), rlen = sage_vector_len (rhs);

    if (float_eq (clen, rlen)) return SAGE_COMPARE_EQ;
    else if (float_lt (clen, rlen)) return SAGE_COMPARE_LT;
    else return SAGE_COMPARE_GT;
}


extern void 
sage_vector_add(sage_vector_t *ctx, const sage_vector_t *add)
{
    ctx->x += add->x;
    ctx->y += add->y;
}


extern void 
sage_vector_sub(sage_vector_t *ctx, const sage_vector_t *sub)
{
    ctx->x -= sub->x;
    ctx->y -= sub->y;
}


extern void 
sage_vector_mul(sage_vector_t *ctx, float mul)
{
    ctx->x *= mul;
    ctx->y *= mul;
}


extern void 
sage_vector_div(sage_vector_t *ctx, const float div)
{
    sage_require (!float_eq (div, 0.0f));
    ctx->x /= div;
    ctx->y /= div;
}
