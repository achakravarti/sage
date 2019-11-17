#include "../src/sage.h"


int
main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    struct sage_area_t res = {.w = 640, .h = 480};
    sage_screen_t *screen = sage_screen_new ("Sage Test", &res);

    sage_screen_render (screen);
    sage_screen_free (screen);
}

