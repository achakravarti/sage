#include "graphics.h"


struct sage_colour_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};


extern sage_colour_t *
sage_colour_new(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    sage_colour_t *ctx;
    sage_require (ctx = malloc (sizeof *ctx));

    ctx->red = red;
    ctx->green = green;
    ctx->blue = blue;
    ctx->alpha = alpha;

    return ctx;
}


extern sage_colour_t *
sage_colour_new_hue(enum sage_hue_t hue)
{
    const uint32_t mask = 0xFF000000;

    const uint8_t red = hue & mask;
    const uint8_t green = (hue << 1) & mask;
    const uint8_t blue = (hue << 2) & mask;
    const uint8_t alpha = 0xFF;

    return sage_colour_new (red, green, blue, alpha);
}


extern sage_colour_t *
sage_colour_copy(const sage_colour_t *src)
{
    return sage_colour_new (src->red, src->green, src->blue, src->alpha);
}


extern sage_colour_t *
sage_colour_free(sage_colour_t *ctx)
{
    free (ctx);
    return NULL;
}


extern uint8_t
sage_colour_red(const sage_colour_t *ctx)
{
    return ctx->red;
}


extern void
sage_colour_red_set(sage_colour_t *ctx, uint8_t red)
{
    ctx->red = red;
}


extern uint8_t
sage_colour_green(const sage_colour_t *ctx)
{
    return ctx->green;
}


extern void
sage_colour_green_set(sage_colour_t *ctx, uint8_t green)
{
    ctx->green = green;
}


extern uint8_t
sage_colour_blue(const sage_colour_t *ctx)
{
    return ctx->blue;
}


extern void
sage_colour_blue_set(sage_colour_t *ctx, uint8_t blue)
{
    ctx->blue = blue;
}


extern uint8_t
sage_colour_alpha(const sage_colour_t *ctx)
{
    return ctx->alpha;
}


extern void
sage_colour_alpha_set(sage_colour_t *ctx, uint8_t alpha)
{
    ctx->alpha = alpha;
}

