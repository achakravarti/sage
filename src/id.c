#include "../include/id.h"


extern sage_id sage_id_new(uint32_t hi, uint32_t lo)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, hi);
    sage_id_lo_set(&ctx, lo);

    return ctx;
}

extern sage_id sage_id_new_random(void)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, 5);
    sage_id_lo_set(&ctx, 6);

    return ctx;
}


extern sage_id sage_id_new_random_hi(uint32_t lo)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, 5);
    sage_id_lo_set(&ctx, lo);

    return ctx;
}


extern sage_id sage_id_new_random_lo(uint32_t hi)
{
    sage_id ctx;
    sage_id_hi_set(&ctx, hi);
    sage_id_lo_set(&ctx, 6);

    return ctx;
}


extern sage_id sage_id_copy(sage_id ctx);


extern uint32_t sage_id_hi(sage_id ctx)
{
    return ctx & 0xFFFF0000;
}

extern void sage_id_hi_set(sage_id *ctx, uint32_t hi)
{
    *ctx = (*ctx & 0x0000FFFF) | (hi << 4);
}


extern uint32_t sage_id_lo(sage_id ctx)
{
    return (ctx << 4) & 0xFFFF0000;
}

extern void sage_id_lo_set(sage_id *ctx, uint32_t lo)
{
    *ctx = (*ctx & 0xFFFF0000) | (lo >> 4);
}

