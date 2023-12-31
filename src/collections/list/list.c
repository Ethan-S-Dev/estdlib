#include <string.h>
#include <assert.h>
#include "list.h"

typedef char byte;

typedef struct estd_list{
    size_t item_size;
    size_t length;
    size_t capacity;
    void* items;
} estd_list;

estd_list* estd_list_create(size_t item_size){
    return estd_list_create_cap(LIST_INIT_CAPACITY, item_size);
}
estd_list* estd_list_create_cap(size_t capacity, size_t item_size){
    capacity = LIST_INIT_CAPACITY;

    estd_list* list = ESTD_MALLOC(sizeof(estd_list));
    if(list == NULL) return NULL;

    list->capacity = capacity;
    list->item_size = item_size;
    list->length = 0;

    if(capacity > 0){
        list->items = ESTD_MALLOC(capacity * item_size);
        if(list->items == NULL){
            ESTD_FREE(list);
            return NULL;
        }
    }
    else
        list->items = NULL;

    return list;
}
estd_list* estd_list_create_iterable(iterable iter, size_t item_size){
    estd_list* list = estd_list_create(item_size);
    if(list == NULL) return NULL;

    foreach_pointer(item, iter)
    {
        if(!estd_list_add(list, iteration.item)){
            estd_list_destroy(list);
            return NULL;
        }
    }

    return list;
}

void estd_list_destroy(estd_list* list){
    ESTD_FREE(list->items);
    ESTD_FREE(list);
}

static bool list_grow_if_needed(estd_list* list, size_t size){
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

bool estd_list_add(estd_list* list, void* item){
    if(!list_grow_if_needed(list, 1))
        return false;

    byte* items_bytes = (byte*)list->items;
    size_t offset = list->length * list->item_size;
    memcpy(items_bytes + offset, item, list->item_size);
    list->length++;
    return true;
}
void* estd_list_get(estd_list* list, size_t index){
    assert(index < list->length);

    byte* items_bytes = (byte*)list->items;
    size_t offset = index * list->item_size;
    return items_bytes + offset;
}
bool estd_list_remove(estd_list* list, size_t index){
    assert(index < list->length);

    byte* items_bytes = (byte*)list->items;
    size_t offset = index * list->item_size;
    byte* dest = items_bytes + offset;
    byte* src = items_bytes + offset + list->item_size;
    size_t size = (list->length - index - 1) * list->item_size;
    memmove(dest, src, size);
    list->length--;
    return true;
}
size_t estd_list_len(estd_list* list){
    return list->length;
}

typedef struct estd_list_iterator_t {
    estd_list* list;
    size_t current_index;
} estd_list_iterator_t;

static bool list_iterator_move_next(estd_list_iterator_t* self){
    if(self->current_index == self->list->length)
        return false;

    self->current_index++;
    return self->current_index < self->list->length;
}
static void* list_iterator_get_current(estd_list_iterator_t* self){
    return estd_list_get(self->list, self->current_index);
}
static void list_iterator_reset(estd_list_iterator_t* self){
    self->current_index = -1;
}
static void list_iterator_disposable_dispose(estd_list_iterator_t* self){
    ESTD_FREE(self);
}

static implement_disposable(estd_list_iterator_t, estd_list_iterator_disposable, list_iterator_disposable_dispose);

static implement_iterator(estd_list_iterator_t, estd_list_iterator_iterator, list_iterator_get_current, list_iterator_move_next, list_iterator_reset, estd_list_iterator_disposable);

static iterator estd_list_get_iterator(estd_list* list){
    estd_list_iterator_t* iter = ESTD_MALLOC(sizeof(estd_list_iterator_t));
    if(iter == NULL) return (iterator){0};
    iter->current_index = -1;
    iter->list = list;
    return estd_list_iterator_iterator(iter);
}

implement_iterable(estd_list, estd_list_iterable, estd_list_get_iterator);