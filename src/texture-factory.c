#include "sage.h"


static thread_local struct {
    sage_texture_t **pool;
    size_t len;
} *factory = NULL;


extern void
sage_texture_factory_start(size_t len)
{
    sage_require (factory = malloc (sizeof *factory));
    sage_require (factory->pool = malloc (sizeof *factory->pool * len));
    factory->len = len;
}


extern void
sage_texture_factory_stop(void)
{
    if (sage_likely (factory)) {
        for (register size_t i = 0; i < factory->len; i++)
            free (factory->pool [i]);
    }

    free (factory);
}


extern void
sage_texture_factory_register(const sage_texture_t *tex)
{
    factory->pool [sage_texture_id (tex)] = sage_texture_copy (tex);
}


extern sage_texture_t *
sage_texture_factory_spawn(sage_id_t id)
{
    return sage_texture_copy (factory->pool [id]);
}

