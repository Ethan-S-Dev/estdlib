#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>
#include <stdlib.h>
#include "../../disposable/disposable.h"

typedef struct iterator_f{
    disposable (*get_disposable)(void* self);
    void* (*get_current)(void* self);

    bool (*move_next)(void* self);
    void (*reset)(void* self);
} iterator_f;

typedef struct iterator{
    void* self;
    iterator* inner_iterator;
    iterator_f const* iterator_f;
} iterator;

#define implement_iterator(T, Name, get_current_f, move_next_f, reset_f, get_disposable_f)                  \
iterator Name(T* x)                                                                                         \
{                                                                                                           \
    disposable (*const get_disposable_)(T* e) = (get_disposable_f);                                         \
    (void)get_disposable_;                                                                                  \
    void* (*const get_current_)(T* e) = (get_current_f);                                                    \
    (void)get_current_;                                                                                     \
                                                                                                            \
    bool (*const move_next_)(T* e) = (move_next_f);                                                         \
    (void)move_next_;                                                                                       \
    void (*const reset_)(T* e) = (reset_f);                                                                 \
    (void)reset_;                                                                                           \
                                                                                                            \
    static iterator_f const functions_ ={                                                                   \
                                    .get_disposable = (disposable (*const)(void* self))(get_disposable_f),  \
                                    .get_current = (void* (*const)(void* self))(get_current_f),             \
                                                                                                            \
                                    .move_next = (bool (*const)(void* self))(move_next_f),                  \
                                    .reset = (void (*const)(void* self))(reset_f)                           \
                                };                                                                          \
    return (iterator){.iterator_f = &functions_, .self = x};                                                \
}

bool estd_iterator_move_next(iterator iter);
void* estd_iterator_current(iterator iter);
void estd_iterator_reset(iterator iter);
disposable estd_iterator_get_disposable(iterator iter);
#endif
