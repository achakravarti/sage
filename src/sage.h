#if (!defined SCHEME_ASSISTED_GAME_ENGINE)
#   define SCHEME_ASSISTED_GAME_ENGINE


#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>


#if defined __cplusplus
    extern "C" {
#endif


#define SAGE_COMPILER_GCC (0)
#define SAGE_COMPILER_CLANG (1)

/* https://stackoverflow.com/questions/38499462/ */
#if (defined __GNUC__ && !defined __clang__)
#   define sage_compiler() SAGE_COMPILER_GCC
#elif (defined __clang__)
#   define sage_compiler() SAGE_COMPILER_CLANG
#else
#   error "sage_compiler(): unsupported C compiler"
#endif


#define sage_compiler_gnuex()              \
    (sage_compiler () == SAGE_COMPILER_GCC \
     || sage_compiler () == SAGE_COMPILER_CLANG)


#if (sage_compiler_gnuex ())
#   define SAGE_PURE __attribute__((pure))
#else
#   define SAGE_PURE 
#   warning "SAGE_PURE has no effect on non GCC-compatible compilers" 
#endif


#if (sage_compiler_gnuex ())
#   define SAGE_HOT __attribute__((hot))
#else
#   define SAGE_HOT
#   warning "SAGE_HOT has no effect on non GCC-compatible compilers"
#endif


#if (sage_compiler_gnuex ())
#   define SAGE_COLD __attribute__((cold))
#else
#   define SAGE_COLD
#   warning "SAGE_COLD has no effect on non GCC-compatible compilers"
#endif


#if (sage_compiler_gnuex ())
#   define sage_likely(p) (__builtin_expect (!!(p), 1))
#else
#   define sage_likely(p) (p)
#   warning "sage_likely() has no effect on non GCC-compatible compilers"
#endif


#if (sage_compiler_gnuex ())
#   define sage_unlikely(p) (__builtin_expect (!!(p), 0))
#else
#   define sage_unlikely(p) (p)
#   warning "sage_unlikely() has no effect on non GCC-compatible compilers"
#endif


#if !(defined NDEBUG)
#   define sage_assert(c)                                                \
    do {                                                                 \
        if (sage_unlikely (!(c))) {                                      \
            printf ("sage_assert() condition failed: %s [%s, %s, %d]\n", \
                    #c, __func__, __FILE__, __LINE__);                   \
            abort ();                                                    \
    } while (0)
#else
#   define sage_assert(c)
#endif


#define sage_require(c)                                                 \
do {                                                                    \
    if (sage_unlikely (!(c))) {                                         \
        printf ("sage_require() condition failed @ %s() [%s:%d]: %s\n", \
                __func__, __FILE__, __LINE__, #c);                      \
        exit (EXIT_FAILURE);                                            \
    }                                                                   \
} while (0)


struct sage_point_t {
    int16_t x;
    int16_t y;
};


struct sage_area_t {
    uint16_t h;
    uint16_t w;
};


extern void
sage_screen_start(const char *title, struct sage_area_t res);

extern void
sage_screen_stop(void);

extern SAGE_HOT void*
sage_screen_brush(void);

extern SAGE_HOT void
sage_screen_clear(void);

extern SAGE_HOT void sage_screen_render(void);


typedef struct __sage_texture sage_texture_t;

extern SAGE_HOT sage_texture_t *sage_texture_new(const char *path);

extern sage_texture_t*
sage_texture_free(sage_texture_t *tex);

extern SAGE_HOT void
sage_texture_draw(sage_texture_t *tex, struct sage_point_t dst);

extern SAGE_HOT void
sage_texture_draw_clipped(sage_texture_t *tex, 
                          struct sage_point_t dst,
                          struct sage_point_t src,
                          struct sage_area_t clip);

extern SAGE_HOT void
sage_texture_draw_scaled(sage_texture_t *tex, 
                         struct sage_point_t dst,
                         struct sage_area_t proj,
                         struct sage_point_t src,
                         struct sage_area_t clip);


#if defined __cplusplus
    } /* extern "C" */
#endif

#endif /* SCHEME_ASSISTED_GAME_ENGINE */

