#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>
#include <stdlib.h>

#ifndef ESTD_FREE
#define ESTD_FREE(ptr) free(ptr)
#endif
#ifndef ESTD_MALLOC
#define ESTD_MALLOC(size) malloc(size)
#endif

typedef struct iterator_t {
    void* internal_iterator;
    size_t item_size;

    bool (*move_next)(struct iterator_t* self);
    void* (*current)(struct iterator_t* self);
    void (*reset)(struct iterator_t* self);
    size_t (*count)(struct iterator_t* self);
    void (*dispose)(struct iterator_t* self);
} iterator_t;

size_t estd_iter_count(iterator_t* iter);
void estd_iter_dispose(iterator_t* iter);
void estd_iter_for_each(iterator_t* iter, void (*action)(void*));
void estd_iter_for_each_args(iterator_t* iter, void (*action)(void* args, void* item), void* args);
void estd_iter_for_each_indexed(iterator_t* iter, void (*action)(void* item, size_t index));
void estd_iter_for_each_indexed_args(iterator_t* iter, void (*action)(void* args, void* item, size_t index), void* args);
iterator_t estd_iter_filter(iterator_t* iter, bool (*predicate)(void* item));
iterator_t estd_iter_filter_indexed(iterator_t* iter, bool (*predicate)(void* item, size_t index));
iterator_t estd_iter_filter_args(iterator_t* iter, bool (*predicate)(void* args, void* item), void* args);
iterator_t estd_iter_filter_indexed_args(iterator_t* iter, bool (*predicate)(void* args, void* item, size_t index), void* args);
iterator_t estd_iter_map(iterator_t* iter, void* (*builder)(void* item), size_t new_item_size);
iterator_t estd_iter_map_indexed(iterator_t* iter, void* (*builder)(void* item, size_t index), size_t new_item_size);
iterator_t estd_iter_map_args(iterator_t* iter, void* (*builder)(void* args, void* item), void* args, size_t new_item_size);
iterator_t estd_iter_map_indexed_args(iterator_t* iter, void* (*builder)(void* args, void* item, size_t index), void* args, size_t new_item_size);
#endif