#ifndef FILTER_ITERATOR_H
#define FILTER_ITERATOR_H
#include "../iterator.h"

iterator_t estd_iter_filter(iterator_t* iter, bool (*predicate)(void* item));
iterator_t estd_iter_filter_args(iterator_t* iter, bool (*predicate)(void* args, void* item), void* args);
iterator_t estd_iter_filter_indexed(iterator_t* iter, bool (*predicate)(void* item, size_t index));
iterator_t estd_iter_filter_indexed_args(iterator_t* iter, bool (*predicate)(void* args, void* item, size_t index), void* args);

#endif
