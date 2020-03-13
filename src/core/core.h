/******************************************************************************
 *                           ____   __    ___  ____ 
 *                          / ___) / _\  / __)(  __)
 *                          \___ \/    \( (_ \ ) _) 
 *                          (____/\_/\_/ \___/(____)
 *
 * Schemable? Game Engine (SAGE) Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>.
 *
 * This code is released under the MIT License. See the accompanying
 * sage/LICENSE.md file or <http://opensource.org/licenses/MIT> for complete 
 * licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT THEM.
 *
 * This is the sage/src/utility.h header file; it declares the interface of
 * utility types and functions on with the SAGE Library is built.
 ******************************************************************************/


#ifndef SCHEME_ASSISTED_GAME_ENGINE_UTILITY_HEADER
#define SCHEME_ASSISTED_GAME_ENGINE_UTILITY_HEADER


#include <stddef.h>
#include <stdbool.h>
#include <stdio.h> /* for printf() */
#include <stdlib.h> /* for abort() and exit() */
#include <inttypes.h>
#include <threads.h>


enum sage_compare_t {
    SAGE_COMPARE_LT = -1,
    SAGE_COMPARE_EQ = 0,
    SAGE_COMPARE_GT = 1
};


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


#if !(defined NDEBUG)
#   define sage_assert(c)                                                \
    do {                                                                 \
        if (sage_unlikely (!(c))) {                                      \
            printf ("sage_assert() condition failed: %s [%s, %s, %d]\n", \
                    #c, __func__, __FILE__, __LINE__);                   \
            abort ();                                                    \
        }                                                                \
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


#define sage_heap_init()

#define sage_heap_exit()

extern void *sage_heap_new(size_t sz);

extern void *sage_heap_resize(void *ptr, size_t sz);

extern void sage_heap_free(void **bfr);


/**
 * sage_id - unique ID with high and low order components.
 */
typedef uint64_t sage_id;


enum sage_object_id {
    SAGE_OBJECT_ID_VECTOR = 1,
    SAGE_OBJECT_ID_TEXTURE,
    SAGE_OBJECT_ID_SPRITE,
    SAGE_OBJECT_ID_ENTITY,
    SAGE_OBJECT_ID_ENTITY_LIST,
    SAGE_OBJECT_ID_SCENE
};


/*
 * sage_id_new() - create new ID.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new(uint32_t hi, uint32_t lo);


/*
 * sage_id_new_random() - create new random ID.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new_random(void);


/*
 * sage_id_new_random_hi() - create new ID with random high order component.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new_random_hi(uint32_t lo);


/*
 * sage_id_new_random_lo() - create new ID with random low order component.
 * See sage/src/id.c for details.
 */
extern sage_id sage_id_new_random_lo(uint32_t hi);


/**
 * sage_id_copy() - create a copy of an ID.
 */
inline sage_id sage_id_copy(sage_id ctx)
{
    return ctx;
}


/**
 * sage_id_free() - destroy an existing ID.
 */
#define /* void */ sage_id_free(/* sage_id * */ id)


/*
 * sage_id_hi() - get high order component of ID.
 * See sage/src/id.c for details.
 */
extern uint32_t sage_id_hi(sage_id ctx);


/*
 * sage_id_hi_set() - set high order component of ID.
 * See sage/src/id.c for details.
 */
extern void sage_id_hi_set(sage_id *ctx, uint32_t hi);


/*
 * sage_id_lo() - get low order component of ID.
 * See sage/src/id.c for details.
 */
extern uint32_t sage_id_lo(sage_id ctx);


/*
 * sage_id_lo_set() - set low order component of ID.
 * See sage/src/id.c for details.
 */
extern void sage_id_lo_set(sage_id *ctx, uint32_t lo);




typedef size_t sage_id_t;


typedef struct sage_string_t sage_string_t;

extern sage_string_t *sage_string_new(const char *str);

extern sage_string_t *sage_string_copy(const sage_string_t *ctx);

extern void sage_string_free(sage_string_t **ctx);

extern size_t sage_string_len(const sage_string_t *ctx);

extern size_t sage_string_size(const sage_string_t *ctx);

extern size_t sage_string_refcount(const sage_string_t *ctx);

extern const char *sage_string_cstr(const sage_string_t *ctx);


struct sage_object_vtable {
    void *(*copy)(const void *ctx);
    void (*free)(void **ctx);
};


typedef struct sage_object sage_object;

extern sage_object *sage_object_new(sage_id id, void *cdata, 
        const struct sage_object_vtable *vt);

extern sage_object *sage_object_copy(const sage_object *ctx);

extern void sage_object_free(sage_object **ctx);

extern sage_id sage_object_id(const sage_object *ctx);

extern void sage_object_id_set(sage_object **ctx, sage_id id);

extern const void *sage_object_cdata(const sage_object *ctx);

extern void *sage_object_cdata_mutable(sage_object **ctx);


typedef struct sage_object_map sage_object_map;

extern sage_object_map *sage_object_map_new(size_t buck);

extern void sage_object_map_free(sage_object_map **ctx);

extern size_t sage_object_map_hash(const sage_object_map *ctx, sage_id key);

extern sage_object *sage_object_map_value(const sage_object_map *ctx, 
        sage_id key);

extern void sage_object_map_value_set(sage_object_map *ctx, sage_id key, 
        const sage_object *val);


typedef sage_object sage_object_list;

extern sage_object_list *sage_object_list_new(void);

inline sage_object_list *sage_object_list_copy(const sage_object_list *ctx)
{
    sage_assert (ctx);
    return sage_object_copy(ctx);
}

inline void sage_object_list_free(sage_object_list **ctx)
{
    sage_object_free(ctx);
}

extern size_t sage_object_list_len(const sage_object_list *ctx);

extern size_t sage_object_list_find(const sage_object_list *ctx, sage_id id);

extern sage_object *sage_object_list_get(const sage_object_list *ctx, 
        sage_id id);

extern sage_object *sage_object_list_get_at(const sage_object_list *ctx,
        size_t idx);

extern void sage_object_list_set(sage_object_list **ctx, sage_id id,
        const sage_object *obj);

extern void sage_object_list_set_at(sage_object_list **ctx, size_t idx,
        const sage_object *obj);

extern void sage_object_list_push(sage_object_list **ctx, 
        const sage_object *obj);

extern void sage_object_list_pop(sage_object_list **ctx, sage_id id);

extern void sage_object_list_pop_at(sage_object_list **ctx, size_t idx);


struct sage_point_t {
    float x;
    float y;
};


struct sage_area_t {
    uint16_t h;
    uint16_t w;
};


/** VECTOR **/

typedef sage_object sage_vector;

extern sage_vector *sage_vector_new(float x, float y);

extern sage_vector *sage_vector_new_zero(void);

inline sage_vector *sage_vector_copy(const sage_vector *ctx)
{
    sage_assert (ctx);
    return sage_object_copy(ctx);
}

inline void sage_vector_free(sage_vector **ctx)
{
    sage_object_free(ctx);
}


extern float sage_vector_x(const sage_vector *ctx);

extern void sage_vector_x_set(sage_vector **ctx, float x);

extern float sage_vector_y(const sage_vector *ctx);

extern void sage_vector_y_set(sage_vector **ctx, float y);

extern struct sage_point_t sage_vector_point(const sage_vector *ctx);

extern float sage_vector_len(const sage_vector *ctx);

extern bool sage_vector_visible(const sage_vector *ctx);

extern void sage_vector_norm(sage_vector **ctx);

extern enum sage_compare_t sage_vector_cmp(const sage_vector *lhs,
        const sage_vector *rhs);

inline bool sage_vector_lt(const sage_vector *ctx, const sage_vector *rhs)
{
    sage_assert (ctx && rhs);
    return sage_vector_cmp(ctx, rhs) == SAGE_COMPARE_LT;
}

inline bool sage_vector_eq(const sage_vector *ctx, const sage_vector *rhs)
{
    sage_assert (ctx && rhs);
    return sage_vector_cmp(ctx, rhs) == SAGE_COMPARE_EQ;
}

inline bool sage_vector_gt(const sage_vector *ctx, const sage_vector *rhs)
{
    sage_assert (ctx && rhs);
    return sage_vector_cmp(ctx, rhs) == SAGE_COMPARE_GT;
}

inline bool sage_vector_lteq(const sage_vector *ctx, const sage_vector *rhs)
{
    sage_assert (ctx && rhs);
    return !sage_vector_gt(ctx, rhs);
}


inline bool sage_vector_gteq(const sage_vector *ctx, const sage_vector *rhs)
{
    sage_assert (ctx && rhs);
    return !sage_vector_lt(ctx, rhs);
}

extern void sage_vector_add(sage_vector **ctx, const sage_vector *add);

extern void sage_vector_sub(sage_vector **ctx, const sage_vector *sub);

extern void sage_vector_mul(sage_vector **ctx, float mul);

extern void sage_vector_div(sage_vector **ctx, const float div);


struct sage_id_map_vtable_t {
    void *(*copy) (const void *ctx);
    void (*free) (void **ctx);
};


typedef struct sage_id_map_t sage_id_map_t;

/*typedef void *(sage_id_map_copy_f)(const void *ctx);

typedef void (sage_id_map_free_f)(void *ctx);*/


/*extern sage_id_map_t *sage_id_map_new(size_t buck, size_t sz, 
    sage_id_map_copy_f *copy, sage_id_map_free_f *free);*/


extern sage_id_map_t *
sage_id_map_new(size_t buck,
                size_t sz,
                struct sage_id_map_vtable_t *vt);



extern sage_id_map_t *
sage_id_map_free(sage_id_map_t *ctx);

extern size_t
sage_id_map_hash(const sage_id_map_t *ctx, sage_id_t key);

extern void *
sage_id_map_value(sage_id_map_t *ctx, sage_id_t key);

extern void
sage_id_map_value_set(sage_id_map_t *ctx, sage_id_t key, void *val);


typedef struct sage_payload_t sage_payload_t;

struct sage_payload_vtable_t {
    void *(*copy_deep)(const void *data);
    void (*free)      (void **data);
};

extern sage_payload_t *
sage_payload_new(const void                         *data,
                 size_t                             sz,
                 const struct sage_payload_vtable_t *vt);

inline const sage_payload_t *
sage_payload_link(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

inline sage_payload_t *
sage_payload_move(sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx;
}

extern sage_payload_t *
sage_payload_copy(const sage_payload_t *ctx);

extern void 
sage_payload_free(sage_payload_t **ctx);

extern size_t
sage_payload_size(void);

extern size_t 
sage_payload_size_data(const sage_payload_t *ctx);

extern const void *
sage_payload_data(const sage_payload_t *ctx);

extern void 
sage_payload_data_set(sage_payload_t *ctx,
                      const void     *data);


#endif /* SCHEME_ASSISTED_GAME_ENGINE_UTILITY_HEADER */


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 ******************************************************************************/

