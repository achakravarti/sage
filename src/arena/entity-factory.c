#include "arena.h"


#define MAP_BUCKETS ((size_t) 16)

static thread_local sage_object_map *map = NULL;


extern void sage_entity_factory_init(void)
{
    if (sage_likely (!map))
        map = sage_object_map_new(MAP_BUCKETS);
}


extern void sage_entity_factory_exit(void)
{
    sage_object_map_free(&map);
}


extern void sage_entity_factory_register(const sage_entity *ent)
{
    sage_assert (ent);
    sage_object_map_value_set(map, sage_entity_id(ent), sage_entity_copy(ent));
}


extern sage_entity *sage_entity_factory_clone(sage_id entid)
{
    sage_assert (entid);
    return sage_object_map_value(map, entid);
}

