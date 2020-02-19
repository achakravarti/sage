
/**
 * A hardware-accelerated texture.
 */
typedef struct sage_texture_t sage_texture_t;


/**
 * Creates a new texture.
 *
 * @param path File path of texture image.
 * @param id Texture ID.
 *
 * @return The new texture instance.
 *
 * @see sage_texture_copy()
 * @see sage_texture_free()
 */
extern SAGE_HOT sage_texture_t *
sage_texture_new(const char *path, 
                 sage_id_t  id);


extern const sage_texture_t *
sage_texture_copy(const sage_texture_t *ctx);


/**
 * Copies a texture.
 *
 * @param ctx Contextual exture instance to copy.
 *
 * @return The copy of @p ctx.
 *
 * @see sage_texture_new()
 * @see sage_texture_free()
 */
extern SAGE_HOT sage_texture_t *
sage_texture_copy_deep(const sage_texture_t *ctx);


/**
 * Releases a texture.
 *
 * @param ctx Texture to release.
 *
 * @return NULL.
 *
 * @see sage_texture_new()
 * @see sage_texture_copy()
 */
extern void 
sage_texture_free(sage_texture_t **ctx);


/**
 * Gets the size in bytes of a texture instance.
 *
 * @return The size of a texture instance.
 */
extern size_t 
sage_texture_size(void);


/**
 * Gets the ID of a texture.
 *
 * @param ctx Contextual texture instance.
 *
 * @return The ID of @p ctx.
 */
extern sage_id_t 
sage_texture_id(const sage_texture_t *ctx);


/**
 * Gets the area of a texture.
 *
 * @param ctx Contextual texture instance.
 *
 * @return The area of @p ctx.
 */
extern SAGE_HOT struct sage_area_t 
sage_texture_area(const sage_texture_t *ctx);


/**
 * Clips a texture.
 *
 * @param ctx  Contextual texture instance.
 * @param nw   North-west corner coordinates.
 * @param clip Clip area.
 *
 * @see sage_texture_scale()
 * @see sage_texture_reset()
 * @see sage_texture_draw()
 */
extern void 
sage_texture_clip(sage_texture_t      *ctx, 
                  struct sage_point_t nw,
                  struct sage_area_t  clip);


/**
 * Scales a texture.
 *
 * @param ctx  Contextual texture instance.
 * @param proj Projection area.
 *
 * @see sage_texture_clip()
 * @see sage_texture_reset()
 * @see sage_texture_draw()
 */
extern void 
sage_texture_scale(sage_texture_t     *ctx, 
                   struct sage_area_t proj);


/**
 * Resets a texture.
 *
 * @param ctx Contextual texture instance.
 *
 * @see sage_texture_clip()
 * @see sage_texture_scale()
 * @see sage_texture_draw()
 */
extern void 
sage_texture_reset(sage_texture_t *ctx);


/**
 * Draws a texture.
 *
 * @param ctx Contextual texture instance.
 * @param dst Destination coordinates.
 *
 * @see sage_texture_clip()
 * @see sage_texture_scale()
 * @see sage_texture_reset()
 */
extern SAGE_HOT void 
sage_texture_draw(const sage_texture_t *ctx, 
                  struct sage_point_t  dst);


/** TEXTURE FACTORY **/


extern void
sage_texture_factory_start(void);

extern void
sage_texture_factory_stop(void);

extern void
sage_texture_factory_register(sage_id_t  id,
                              const char *path);

extern sage_texture_t *
sage_texture_factory_spawn(sage_id_t id);
