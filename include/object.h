#ifndef SAGE_OBJECT_H
#define SAGE_OBJECT_H


#include "primitives.h"
#include "id-map.h"


struct sage_object_vtable_t {
    void *(*copy)(const void *ctx);
    void  (*free)(void *ctx);
};


typedef struct __sage_object_t *sage_object_t;


extern sage_object_t sage_object_new(const sage_id_t oid, const sage_id_t iid, 
    void *cdata, const struct sage_object_vtable_t *vt);

extern sage_object_t sage_object_copy(const sage_object_t ctx);

extern void sage_object_free(sage_object_t *ctx);

extern sage_id_t sage_object_oid(const sage_object_t ctx);

extern sage_id_t sage_object_iid(const sage_object_t ctx);

extern void sage_object_iid_set(sage_object_t *ctx, const sage_id_t iid);

extern const void *sage_object_cdata(const sage_object_t ctx);

extern void *sage_object_cdata_rw(sage_object_t *ctx);


#endif /* SAGE_OBJECT_H */

