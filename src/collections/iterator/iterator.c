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
    while(iter->move_next(iter)){
        func(iter->current(iter));
    }
}
void estd_iter_for_each_args(iterator_t* iter, void (*func)(void *, void *), void *args){
    iter->reset(iter);
    while(iter->move_next(iter)){
        func(iter->current(iter), args);
    }
}
void estd_iter_for_each_indexed(iterator_t* iter, void (*func)(void *, size_t)){
    iter->reset(iter);
    size_t index = 0;
    while(iter->move_next(iter)){
        func(iter->current(iter), index++);
    }
}
void estd_iter_for_each_indexed_args(iterator_t* iter, void (*func)(void *, size_t, void *), void *args){
    iter->reset(iter);
    size_t index = 0;
    while(iter->move_next(iter)){
        func(iter->current(iter), index++, args);
    }
}

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
static size_t filter_iterator_count(iterator_t *iter){
    filter_iterator_t *filter_iter = (filter_iterator_t *)iter->internal_iterator;
    iterator_t *internal_iter = filter_iter->iter;
    size_t count = 0;
    while(internal_iter->move_next(internal_iter)){
        if(filter_iter->predicate(internal_iter->current(internal_iter))){
            count++;
        }
    }
    return count;
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
    new_iter.count = filter_iterator_count;
    new_iter.dispose = filter_iterator_dispose;
}

typedef struct _map_iterator_t{
    iterator_t* iter;
    size_t new_item_size;
    void* latest_item;

    void* (*builder)(void *);
} map_iterator_t;

static void clean_latest_item(map_iterator_t* map_iter){
    if(map_iter->latest_item != NULL)
    {
        ESTD_FREE(map_iter->latest_item);
        map_iter->latest_item = NULL;
    }
}

static void map_iterator_reset(iterator_t *iter){
    map_iterator_t *map_iter = (map_iterator_t *)iter->internal_iterator;

    clean_latest_item(map_iter);

    map_iter->iter->reset(map_iter->iter);
}
static bool map_iterator_move_next(iterator_t *iter){
    map_iterator_t *map_iter = (map_iterator_t *)iter->internal_iterator;
    iterator_t *internal_iter = map_iter->iter;

    if(internal_iter->move_next(internal_iter)){
        clean_latest_item(map_iter);
        return true;
    }

    return false;
}
static void* map_iterator_current(iterator_t *iter){
    map_iterator_t *map_iter = (map_iterator_t *)iter->internal_iterator;
    iterator_t *internal_iter = map_iter->iter;
    return map_iter->builder(internal_iter->current(internal_iter));
}
static size_t map_iterator_count(iterator_t *iter){
    map_iterator_t *map_iter = (map_iterator_t *)iter->internal_iterator;
    iterator_t *internal_iter = map_iter->iter;
    return internal_iter->count(internal_iter);
}

static void map_iterator_dispose(iterator_t *iter){
    map_iterator_t *map_iter = (map_iterator_t *)iter->internal_iterator;
    clean_latest_item(map_iter);
    ESTD_FREE(map_iter);
}

iterator_t estd_iter_map(iterator_t *iter, size_t new_item_size, void* (*builder)(void *)){
    iterator_t new_iter;
    map_iterator_t *map_iter = ESTD_MALLOC(sizeof(map_iterator_t));

    map_iter->iter = iter;
    map_iter->new_item_size = new_item_size;
    map_iter->builder = builder;
    map_iter->latest_item = NULL;

    new_iter.internal_iterator = map_iter;
    new_iter.item_size = new_item_size;

    new_iter.reset = map_iterator_reset;
    new_iter.move_next = map_iterator_move_next;
    new_iter.current = map_iterator_current;
    new_iter.count = map_iterator_count;
    new_iter.dispose = map_iterator_dispose;
}