/** SCREEN **/


struct sage_viewport_t {
    struct sage_point_t point;
    struct sage_area_t area;
};


extern void
sage_screen_start(const char *title, struct sage_area_t res);

extern void
sage_screen_stop(void);

extern SAGE_HOT void*
sage_screen_brush(void);

extern SAGE_HOT void
sage_screen_clear(const sage_colour_t *col);

extern SAGE_HOT struct sage_viewport_t *
sage_screen_viewport(void);

extern SAGE_HOT void
sage_screen_viewport_set(const struct sage_viewport_t *vp);

extern SAGE_HOT void sage_screen_render(void);


