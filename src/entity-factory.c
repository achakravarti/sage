#include "./sage.h"


#define MAP_BUCKETS ((size_t) 16)


static thread_local sage_id_map_t *map = NULL;


// TODO: implement map_copy_deep() and map_copy()
static inline void *
map_copy(const void *ent)
{
    return sage_entity_copy((const sage_entity_t *) ent);
}


static inline void 
map_free(void **ent)
{
    (void) sage_entity_free ((sage_entity_t **) ent);
}


extern void 
sage_entity_factory_start(void)
{
    if (sage_likely (!map)) {
        struct sage_id_map_vtable_t vt = {.copy = map_copy, .free = map_free};
        map = sage_id_map_new (MAP_BUCKETS, sage_entity_size (), &vt);
    }
}


extern void 
sage_entity_factory_stop(void)
{
    map = sage_id_map_free (map);
}


extern void 
sage_entity_factory_register(sage_id_t cls,
                             sage_id_t texid)
                             
{
    struct sage_frame_t frm = {.r = 1, .c = 1};
    sage_id_map_value_set(map, cls, sage_entity_new_default(cls, texid, frm));
}


extern sage_entity_t *
sage_entity_factory_spawn(sage_id_t id)
{
    return (sage_entity_t *) sage_id_map_value (map, id);
}

