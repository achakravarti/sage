#include "sage.h"


struct node_t {
    sage_id_t id;
    sage_texture_t *tex;
    struct node_t *next;
};


static thread_local struct {
    size_t len;
    struct node_t **buck;
} *map = NULL;


#define MAP_BUCKETS ((size_t) 16)


static inline size_t
map_hash(sage_id_t id)
{
    return id % map->len;
}


extern void
sage_texture_factory_start(void)
{
    if (sage_unlikely (map))
        return;

    sage_require (map = malloc (sizeof *map));
    map->len = MAP_BUCKETS;

    sage_require (map->buck = malloc (sizeof *map->buck * map->len));
    for (register size_t i = 0; i < map->len; i++)
        map->buck [i] = NULL;
}


extern void
sage_texture_factory_stop(void)
{
    if (sage_unlikely (!map))
        return;

    struct node_t *buck, *next;
    for (register size_t i = 0; i < map->len; i++) {
        if ((buck = map->buck [i])) {
            do {
                next = buck->next;
                sage_texture_free (buck->tex);
                free (buck);
                buck = next;
            } while (buck);
        }
    }

    free (map->buck);
    free (map);
}


extern void
sage_texture_factory_register(const sage_texture_t *tex)
{
    sage_id_t id = sage_texture_id (tex);
    size_t hash = map_hash (id);
    struct node_t *buck = map->buck [hash];
    
    struct node_t *itr = buck;
    while (itr) {
        if (itr->id == id) {
            (void) sage_texture_free (itr->tex);
            itr->tex = sage_texture_copy (tex);
            return;
        }

        itr = itr->next;
    }

    struct node_t *add;
    sage_require (add = malloc (sizeof *add));
    add->id = id;
    add->tex = sage_texture_copy (tex);
    add->next = buck;
    map->buck [hash] = add;
}


extern sage_texture_t *
sage_texture_factory_spawn(sage_id_t id)
{
    struct node_t *buck = map->buck [map_hash (id)];
    struct node_t *itr = buck;

    while (itr) {
        if (itr->id == id)
            return sage_texture_copy (itr->tex);

        itr = itr->next;
    }

    // TODO: id not found, so abort; think of cleaner code
    sage_require (itr != NULL);
    return NULL;
}

