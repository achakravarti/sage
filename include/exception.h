
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

