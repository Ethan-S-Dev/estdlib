#include "list.h"
#include <string.h>

typedef char byte;

typedef struct list_t{
    size_t item_size;
    size_t length;
    size_t capacity;
    void* items;
} list_t;

list_t* estd_list_create_cap(size_t capacity, size_t item_size){
    
        capacity = LIST_INIT_CAPACITY;

    list_t* list = ESTD_MALLOC(sizeof(list_t));
    list->capacity = capacity;
    list->item_size = item_size;
    list->length = 0;

    if(capacity > 0)
        list->items = ESTD_MALLOC(capacity * item_size);
    else
        list->items = NULL;

    return list;
}
list_t* estd_list_create(size_t item_size){
    return estd_list_create_cap(LIST_INIT_CAPACITY, item_size);
}

static void list_add_item(void* list_p, void* item){
    list_t* list = (list_t*)list_p;
    estd_list_add(list, item);
}

list_t* estd_list_create_iter(iterator_t* iterator){
    list_t* list = iterator->count == NULL ? 
                    estd_list_create(iterator->item_size) :
                    estd_list_create_cap(estd_iter_count(iterator), iterator->item_size);

    estd_iter_for_each_args(iterator, list_add_item, list);
    return list;
}

void estd_list_destroy(list_t* list){
    ESTD_FREE(list->items);
    ESTD_FREE(list);
}

static bool list_grow_if_needed(list_t* list, size_t size){
    if(list->capacity > list->length)
        return true;

    size_t needed_capacity = list->length + size;

    if(needed_capacity <= LIST_GROWTH_FACTOR * list->capacity)
        needed_capacity = LIST_GROWTH_FACTOR * list->capacity;
    else if(needed_capacity < LIST_INIT_CAPACITY)
        needed_capacity = LIST_INIT_CAPACITY;

    void* new_items = ESTD_REALLOC(list->items, needed_capacity* list->item_size);

    if(new_items == NULL)
        return false;

    list->items = new_items;
    list->capacity = needed_capacity;
    return true;
}

bool estd_list_add(list_t* list, void* item){
    if(!list_grow_if_needed(list, 1))
        return false;

    byte* items_bytes = (byte*)list->items;
    size_t offset = list->length * list->item_size;
    memcpy(items_bytes + offset, item, list->item_size);
    list->length++;
    return true;
}

void* estd_list_get(list_t* list, size_t index){
    if(index >= list->length)
        return NULL;

    byte* items_bytes = (byte*)list->items;
    size_t offset = index * list->item_size;
    return items_bytes + offset;
}

bool estd_list_remove(list_t* list, size_t index){
    if(index >= list->length)
        return false;

    byte* items_bytes = (byte*)list->items;
    size_t offset = index * list->item_size;
    byte* dest = items_bytes + offset;
    byte* src = items_bytes + offset + list->item_size;
    size_t size = (list->length - index - 1) * list->item_size;
    memmove(dest, src, size);
    list->length--;
    return true;
}

size_t estd_list_len(list_t* list){
    return list->length;
}

typedef struct list_iterator_t {
    list_t* list;
    size_t current_index;
} list_iterator_t;

static bool list_iterator_move_next(iterator_t* iterator){
    list_iterator_t* list_iterator = (list_iterator_t*)iterator->internal_iterator;
    list_iterator->current_index++;
    return list_iterator->current_index < list_iterator->list->length;
}

static void* list_iterator_current(iterator_t* iterator){
    list_iterator_t* list_iterator = (list_iterator_t*)iterator->internal_iterator;
    return estd_list_get(list_iterator->list, list_iterator->current_index);
}

static void list_iterator_reset(iterator_t* iterator){
    list_iterator_t* list_iterator = (list_iterator_t*)iterator->internal_iterator;
    list_iterator->current_index = 0;
}

static size_t list_iterator_count(iterator_t* iterator){
    list_iterator_t* list_iterator = (list_iterator_t*)iterator->internal_iterator;
    return list_iterator->list->length;
}

static void list_iterator_dispose(iterator_t* iterator){
    list_iterator_t *list_iter = (list_iterator_t *)iterator->internal_iterator;
    ESTD_FREE(list_iter);
}

iterator_t estd_list_iterator(list_t* list){
    list_iterator_t* list_iterator = ESTD_MALLOC(sizeof(list_iterator_t));
    list_iterator->list = list;
    list_iterator->current_index = 0;

    iterator_t iterator = {
        .internal_iterator = list_iterator,
        .item_size = list->item_size,
        .move_next = list_iterator_move_next,
        .current = list_iterator_current,
        .reset = list_iterator_reset,
        .count = list_iterator_count,
        .dispose = list_iterator_dispose
    };

    return iterator;
}
