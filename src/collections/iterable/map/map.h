#ifndef MAP_ITERATOR_H
#define MAP_ITERATOR_H

#include "../iterator.h"

#ifndef ESTD_MALLOC
#define ESTD_MALLOC malloc
#endif
#ifndef ESTD_FREE
#define ESTD_FREE free
#endif
#ifndef ESTD_REALLOC
#define ESTD_REALLOC realloc
#endif
#ifndef ESTD_CALLOC
#define ESTD_CALLOC calloc
#endif

iterator estd_map_iterator(iterator enumerable, void* (*mapper)(void* item));
#endif
