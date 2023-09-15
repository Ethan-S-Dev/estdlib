#ifndef MAP_FILTER_H
#define MAP_FILTER_H

#include "../iterator.h"

iterator_t estd_iter_map(iterator_t* iter, void* (*builder)(void* item), size_t new_item_size);
iterator_t estd_iter_map_indexed(iterator_t* iter, void* (*builder)(void* item, size_t index), size_t new_item_size);
iterator_t estd_iter_map_args(iterator_t* iter, void* (*builder)(void* args, void* item), void* args, size_t new_item_size);
iterator_t estd_iter_map_indexed_args(iterator_t* iter, void* (*builder)(void* args, void* item, size_t index), void* args, size_t new_item_size);

#endif
