#include "iterator.h"

iterator_t estd_iter_create_empty(){
    iterator_t iter = {
        .item_size = 0,
        .internal_iterator = NULL,

        .current = NULL,
        .move_next = NULL,
        .reset = NULL,
        .dispose = NULL,
        .count = NULL
    };

    return iter;
}

bool estd_iter_is_empty(iterator_t* iter){
    return iter->internal_iterator == NULL &&
        iter->item_size == 0 &&
        iter->current == NULL &&
        iter->move_next == NULL &&
        iter->reset == NULL &&
        iter->dispose == NULL &&
        iter->count == NULL;
}

size_t estd_iter_count(iterator_t* iter){
    if(iter->count != NULL) return iter->count(iter);

    iter->reset(iter);
    if(iter->current(iter) == NULL) return 0;

    size_t count = 1;

    while (iter->move_next(iter)) count++;

    return count;
}

void estd_iter_dispose(iterator_t* iter){
    if(iter->dispose == NULL) return;
    iter->dispose(iter);
}

void estd_iter_for_each(iterator_t* iter, void (*func)(void *)){
    iter->reset(iter);
    while(iter->move_next(iter)) func(iter->current(iter));
}
void estd_iter_for_each_args(iterator_t* iter, void (*func)(void *, void *), void *args){
    iter->reset(iter);
    while(iter->move_next(iter)) func(iter->current(iter), args);
}
void estd_iter_for_each_indexed(iterator_t* iter, void (*func)(void *, size_t)){
    iter->reset(iter);
    size_t index = 0;
    while(iter->move_next(iter)) func(iter->current(iter), index++);
}
void estd_iter_for_each_indexed_args(iterator_t* iter, void (*func)(void *, void *, size_t), void *args){
    iter->reset(iter);
    size_t index = 0;
    while(iter->move_next(iter)) func(args, iter->current(iter), index++);
}
