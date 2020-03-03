#include "graphics.h"


#define MAP_BUCKETS ((size_t) 16)

static thread_local sage_object_map *map = NULL;


extern void sage_texture_factory_init(void)
{
    if (sage_likely (!map))
        map = sage_object_map_new(MAP_BUCKETS);
}


extern void sage_texture_factory_exit(void)
{
    sage_object_map_free(&map);
}


extern void sage_texture_factory_register(sage_id id, const char *path)
{
    sage_assert (id && path && *path);
    sage_object_map_value_set(map, id, sage_texture_new(id, path));
}


extern sage_texture *sage_texture_factory_clone(sage_id id)
{
    sage_assert (id);
    return sage_object_map_value(map, id);
}


