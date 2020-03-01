#include "core.h"


extern void *sage_heap_new(size_t sz)
{
    void *bfr;

    sage_assert (sz);
    sage_require (bfr = malloc (sz));

    return bfr;
}


extern void *
sage_heap_resize(void   *ptr,
                 size_t sz)
{
    sage_assert (ptr && sz);

    void *bfr;
    sage_require (bfr = realloc(ptr, sz));

    return bfr;

}


extern void sage_heap_free(void **bfr)
{
    if (sage_likely (bfr && *bfr)) {
        free (*bfr);
        *bfr = NULL;
    }
}

