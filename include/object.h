#ifndef SAGE_OBJECT_H
#define SAGE_OBJECT_H

#if 0
#include "primitives.h"
#include "id.h"


struct sage_object_vtable {
    void *(*copy)(const void *ctx);
    void (*free)(void **ctx);
};


typedef struct sage_object sage_object;

extern sage_object *sage_object_new(sage_id id, void *cdata, 
        const struct sage_object_vtable *vt);

extern sage_object *sage_object_copy(const sage_object *ctx);

extern void sage_object_free(sage_object **ctx);

extern size_t sage_object_size(void);

extern sage_id sage_object_id(const sage_object *ctx);

extern void sage_object_id_set(sage_object **ctx, sage_id id);

extern const void *sage_object_cdata(const sage_object *ctx);

extern void *sage_object_cdata_mutate(sage_object **ctx);
#endif


#endif /* SAGE_OBJECT_H */

