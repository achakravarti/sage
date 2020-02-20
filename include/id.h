#ifndef SAGE_ID_H
#define SAGE_ID_H


#include "primitives.h"


typedef uint64_t sage_id;


extern sage_id sage_id_new(uint32_t hi, uint32_t lo);

extern sage_id sage_id_new_random(void);

extern sage_id sage_id_new_random_hi(uint32_t lo);

extern sage_id sage_id_new_random_lo(uint32_t hi);

inline sage_id sage_id_copy(sage_id ctx)
{
    return ctx;
}

#define /* void */ sage_id_free(/* sage_id * */ id)

extern uint32_t sage_id_hi(sage_id ctx);

extern void sage_id_hi_set(sage_id *ctx, uint32_t hi);

extern uint32_t sage_id_lo(sage_id ctx);

extern void sage_id_lo_set(sage_id *ctx, uint32_t lo);

#endif /* SAGE_ID_H */
