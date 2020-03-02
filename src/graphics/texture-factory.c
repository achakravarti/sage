#include "graphics.h"


#define MAP_BUCKETS ((size_t) 16)


static thread_local sage_id_map_t *map = NULL;


// TODO: implement map_copy_deep() and map_copy()
static inline void *
map_copy(const void *tex)
{
    return sage_texture_copy((const sage_texture_t *) tex);
}


static inline void
map_free(void **tex)
{
    sage_texture_free ((sage_texture_t **) tex);
}


extern void
sage_texture_factory_start(void)
{
    if (sage_likely (!map)) {
        struct sage_id_map_vtable_t vt = {.copy = map_copy, .free = map_free};
        map = sage_id_map_new (MAP_BUCKETS, sage_texture_size (), &vt);
    }
}


extern void
sage_texture_factory_stop(void)
{
    map = sage_id_map_free (map);
}


extern void
sage_texture_factory_register(sage_id_t  id,
                              const char *path)
{
    sage_id_map_value_set(map, id, sage_texture_new(path, id));
}


extern sage_texture_t *
sage_texture_factory_spawn(sage_id_t id)
{
    return (sage_texture_t *) sage_id_map_value (map, id);
}


static thread_local sage_object_map *map2 = NULL;


extern void sage_texture_factory2_init(void)
{
    if (sage_likely (!map))
        map2 = sage_object_map_new(MAP_BUCKETS);
}


extern void sage_texture_factory2_exit(void)
{
    sage_object_map_free(&map2);
}


extern void sage_texture_factory2_register(sage_id id, const char *path)
{
    sage_object_map_value_set(map2, id, sage_tex_new(id, path));
}


extern sage_tex *sage_texture_factory2_clone(sage_id id)
{
    return sage_object_map_value(map2, id);
}


