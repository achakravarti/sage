
/** HEAP MANAGER **/

#define sage_heap_start()

#define sage_heap_stop()

extern void *sage_heap_new(size_t sz);

extern void *
sage_heap_resize(void   *ptr,
                 size_t sz);

extern void sage_heap_free(void **bfr);


