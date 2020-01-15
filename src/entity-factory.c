#include "sage.h"


#define MAP_BUCKETS ((size_t) 16)


static thread_local sage_id_map_t *map = NULL;


static inline void *map_copy(const void *ent)
{
    return sage_entity_copy ((const sage_entity_t *) ent);
}


static inline void
map_free(void *ent)
{
    (void) sage_entity_free ((sage_entity_t *) ent);
}


extern void 
sage_entity_factory_start(void)
{
    if (sage_likely (!map)) {
        map = sage_id_map_new (MAP_BUCKETS, sage_entity_size (), map_copy, 
            map_free);
    }
}


extern void 
sage_entity_factory_stop(void)
{
    map = sage_id_map_free (map);
}


extern void 
sage_entity_factory_register(const sage_entity_t *ent)
{
    sage_id_map_value_set (map, sage_entity_id (ent), ent);
}


extern sage_entity_t *
sage_entity_factory_spawn(sage_id_t id)
{
    return (sage_entity_t *) sage_id_map_value (map, id);
}

