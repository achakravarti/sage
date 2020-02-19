
#ifndef SAGE_ID_MAP_H
#define SAGE_ID_MAP_H

#include <stddef.h> /* for size_t */

typedef size_t sage_id_t;


struct sage_id_map_vtable_t {
    void *(*copy) (const void *ctx);
    void (*free) (void **ctx);
};


typedef struct sage_id_map_t sage_id_map_t;

/*typedef void *(sage_id_map_copy_f)(const void *ctx);

typedef void (sage_id_map_free_f)(void *ctx);*/


/*extern sage_id_map_t *sage_id_map_new(size_t buck, size_t sz, 
    sage_id_map_copy_f *copy, sage_id_map_free_f *free);*/


extern sage_id_map_t *
sage_id_map_new(size_t buck,
                size_t sz,
                struct sage_id_map_vtable_t *vt);



extern sage_id_map_t *
sage_id_map_free(sage_id_map_t *ctx);

extern size_t
sage_id_map_hash(const sage_id_map_t *ctx, sage_id_t key);

extern void *
sage_id_map_value(sage_id_map_t *ctx, sage_id_t key);

extern void
sage_id_map_value_set(sage_id_map_t *ctx, sage_id_t key, void *val);

#endif /* SAGE_ID_MAP_H */

