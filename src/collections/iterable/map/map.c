#include "map.h"
#include <assert.h>

typedef struct map_iterator_t{
    iterator source_iterator;
    void* latest_item;
    void* (*mapper)(void *);
} map_iterator_t;

static void clear_latest_item(map_iterator_t* self){
    if(self->latest_item != NULL)
    {
        ESTD_FREE(self->latest_item);
        self->latest_item = NULL;
    }
}
static void set_latest_item(map_iterator_t* self, void* item){
    clear_latest_item(self);
    self->latest_item = item;
}
static void map_iterator_reset(map_iterator_t *self){
    clean_latest_item(self);
    estd_iterator_reset(self->source_iterator);
}
static bool map_iterator_move_next(map_iterator_t *self){
    return estd_iterator_move_next(self->source_iterator);
}
static void* map_iterator_get_current(map_iterator_t *self){
    void* item = self->mapper(estd_iterator_current(self->source_iterator));
    set_latest_item(self, item);
    return item;
}

static void map_disposable_dispose(map_iterator_t *self){
    clean_latest_item(self);
    estd_disposable_dispose(estd_iterator_get_disposable(self->source_iterator));
    ESTD_FREE(self);
}

implement_disposable(map_iterator_t, estd_map_iterator_get_disposable, map_disposable_dispose)

implement_iterator(map_iterator_t, estd_map_iterator_iterator, map_iterator_get_current, map_iterator_move_next, map_iterator_reset, estd_map_iterator_get_disposable)

iterator estd_enumerable_map(iterator iter, void* (*mapper)(void* item)){
    map_iterator_t* map_enumerator = ESTD_MALLOC(sizeof(map_iterator_t));
    map_enumerator->source_iterator = iter;
    map_enumerator->mapper = mapper;

    return estd_map_iterator_iterator(map_enumerator);
}