#include "list.h"
#include <string.h>

#define list_head(list) ((_list_head*) ((byte*)(list) - sizeof(_list_head)))
#define list_bytes(list) ((byte*)(list) + sizeof(_list_head));

typedef struct _list_header{
    size_t item_size;
    size_t length;
    size_t capacity;
    void* head;
} _list_header;

typedef struct _list_head{
    _list_header* header;
} _list_head;

list_t* estd_list_create(size_t capacity, size_t item_size){
    if(capacity == 0)
        capacity = LIST_INIT_CAPACITY;

    _list_header* header = ESTD_MALLOC(sizeof(_list_header));
    _list_head* head = ESTD_MALLOC(sizeof(_list_head) + (capacity* item_size));
    header->capacity = capacity;
    header->item_size = item_size;
    header->length = 0;
    header->head = head;
    head->header = header;

    return list_bytes(head);
}

void estd_list_destroy(list_t* list){
    _list_head* head = list_head(list);
    _list_header* header = head->header;

    ESTD_FREE(head);
    ESTD_FREE(header);
}

bool estd_list_add(list_t* list, byte* item){
    _list_head* head = list_head(list);
    _list_header* header = head->header;

    if(header->capacity <= header->length){
        size_t new_capacity = header->capacity * LIST_GROWTH_FACTOR;
        _list_head* new_head = ESTD_REALLOC(header->head, sizeof(_list_head) + (new_capacity * header->item_size));

        if(new_head == NULL)
            return false;

        header->head = new_head;
        header->capacity = new_capacity;
    }

    byte* _list = list_bytes(header->head);
    size_t list_end_bytes_index = (header->length * header->item_size);
    memcpy(_list + list_end_bytes_index, item, header->item_size);
    header->length++;
    return true;
}

bool estd_list_remove(list_t* list, size_t index){
    _list_head* head = list_head(list);
    _list_header* header = head->header;

    if(header->length <= index)
        return false;
    
    byte* _list = list_bytes(head);

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
    _list_head* head = list_head(list);
    _list_header* header = head->header;
    return header->length;
}
