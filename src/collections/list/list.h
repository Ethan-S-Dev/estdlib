#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "../iterator/iterator.h"

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

#define list_add(type, list, item)      do{ type value = (item); estd_list_add((list), &value); }while(0)
#define list_items(type, list)          ((type*)estd_list_get((list), 0))
#define list_len(list)                  ((size_t)estd_list_len((list)))  

typedef struct list_t list_t;

list_t* estd_list_create_cap(size_t capacity, size_t item_size);
list_t* estd_list_create(size_t item_size);
list_t* estd_list_create_iter(iterator_t* iterator);
void estd_list_destroy(list_t* list);
bool estd_list_add(list_t* list, void* item);
void* estd_list_get(list_t* list, size_t index);
bool estd_list_remove(list_t* list, size_t index);
size_t estd_list_len(list_t* list);
iterator_t estd_list_iterator(list_t* list);
#endif
