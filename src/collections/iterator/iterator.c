#include "iterator.h"

size_t estd_iter_count(iterator_t* iter){
    if(iter->count == NULL)
    {
        size_t count = 0;
        while (iter->move_next(iter)) count++;

        return count;
    }

    return iter->count(iter);
}

void estd_iter_dispose(iterator_t* iter){
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
