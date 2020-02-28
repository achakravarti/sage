#ifndef SAGE_TEX_H
#define SAGE_TEX_H


#include "exception.h"
#include "object.h"


typedef sage_object sage_tex;

extern sage_tex *sage_tex_new(sage_id id, const char *path);

inline sage_tex *sage_tex_copy(const sage_tex *ctx)
{
    sage_assert (ctx);
    return sage_object_copy(ctx);
}

inline void sage_tex_free(sage_tex **ctx)
{
    sage_object_free(ctx);
}

inline size_t sage_tex_size(void)
{
    return sage_object_size();
}

inline sage_id sage_tex_id(const sage_tex *ctx)
{
    sage_assert (ctx);
    return sage_object_id(ctx);
}

extern struct sage_area_t sage_tex_area(const sage_tex *ctx);

extern void sage_tex_clip(sage_tex **ctx, struct sage_point_t nw, 
        struct sage_area_t clip);

extern void sage_tex_scale(sage_tex **ctx, struct sage_area_t proj);

extern void sage_tex_reset(sage_tex **ctx);

extern void sage_tex_draw(const sage_tex *ctx, struct sage_point_t dst);

#endif /* SAGE_TEX_H */
