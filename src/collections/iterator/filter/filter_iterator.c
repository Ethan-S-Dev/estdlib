#include "filter_iterator.h"

typedef struct _filter_iterator_t{
    iterator_t* iter;
    
    bool (*predicate)(void *);
} filter_iterator_t;

static void filter_iterator_reset(iterator_t *iter){
    filter_iterator_t *filter_iter = (filter_iterator_t *)iter->internal_iterator;
    filter_iter->iter->reset(filter_iter->iter);
}
static bool filter_iterator_move_next(iterator_t *iter){
    filter_iterator_t *filter_iter = (filter_iterator_t *)iter->internal_iterator;
    iterator_t *internal_iter = filter_iter->iter;
    while(internal_iter->move_next(internal_iter)){
        if(filter_iter->predicate(internal_iter->current(internal_iter))){
            return true;
        }
    }
    return false;
}
static void* filter_iterator_current(iterator_t *iter){
    filter_iterator_t *filter_iter = (filter_iterator_t *)iter->internal_iterator;
    return filter_iter->iter->current(filter_iter->iter);
}
static void filter_iterator_dispose(iterator_t *iter){
    filter_iterator_t *filter_iter = (filter_iterator_t *)iter->internal_iterator;
    ESTD_FREE(filter_iter);
}

iterator_t estd_iter_filter(iterator_t *iter, bool (*predicate)(void *)){
    iterator_t new_iter;
    filter_iterator_t *filter_iter = ESTD_MALLOC(sizeof(filter_iterator_t));
    filter_iter->iter = iter;
    filter_iter->predicate = predicate;

    new_iter.internal_iterator = filter_iter;
    new_iter.item_size = iter->item_size;

    new_iter.reset = filter_iterator_reset;
    new_iter.move_next = filter_iterator_move_next;
    new_iter.current = filter_iterator_current;
    new_iter.count = NULL;
    new_iter.dispose = filter_iterator_dispose;

    return new_iter;
}


typedef struct _filter_iterator_args_t{
    iterator_t* iter;
    void* args;
    
    bool (*predicate)(void * args, void * item);
} filter_iterator_args_t;

static bool filter_iterator_args_move_next(iterator_t *iter){
    filter_iterator_args_t *filter_iter = (filter_iterator_args_t *)iter->internal_iterator;
    iterator_t *internal_iter = filter_iter->iter;
    while(internal_iter->move_next(internal_iter))
        if(filter_iter->predicate(filter_iter->args, internal_iter->current(internal_iter)))
            return true;
           
    return false;
}
static size_t filter_iterator_args_count(iterator_t *iter){
    filter_iterator_args_t *filter_iter = (filter_iterator_args_t *)iter->internal_iterator;
    iterator_t *internal_iter = filter_iter->iter;
    size_t count = 0;
    while(internal_iter->move_next(internal_iter))
        if(filter_iter->predicate(filter_iter->args, internal_iter->current(internal_iter)))
            count++;
           
    return count;
}

iterator_t estd_iter_filter_args(iterator_t* iter, bool (*predicate)(void* args, void* item), void* args){
    iterator_t new_iter;
    filter_iterator_args_t* filter_iter = ESTD_MALLOC(sizeof(filter_iterator_args_t));
    filter_iter->iter = iter;
    filter_iter->predicate = predicate;
    filter_iter->args = args;
    
    new_iter.internal_iterator = filter_iter;
    new_iter.item_size = iter->item_size;
    
    new_iter.reset = filter_iterator_reset;
    new_iter.move_next = filter_iterator_args_move_next;
    new_iter.current = filter_iterator_current;
    new_iter.count = NULL;
    new_iter.dispose = filter_iterator_dispose;
    
    return new_iter;
}