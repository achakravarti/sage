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
 * This is the sage/src/vector.c source file; it implements the entity API of 
 * the SAGE Library.
 ******************************************************************************/


/*
 * The sage/include/api.h header file contains the declaration of the API of the
 * SAGE Library.
 */
#include "../include/api.h"


/*
 * The sage_payload_t struct was forward declared in the sage/src/sage.h header
 * file. We're defining the struct here with its attributes.
 *
 * The payload data is the data that is encapsulated within the payload. It's
 * important to remember that this data must be allocated on the heap. The size
 * attribute is the size in bytes of the payload data. The v-table attribute
 * holds the callback functions that are used to release and make deep copies of
 * the payload data.
 */
struct sage_payload_t {
    void                         *data; /* payload data           */
    size_t                       sz;    /* payload data size      */
    struct sage_payload_vtable_t vt;    /* payload data callbacks */
};


/*
 * The sage_payload_new() interface function creates a new payload instance. We
 * initialise the data, size and v-table attributes of the payload with the
 * arguments supplied. We need to ensure that both the copy_deep and free
 * callbacks are supplied by client code.
 */
extern sage_payload_t *
sage_payload_new(const void                         *data,
                 size_t                             sz,
                 const struct sage_payload_vtable_t *vt)
{
    sage_payload_t *ctx = sage_heap_new(sizeof *ctx);

    sage_assert (vt && vt->copy_deep && vt->free);
    ctx->vt.copy_deep = vt->copy_deep;
    ctx->vt.free = vt->free;

    sage_assert (sz && data);
    ctx->sz = sz;
    ctx->data = vt->copy_deep(data);

    return ctx;
}


/*
 * The sage_payload_copy() interface function creates a shallow copy of a given
 * payload instance. We do so by returning a constant pointer to the contextual
 * payload instance.
 */
extern inline const sage_payload_t *
sage_payload_link(const sage_payload_t *ctx);


extern inline sage_payload_t *
sage_payload_move(sage_payload_t *ctx);


/*
 * The sage_payload_copy() interface function creates a deep copy of a given
 * payload instance. We do so by creating a new payload instance with the same
 * attributes as the contextual instance, and then returning the newly created
 * instance.
 */
extern sage_payload_t *
sage_payload_copy(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return sage_payload_new(ctx->data, ctx->sz, &ctx->vt);
}


/*
 * The sage_payload_free() interface function releases an existing payload
 * instance from the heap. We first release the heap memory allocated to the
 * data contained within the payload before releasing the payload itself.
 */
extern void 
sage_payload_free(sage_payload_t **ctx)
{
    sage_payload_t *hnd;

    if (sage_likely (ctx && (hnd = *ctx))) {
        hnd->vt.free(&hnd->data);
        sage_heap_free((void **) ctx);
    }
}


/*
 * The sage_payload_size() interface function gets the size of a payload
 * instance. We do so by simply returning the size of the payload structure.
 */
extern size_t
sage_payload_size(void)
{
    return sizeof (struct sage_payload_t);
}


/*
 * The sage_payload_size_data() interface function gets the size of the data
 * contained within a payload instance. The data size is held within the sz
 * field.
 */
extern size_t 
sage_payload_size_data(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx->sz;
}


/*
 * The sage_payload_data() interface function gets the data contained within a
 * payload instance. We do so by returning a shallow copy of the data attribute.
 */
extern const void *
sage_payload_data(const sage_payload_t *ctx)
{
    sage_assert (ctx);
    return ctx->data;
}


/*
 * The sage_payload_data_set() interface function sets the data contained within
 * a payload instance. We do so by making a deep copy of the data argument to
 * the data attribute of the contextual payload instance.
 */
extern void 
sage_payload_data_set(sage_payload_t *ctx,
                      const void     *data)
{
    sage_assert (ctx && data);
    ctx->vt.free(&ctx->data);
    ctx->data = ctx->vt.copy_deep(data);
}


/******************************************************************************
 *                                   __.-._
 *                                   '-._"7'
 *                                    /'.-c
 *                                    |  /T
 *                                   _)_/LI
 ******************************************************************************/

