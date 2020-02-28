#ifndef SAGE_ID_H
#define SAGE_ID_H

#if 0
#include "primitives.h"


/**
 * sage_id - unique ID with high and low order components.
 */
typedef uint64_t sage_id;


/*
 * sage_id_new() - create new ID.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new(uint32_t hi, uint32_t lo);


/*
 * sage_id_new_random() - create new random ID.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new_random(void);


/*
 * sage_id_new_random_hi() - create new ID with random high order component.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new_random_hi(uint32_t lo);


/*
 * sage_id_new_random_lo() - create new ID with random low order component.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new_random_lo(uint32_t hi);


/**
 * sage_id_copy() - create a copy of an ID.
 */
inline sage_id sage_id_copy(sage_id ctx)
{
    return ctx;
}


/**
 * sage_id_free() - destroy an existing ID.
 */
#define /* void */ sage_id_free(/* sage_id * */ id)


/*
 * sage_id_hi() - get high order component of ID.
 * See sage/src/id.c for details.
 */
extern uint32_t sage_id_hi(sage_id ctx);


/*
 * sage_id_hi_set() - set high order component of ID.
 * See sage/src/id.c for details.
 */
extern void sage_id_hi_set(sage_id *ctx, uint32_t hi);


/*
 * sage_id_lo() - get low order component of ID.
 * See sage/src/id.c for details.
 */
extern uint32_t sage_id_lo(sage_id ctx);


/*
 * sage_id_lo_set() - set low order component of ID.
 * See sage/src/id.c for details.
 */
extern void sage_id_lo_set(sage_id *ctx, uint32_t lo);
#endif

#endif /* SAGE_ID_H */

