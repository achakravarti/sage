
#ifndef SAGE_HEAP_H
#define SAGE_HEAP_H

#include "primitives.h"

#define sage_heap_init()

#define sage_heap_exit()

extern void *sage_heap_new(size_t sz);

extern void *sage_heap_resize(void *ptr, size_t sz);

extern void sage_heap_free(void **bfr);

#endif /* SAGE_HEAP_H */

