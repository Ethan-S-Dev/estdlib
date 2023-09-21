#include "filter.h"

#define nameof(item) #item

typedef struct iterator_filter_t{
    iterator source_iterator;
    bool (*predicate)(void *);
} iterator_filter_t;


static bool filter_move_next(iterator_filter_t* self){
    while(estd_iterator_move_next(self->source_iterator)){
        if(self->predicate(estd_iterator_current(self->source_iterator)))
            return true;
    }
    return false;
}

static void* filter_current(iterator_filter_t* self){
    return estd_iterator_current(self->source_iterator);
}

static void filter_reset(iterator_filter_t* self){
    estd_iterator_reset(self->source_iterator);
}

static disposable iterator_filter_dispose(iterator_filter_t* self){
    estd_disposable_dispose(estd_iterator_get_disposable(self->source_iterator));
    ESTD_FREE(self);
}

implement_disposable(iterator_filter_t, filter_get_disposable, iterator_filter_dispose)

static implement_iterator(iterator_filter_t, filter_iterator, filter_move_next, filter_current, filter_reset, filter_get_disposable);

iterator estd_filter_iterator(iterator iter, bool (*predicate)(void* item)){
    iterator_filter_t* iterator_filter = ESTD_MALLOC(sizeof(iterator_filter_t));
    iterator_filter->source_iterator = iter;
    iterator_filter->predicate = predicate;

    return filter_iterator(iterator_filter);
}




                                                                                    
