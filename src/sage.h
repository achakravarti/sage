#if (defined __GNUC__ || defined __clang__)
#   pragma once
#else
#   if (!defined SCHEME_ASSISTED_GAME_ENGINE)
#       define SCHEME_ASSISTED_GAME_ENGINE
#endif

#if defined __cplusplus
    extern "C" {
#endif


#if (defined __GNUC__ || defined __clang__)
#   define SAGE_PURE __attribute__((pure))
#else
#   define SAGE_PURE
#   warning "SAGE_PURE has no effect on non GCC-compatible compilers"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define SAGE_HOT __attribute__((hot))
#else
#   define SAGE_HOT
#   warning "SAGE_HOT has no effect on non GCC-compatible compilers"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define SAGE_COLD __attribute__((cold))
#else
#   define SAGE_COLD
#   warning "SAGE_COLD has no effect on non GCC-compatible compilers"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define sage_likely(p) (__builtin_expect (!!(p), 1))
#else
#   define sage_likely(p) (p)
#   warning "sage_likely() has no effecto on non GCC-compatible compilers"
#endif


#if defined __cplusplus
    } /* extern "C" */
#endif

#if !(defined __GNUC__ || defined __clang__)
#   endif /* SCHEME_ASSISTED_GAME_ENGINE */
#endif

