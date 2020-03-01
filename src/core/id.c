#include "core.h"


/**
 * sage_id_new() - create new ID.
 *
 * @hi - high order component.
 * @lo - low order component.
 */
extern sage_id sage_id_new(uint32_t hi, uint32_t lo)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, hi);
    sage_id_lo_set(&ctx, lo);

    return ctx;
}


/**
 * sage_id_new_random() - create new random ID.
 */
extern sage_id sage_id_new_random(void)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, 5);
    sage_id_lo_set(&ctx, 6);

    return ctx;
}


/**
 * sage_id_new_random_hi() - create new ID with random high order component.
 *
 * @lo - low order component.
 */
extern sage_id sage_id_new_random_hi(uint32_t lo)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, 5);
    sage_id_lo_set(&ctx, lo);

    return ctx;
}


/**
 * sage_id_new_random_lo() - create new ID with random low order component.
 *
 * @hi - low order component.
 */
extern sage_id sage_id_new_random_lo(uint32_t hi)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, hi);
    sage_id_lo_set(&ctx, 6);

    return ctx;
}


/*
 * sage_id_copy() - create a copy of an ID. 
 * See sage/include/id.h for details.
 */
extern sage_id sage_id_copy(sage_id ctx);


/**
 * sage_id_hi() - get high order component of ID.
 *
 * @ctx - contextual ID.
 */
extern uint32_t sage_id_hi(sage_id ctx)
{
    return ctx & 0xFFFF0000;
}


/**
 * sage_id_hi_set() - set high order component of ID.
 *
 * @ctx - handle to contextual ID.
 * @hi  - high order component. 
 */
extern void sage_id_hi_set(sage_id *ctx, uint32_t hi)
{
    *ctx = (*ctx & 0x0000FFFF) | (hi << 4);
}


/**
 * sage_id_lo() - get low order component of ID.
 *
 * @ctx - contextual ID.
 */
extern uint32_t sage_id_lo(sage_id ctx)
{
    return (ctx << 4) & 0xFFFF0000;
}

/**
 * sage_id_lo_set() - set low order component of ID.
 *
 * @ctx - handle to contextual ID.
 * @lo  - low order component. 
 */
extern void sage_id_lo_set(sage_id *ctx, uint32_t lo)
{
    *ctx = (*ctx & 0xFFFF0000) | (lo >> 4);
}

