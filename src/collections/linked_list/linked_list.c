#include <assert.h>
#include <string.h>
#include "linked_list.h"

typedef struct linked_list_node_t {
    void *data;
    struct linked_list_node_t *next;
    struct linked_list_node_t *prev;
} linked_list_node_t;

typedef struct linked_list_t {
    linked_list_node_t *head;
    linked_list_node_t *tail;
    size_t item_size;
    size_t length;
} linked_list_t;

linked_list_t* estd_linked_list_create(size_t item_size){
    linked_list_t *list = ESTD_MALLOC(sizeof(linked_list_t));
    if(list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->item_size = item_size;
    list->length = 0;
    return list;
}
void estd_linked_list_destroy(linked_list_t* list){
    linked_list_node_t *node = list->head;
    while(node != NULL){
        linked_list_node_t *next = node->next;
        ESTD_FREE(node->data);
        ESTD_FREE(node);
        node = next;
    }
    ESTD_FREE(list);
}

bool estd_linked_list_add_last(linked_list_t* list, void* item){
    linked_list_node_t *node = ESTD_MALLOC(sizeof(linked_list_node_t));
    if(node == NULL) return false;

    node->data = ESTD_MALLOC(list->item_size);
    if(node->data == NULL){
        ESTD_FREE(node);
        return false;
    }

    memcpy(node->data, item, list->item_size);
    node->next = NULL;

    node->prev = list->tail;
    if(list->tail != NULL){
        list->tail->next = node;
    }

    list->tail = node;
    if(list->head == NULL){
        list->head = node;
    }

    list->length++;
    return true;
}
bool estd_linked_list_add_first(linked_list_t* list, void* item){
    linked_list_node_t *node = ESTD_MALLOC(sizeof(linked_list_node_t));
    if(node == NULL) return false;

    node->data = ESTD_MALLOC(list->item_size);
    if(node->data == NULL){
        ESTD_FREE(node);
        return false;
    }

    memcpy(node->data, item, list->item_size);
    node->prev = NULL;

    node->next = list->head;
    if(list->head != NULL){
        list->head->prev = node;
    }

    list->head = node;
    if(list->tail == NULL){
        list->tail = node;
    }

    list->length++;
    return true;
}

static linked_list_node_t* linked_list_get_node(linked_list_t* list, size_t index){
    if(index >= list->length) return NULL;

    size_t distance_from_tail = list->length - index - 1;
    linked_list_node_t *current = NULL;

    if(distance_from_tail < index){
        current = list->tail;
        for(size_t i = 0; i < distance_from_tail; i++) current = current->prev;
        return current;
    }

    current = list->head;
    for(size_t i = 0; i < index; i++) current = current->next;
    return current;
}

bool estd_linked_list_add(linked_list_t* list, size_t index, void* item){
    if(index == 0){
        return estd_linked_list_add_first(list, item);
    }else if(index == list->length){
        return estd_linked_list_add_last(list, item);
    }else if(index > list->length){
        return false;
    }

    linked_list_node_t *node = ESTD_MALLOC(sizeof(linked_list_node_t));
    if(node == NULL) return false;

    node->data = ESTD_MALLOC(list->item_size);
    if(node->data == NULL){
        ESTD_FREE(node);
        return false;
    }

    memcpy(node->data, item, list->item_size);

    linked_list_node_t *current = linked_list_get_node(list, index);

    node->prev = current->prev;
    node->next = current;
    current->prev->next = node;
    current->prev = node;

    list->length++;
    return true;
}

bool estd_linked_list_remove_last(linked_list_t* list){
    if(list->tail == NULL) return false;

    linked_list_node_t *node = list->tail;
    list->tail = node->prev;
    if(list->tail != NULL){
        list->tail->next = NULL;
    }

    ESTD_FREE(node->data);
    ESTD_FREE(node);

    list->length--;
    return true;
}
bool estd_linked_list_remove_first(linked_list_t* list){
    if(list->head == NULL) return false;

    linked_list_node_t *node = list->head;
    list->head = node->next;
    if(list->head != NULL){
        list->head->prev = NULL;
    }

    ESTD_FREE(node->data);
    ESTD_FREE(node);

    list->length--;
    return true;
}


bool estd_linked_list_remove(linked_list_t* list, size_t index){
    if(index == 0) return estd_linked_list_remove_first(list);
    if(index == list->length - 1) return estd_linked_list_remove_last(list);
    if(index >= list->length) return false;

    linked_list_node_t *current = linked_list_get_node(list, index);

    current->prev->next = current->next;
    current->next->prev = current->prev;

    ESTD_FREE(current->data);
    ESTD_FREE(current);

    list->length--;
    return true;
}

void* estd_linked_list_get(linked_list_t* list, size_t index){
    assert(index < list->length);

    linked_list_node_t *current = linked_list_get_node(list, index);

    return current->data;
}

typedef struct linked_list_iterator_t {
    linked_list_t *linked_list;
    linked_list_node_t *current;
} linked_list_iterator_t;

static bool linked_list_iterator_move_next(linked_list_iterator_t *self){
    if(self->current == NULL){
        self->current = self->linked_list->head;
        return self->current != NULL;
    }

    linked_list_node_t *next = self->current->next;
    if(next == NULL) return false;

    self->current = next;
    return true;
}
static void* linked_list_iterator_get_current(linked_list_iterator_t *self){
    return self->current->data;
}
static void linked_list_iterator_reset(linked_list_iterator_t *self){
    self->current = NULL;
}
static void linked_list_iterator_dispose(linked_list_iterator_t *self){
    ESTD_FREE(self);
}

static implement_disposable(linked_list_iterator_t, estd_linked_list_iterator_disposable, linked_list_iterator_dispose);

static implement_iterator(linked_list_iterator_t, estd_linked_list_iterator_iterator, linked_list_iterator_get_current, linked_list_iterator_move_next, linked_list_iterator_reset, estd_linked_list_iterator_disposable)

static iterator estd_linked_list_get_iterator(linked_list_t *list){
    linked_list_iterator_t *iterator = ESTD_MALLOC(sizeof(linked_list_iterator_t));
    iterator->linked_list = list;
    iterator->current = NULL;
    return estd_linked_list_iterator_iterator(iterator);
}

implement_iterable(linked_list_t, estd_linked_list_iterable, estd_linked_list_get_iterator);