#include "./sage.h"


struct node_t {
    sage_state_t        *scn;
    struct node_t *nxt;
    struct node_t *prv;
};



static thread_local struct {
    struct node_t *head;
    struct node_t *tail;
} *list = NULL;


static struct node_t *
node_new(sage_state_t *scn)
{
    struct node_t *ctx = sage_heap_new(sizeof *ctx);

    sage_assert (scn);
    ctx->scn = scn;
    ctx->nxt = ctx->prv = NULL;

    return ctx;
}


static inline void
node_free(struct node_t **ctx)
{
    sage_state_free(&(*ctx)->scn);
    sage_heap_free((void **) ctx);
}


static void
list_push(sage_state_t *scn)
{
    struct node_t *node = node_new(scn);

    sage_assert (list);
    if (sage_likely (list->tail)) {
        node->prv = list->tail;
        list->tail->nxt = node;
    } else
        list->head = node;

    list->tail = node;
}


static void
list_pop(void)
{
    sage_assert (list && list->tail);
    struct node_t *pop = list->tail;

    if (pop == list->head)
        list->tail = list->head = NULL;
    else {
        list->tail = list->tail->prv;
        list->tail->nxt = NULL;
    }
        
    node_free(&pop);
}


extern void
sage_stage_init(void)
{
    sage_assert (!list);
    list = sage_heap_new(sizeof *list);
    list->head = list->tail = NULL;
}


extern void
sage_stage_exit(void)
{
    if (sage_likely (list)) {
        struct node_t *itr = list->head, *tmp;

        while (sage_likely (itr)) {
            tmp = itr;
            itr = itr->nxt;
            node_free(&tmp);
        }

        sage_heap_free((void **) &list);
    }
}


extern void
sage_stage_segue(sage_state_t *scn)
{
    sage_assert (list);
    if (list->tail) {
        sage_assert (sage_state_id(scn) != sage_state_id(list->tail->scn));
        sage_state_stop(list->tail->scn);
        list_pop();
    }

    list_push(scn);
    sage_state_start(scn);
}


extern void
sage_stage_interval(sage_state_t *scn)
{
    sage_assert (list && list->tail && scn);
    list_push(scn);
    sage_state_start(scn);
}


extern void
sage_stage_restore(void)
{
    sage_assert (list && list->tail && list->tail != list->head);
    sage_state_stop(list->tail->scn);
    list_pop();
}


extern void
sage_stage_update(void)
{
    sage_assert (list && list->tail);
    sage_state_update(list->tail->scn);
}


extern void
sage_stage_draw(void)
{
    sage_assert (list && list->tail);
    sage_state_draw(list->tail->scn);
}

