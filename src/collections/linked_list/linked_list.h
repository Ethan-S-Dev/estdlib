#ifndef LINKED_LIST_H 
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "../iterator/iterator.h"

#ifndef ESTD_MALLOC
#define ESTD_MALLOC malloc
#endif
#ifndef ESTD_FREE
#define ESTD_FREE free
#endif

typedef struct linked_list_t linked_list_t;

linked_list_t* estd_linked_list_create(size_t item_size);
void estd_linked_list_destroy(linked_list_t* list);

bool estd_linked_list_add_last(linked_list_t* list, void* item);
bool estd_linked_list_add_first(linked_list_t* list, void* item);
bool estd_linked_list_add(linked_list_t* list, size_t index, void* item);

bool estd_linked_list_remove_last(linked_list_t* list);
bool estd_linked_list_remove_first(linked_list_t* list);
bool estd_linked_list_remove(linked_list_t* list, size_t index);

void* estd_linked_list_get(linked_list_t* list, size_t index);

iterator_t estd_linked_list_iter(linked_list_t* list);
#endif
