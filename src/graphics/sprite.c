#include <SDL2/SDL.h>
#include "graphics.h"

struct cdata {
    sage_texture *tex;
    struct sage_frame_t tot;
    struct sage_frame_t cur;
    SDL_Rect clip;
    struct sage_area_t proj;
};


static inline struct cdata *cdata_new(sage_id texid, struct sage_frame_t tot)
{
    struct cdata *ctx = sage_heap_new(sizeof *ctx);
    
    ctx->tex = sage_texture_factory_clone(texid);
    ctx->tot = tot;

    ctx->clip.x = ctx->clip.y = 0;
    ctx->cur.r = ctx->cur.c = 1;

    struct sage_area_t totarea = sage_texture_area(ctx->tex);
    struct sage_area_t frmarea = { 
        .w = totarea.w / ctx->tot.c,
        .h = totarea.h / ctx->tot.r
    };

    ctx->clip.w = ctx->proj.w = frmarea.w;
    ctx->clip.h = ctx->proj.h = frmarea.h;

    return ctx;
}


static inline void *cdata_copy(const void *ctx)
{
    const struct cdata *hnd = (const struct cdata *) ctx;
    struct cdata *cp = cdata_new(sage_texture_id(hnd->tex), hnd->tot);

    cp->cur = hnd->cur;
    cp->clip = hnd->clip;
    cp->proj = hnd->proj;

    return cp;
}


static inline void cdata_free(void **ctx)
{
    struct cdata *hnd = *((struct cdata **) ctx);
    sage_texture_free(&hnd->tex);
}


extern sage_sprite *sage_sprite_new(sage_id texid, struct sage_frame_t tot)
{
    sage_assert (texid);
    struct sage_object_vtable vt = { .copy = &cdata_copy, .free = &cdata_free };
    return sage_object_new(texid, cdata_new(texid, tot), &vt);
}


extern inline sage_sprite *sage_sprite_copy(const sage_sprite *ctx);


extern inline void sage_sprite_free(sage_sprite **ctx);


extern inline sage_id sage_sprite_id(const sage_sprite *ctx);


extern struct sage_area_t sage_sprite_area(const sage_sprite *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return sage_texture_area(cd->tex);
}


extern struct sage_area_t sage_sprite_area_frame(const sage_sprite *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    struct sage_area_t tot = sage_texture_area(cd->tex);
    struct sage_area_t frm = {
        .w = tot.w / cd->tot.c,
        .h = tot.h / cd->tot.r
    };

    return frm;
}


extern void sage_sprite_frame(sage_sprite **ctx, struct sage_frame_t frm)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->cur = frm;
}


extern struct sage_frame_t sage_sprite_frames(const sage_sprite *ctx)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);
    return cd->tot;
}


extern void sage_sprite_clip(sage_sprite **ctx, struct sage_point_t nw,
        struct sage_area_t clip)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    cd->clip.x = (int) nw.x;
    cd->clip.y = (int) nw.y;
    cd->clip.w = clip.w;
    cd->clip.h = clip.h;
}


extern void sage_sprite_scale(sage_sprite **ctx, struct sage_area_t proj)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);
    cd->proj = proj;
}


extern void sage_sprite_reset(sage_sprite **ctx)
{
    sage_assert (ctx);
    struct cdata *cd = sage_object_cdata_mutable(ctx);

    cd->clip.x = cd->clip.y = 0;
    cd->cur.r = cd->cur.c = 1;

    sage_assert (*ctx);
    struct sage_area_t frm = sage_sprite_area_frame(*ctx);
    cd->clip.w = cd->proj.w = frm.w;
    cd->clip.h = cd->proj.h = frm.h;
}


extern void sage_sprite_draw(const sage_sprite *ctx, struct sage_point_t dst)
{
    sage_assert (ctx);
    const struct cdata *cd = sage_object_cdata(ctx);

    struct sage_area_t frm = sage_sprite_area_frame(ctx);
    struct sage_point_t nw = {
        .x = (float) (frm.w * (cd->cur.c - 1) + cd->clip.x),
        .y = (float) (frm.h * (cd->cur.r - 1) + cd->clip.y)
    };
    struct sage_area_t clip = { .w = cd->clip.w, .h = cd->clip.h };

    sage_sprite *hnd = (sage_sprite *) ctx;
    struct cdata *cdm = sage_object_cdata_mutable(&hnd);

    sage_texture_clip(&cdm->tex, nw, clip);
    sage_texture_scale(&cdm->tex, cdm->proj);
    sage_texture_draw(cdm->tex, dst);
}

