#include "list.h"
#include <string.h>

#define list_header(list) ((_list_header*)(list) - 1)
#define list_items(list) ((byte*)(_list_header*)(list) + 1)

typedef struct _list_header{
    size_t item_size;
    size_t length;
    size_t capacity;
} _list_header;

list_t* estd_list_create(size_t capacity, size_t item_size){
    if(capacity == 0)
        capacity = LIST_INIT_CAPACITY;

    _list_header* header = ESTD_MALLOC(sizeof(_list_header) + (capacity* item_size));
    header->capacity = capacity;
    header->item_size = item_size;
    header->length = 0;
    return list_items(header);
}

void estd_list_destroy(list_t* list){
    _list_header* header = list_header(list);
    ESTD_FREE(header);
}

bool estd_list_grow(list_t** list_p, size_t size){
    _list_header* header = list_header(*list_p);

    if(header->capacity > header->length)
        return true;

    size_t needed_capacity = header->length + size;

    if(needed_capacity <= LIST_GROWTH_FACTOR * header->capacity)
        needed_capacity = LIST_GROWTH_FACTOR * header->capacity;
    else if(needed_capacity < LIST_INIT_CAPACITY)
        needed_capacity = LIST_INIT_CAPACITY;

    _list_header* new_header = ESTD_REALLOC(header, needed_capacity);

    if(new_header == NULL)
        return false;

    ESTD_FREE(header);
    *list_p = list_items(new_header);

    return true;
}

bool estd_list_remove(list_t* list, size_t index){
    _list_header* header = list_header(list);
    byte* _list = list_items(header);

    if(header->length <= index)
        return false;

    size_t list_bytes_size = (header->length * header->item_size);
    size_t item_bytes_index = (index * header->item_size);
    size_t bytes_to_move = list_bytes_size - item_bytes_index;
    size_t next_item_bytes_index = item_bytes_index + header->item_size;

    for (size_t i = next_item_bytes_index; i < list_bytes_size; i++)
       _list[i - 1] = _list[i];
    
    header->length--;
    return true;
}

size_t estd_list_len(list_t* list){
    _list_header* header = list_header(list);
    return header->length;
}
