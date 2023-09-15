#include "map_iterator.h"

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

iterator_t estd_iter_map(iterator_t *iter, void* (*builder)(void *), size_t new_item_size){
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

    return new_iter;
}
