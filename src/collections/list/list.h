#ifndef LIST_H
#define LIST_H
#include <stdbool.h>
#include <stdlib.h>
#include "../iterable/iterable.h"

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

#define LIST_INIT_CAPACITY 4
#define LIST_GROWTH_FACTOR 2
#define LIST_SHRINK_FACTOR 0.25

#define list_add(type, list, item) (bool)estd_list_add((list), (type*)(type[1]){(item)})
#define list_items(type, list) ((type*)estd_list_get((list), 0))
#define list_len estd_list_len

typedef struct estd_list estd_list;

estd_list* estd_list_create(size_t item_size);
estd_list* estd_list_create_cap(size_t capacity, size_t item_size);
estd_list* estd_list_create_iterable(iterable iter, size_t item_size);
void estd_list_destroy(estd_list* list);
bool estd_list_add(estd_list* list, void* item);
void* estd_list_get(estd_list* list, size_t index);
bool estd_list_remove(estd_list* list, size_t index);
size_t estd_list_len(estd_list* list);
iterable estd_list_iterable(estd_list* list);
#endif
