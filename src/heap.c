#include "sage.h"


extern void *sage_heap_new(size_t sz)
{
    void *bfr;

    sage_assert (sz);
    sage_require (bfr = malloc (sz));

    return bfr;
}


extern void sage_heap_free(void **bfr)
{
    if (sage_likely (bfr && *bfr)) {
        free (*bfr);
        *bfr = NULL;
    }
}

