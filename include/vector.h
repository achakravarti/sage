#ifndef SAGE_VECTOR_H
#define SAGE_VECTOR_H

#if 0
#include "primitives.h" /* bool */
#include "exception.h" /* sage_assert */


/** VECTOR **/

typedef struct sage_vector_t sage_vector_t;

extern sage_vector_t *
sage_vector_new(float x, float y);

extern sage_vector_t *
sage_vector_new_zero(void);

inline sage_vector_t *
sage_vector_move(sage_vector_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

inline const sage_vector_t *
sage_vector_link(const sage_vector_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

extern sage_vector_t *
sage_vector_copy(const sage_vector_t *src);

extern void
sage_vector_free(sage_vector_t **ctx);

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

extern bool
sage_vector_lt(const sage_vector_t *ctx, const sage_vector_t *rhs);

extern bool
sage_vector_lteq(const sage_vector_t *ctx, const sage_vector_t *rhs);

extern bool
sage_vector_eq(const sage_vector_t *ctx, const sage_vector_t *rhs);

extern bool
sage_vector_gteq(const sage_vector_t *ctx, const sage_vector_t *rhs);

extern bool
sage_vector_gt(const sage_vector_t *ctx, const sage_vector_t *rhs);

extern void 
sage_vector_add(sage_vector_t *ctx, const sage_vector_t *add);

extern void 
sage_vector_sub(sage_vector_t *ctx, const sage_vector_t *sub);

extern void 
sage_vector_mul(sage_vector_t *ctx, float mul);

extern void 
sage_vector_div(sage_vector_t *ctx, const float div);
#endif

#endif /* SAGE_VECTOR_H */

