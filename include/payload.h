#ifndef SAGE_PAYLOAD_H
#define SAGE_PAYLOAD_H

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


#endif /* SAGE_PAYLOAD_H */

