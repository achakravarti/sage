#ifndef SAGE_ARENA_H
#define SAGE_ARENA_H

extern void 
sage_arena_start(void);

extern void 
sage_arena_stop(void);

extern const sage_entity_t *
sage_arena_entity(size_t idx);

extern void
sage_arena_entity_set(size_t idx, const sage_entity_t *ent);

extern size_t
sage_arena_push(const sage_entity_t *ent);

extern void 
sage_arena_pop(size_t idx);

extern void 
sage_arena_update(void);

extern void 
sage_arena_draw(void);

#endif /* SAGE_ARENA_H */

