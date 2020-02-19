#ifndef SAGE_STAGE_H
#define SAGE_STAGE_H


extern void
sage_stage_init(void);

extern void
sage_stage_exit(void);

extern void
sage_stage_segue(sage_scene_t *scn);

extern void
sage_stage_interval(sage_scene_t *scn);

extern void
sage_stage_restore(void);

extern void
sage_stage_update(void);

extern void
sage_stage_draw(void);


#endif /* SAGE_STAGE_H */

