
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


/**************************************************************************//**
 * @defgroup hint Compiler Hints Module
 * Compiler hints for optimisation.
 *
 * Compilers such as GCC have always focused aggressively on optimisation. Many
 * such optimisations have been introduced as compiler-specific extensions that
 * are @b not part of the C standards.
 *
 * The Compiler Hints Module abstracts such optimisations as conditional macros
 * so that they they can be taken advantage of in GCC and GCC-compatible 
 * compilers such as Clang, but at the same time degrade safely on other 
 * compilers.
 * @{
 */


/**
 * Hints that a function is called frequently.
 *
 * The @c SAGE_HOT macro is used to decorate a function declaration in order to
 * indicate that it is called frequently. This hint follows the GCC attribute
 * syntax, and is recommended to be placed in the function declaration just
 * before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see SAGE_COLD
 */
#if (sage_compiler_gnuex ())
#   define SAGE_HOT __attribute__((hot))
#else
#   define SAGE_HOT
#   warning "SAGE_HOT has no effect on non GCC-compatible compilers"
#endif


/**
 * Hints that a function is called infrequently.
 *
 * The @c SAGE_COLD macro is used to decorate a function declaration in order to
 * indicate that is called infrequently. This hint follows the GCC attribute
 * syntax, and is recommended to be placed in the function declaration just
 * before the function return type.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see SAGE_HOT
 */
#if (sage_compiler_gnuex ())
#   define SAGE_COLD __attribute__((cold))
#else
#   define SAGE_COLD
#   warning "SAGE_COLD has no effect on non GCC-compatible compilers"
#endif


/**
 * Hints that a predicate is likely to be true.
 *
 * The @c sage_likely() macro indicates that a predicate expression @p p is @b
 * likely to evaluate to @c true. This hint has been modelled after the @c
 * likely() macro used in the Linux kernel source.
 *
 * @param p Predicate expected to be true.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see sage_unlikely()
 */
#if (sage_compiler_gnuex ())
#   define sage_likely(p) (__builtin_expect (!!(p), 1))
#else
#   define sage_likely(p) (p)
#   warning "sage_likely() has no effect on non GCC-compatible compilers"
#endif


/**
 * Hints that a predicate is likely to be false.
 *
 * The @c sage_unlikely() macro indicates that a predicate expression @p p is @b
 * unlikely to evaluate to @c false. This hint has been modelled after the @c
 * unlikely() macro used in the Linux kernel source.
 *
 * @param p Predicate expected to be false.
 *
 * @warning This hint is available only on GCC and GCC-compatible compilers such
 * as Clang; on other compilers, this macro degrades with a warning that it has
 * no effect.
 *
 * @see sage_likely()
 */
#if (sage_compiler_gnuex ())
#   define sage_unlikely(p) (__builtin_expect (!!(p), 0))
#else
#   define sage_unlikely(p) (p)
#   warning "sage_unlikely() has no effect on non GCC-compatible compilers"
#endif



