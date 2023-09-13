#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdbool.h>

#define ESTD_MALLOC malloc
#define ESTD_FREE free
#define ESTD_REALLOC realloc
#define ESTD_CALLOC calloc

#define LIST_INIT_CAPACITY 4
#define LIST_GROWTH_FACTOR 2
#define LIST_SHRINK_FACTOR 0.25

#define list_create_cap(type, capacity) ((type *)estd_list_create((capacity), sizeof(type)))
#define list_create(type) ((type *)estd_list_create(LIST_INIT_CAPACITY, sizeof(type)))
#define list_destroy(list) estd_list_destroy(((list_t*)(list)))
#define list_add(list, item) ((bool)estd_list_add(((list_t*)(list)), ((byte*)&(item))))
#define list_remove(list, index) ((bool)estd_list_remove(((list_t*)(list)), (index)))
#define list_len(list) ((size_t)estd_list_len((list_t*)(list)))

typedef void list_t;
typedef char byte;

list_t* estd_list_create(size_t capacity, size_t item_size);
void estd_list_destroy(list_t* list);
bool estd_list_add(list_t* list, byte* item);
bool estd_list_remove(list_t* list, size_t index);
size_t estd_list_len(list_t* list);

#endif
