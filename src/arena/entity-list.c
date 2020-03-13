#include "arena.h"


extern inline sage_entity_list *sage_entity_list_new(void);

extern inline sage_entity_list *sage_entity_list_copy(
        const sage_entity_list *ctx);

extern inline void sage_entity_list_free(sage_entity_list **ctx);

extern inline size_t sage_entity_list_len(const sage_entity_list *ctx);

extern inline size_t sage_entity_list_find(const sage_entity_list *ctx, 
        sage_id id);

extern inline sage_entity *sage_entity_list_get(const sage_entity_list *ctx, 
        sage_id id);

extern inline sage_entity *sage_entity_list_get_at(const sage_entity_list *ctx,
        size_t idx);

extern inline void sage_entity_list_set(sage_entity_list **ctx, sage_id id,
        const sage_entity *obj);

extern inline void sage_entity_list_set_at(sage_entity_list **ctx, size_t idx,
        const sage_entity *obj);

extern inline void sage_entity_list_push(sage_entity_list **ctx, 
        const sage_entity *obj);

extern inline void sage_entity_list_pop(sage_entity_list **ctx, sage_id id);

extern inline void sage_entity_list_pop_at(sage_entity_list **ctx, size_t idx);

