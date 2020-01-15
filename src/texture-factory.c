#include "sage.h"


#define MAP_BUCKETS ((size_t) 16)


static thread_local sage_id_map_t *map = NULL;


static inline void *map_copy(const void *tex)
{
    return sage_texture_copy ((const sage_texture_t *) tex);
}


static inline void
map_free(void *tex)
{
    (void) sage_texture_free ((sage_texture_t *) tex);
}


extern void
sage_texture_factory_start(void)
{
    if (sage_likely (!map)) {
        map = sage_id_map_new (MAP_BUCKETS, sage_texture_size (), map_copy,
            map_free);
    }
}


extern void
sage_texture_factory_stop(void)
{
    map = sage_id_map_free (map);
}


extern void
sage_texture_factory_register(const sage_texture_t *tex)
{
    sage_id_map_value_set (map, sage_texture_id (tex), tex);
}


extern sage_texture_t *
sage_texture_factory_spawn(sage_id_t id)
{
    return (sage_texture_t *) sage_id_map_value (map, id);
}

