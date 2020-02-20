#ifndef SAGE_STRING_H
#define SAGE_STRING_H


#include "exception.h"


typedef struct sage_string_t sage_string_t;

extern sage_string_t *sage_string_new(const char *str);

extern sage_string_t *sage_string_copy(const sage_string_t *ctx);

extern void sage_string_free(sage_string_t **ctx);

extern size_t sage_string_len(const sage_string_t *ctx);

extern size_t sage_string_size(const sage_string_t *ctx);

extern size_t sage_string_refcount(const sage_string_t *ctx);

extern const char *sage_string_cstr(const sage_string_t *ctx);


#endif /* SAGE_STRING_H */

