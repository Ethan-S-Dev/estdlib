#ifndef ITERABLE_H
#define ITERABLE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "iterator.h"

typedef struct iterable_f{
    iterator (*get_iterator)(void* self);
} iterable_f;

typedef struct iterable{
    void* self;
    iterable_f const* iterable_f;
} iterable;

#define iteration_data(type, varName) struct { type varName; bool has_value; size_t index; iterator iter; }
#define iteration_data_void(varName) struct { void* varName; bool has_value; size_t index; iterator iter; }
                                                                                      
#define foreach(type, varName, iterable)                                            \
for (                                                                               \
    iteration_data(type, varName) iteration = {                                     \
        .varName = (type){0},                                                       \
        .has_value = false,                                                         \
        .index = 0,                                                                 \
        .iter = estd_iterable_get_iterator(iterable)                                \
    };                                                                              \
    (                                                                               \
        iteration.has_value = estd_iterator_move_next(iteration.iter),              \
        iteration.has_value                                                         \
    ) &&                                                                            \
    (                                                                               \
        iteration.varName = *(type*)estd_iterator_current(iteration.iter),          \
        true                                                                        \
    );                                                                              \
    iteration.index = (iteration.has_value == false) ?                              \
    (                                                                               \
        estd_disposable_dispose(estd_iterator_get_disposable(iteration.iter)),      \
        0                                                                           \
    ) :                                                                             \
    iteration.index + 1                                                             \
) 

#define foreach_pointer(varName, iterable)                                          \
for (                                                                               \
    iteration_data_void(varName) iteration = {                                      \
            .varName = (void*){0},                                                  \
            .has_value = false,                                                     \
            .index = 0,                                                             \
            .iter = estd_iterable_get_iterator(iterable)                            \
    };                                                                              \
    (                                                                               \
        iteration.has_value = estd_iterator_move_next(iteration.iter),              \
        iteration.has_value                                                         \
    ) &&                                                                            \
    (                                                                               \
        iteration.varName = estd_iterator_current(iteration.iter),                  \
        true                                                                        \
    );                                                                              \
    iteration.index = (iteration.has_value == false) ?                              \
    (                                                                               \
        estd_disposable_dispose(estd_iterator_get_disposable(iteration.iter)),      \
        0                                                                           \
    ) :                                                                             \
    iteration.index + 1                                                             \
)    
    
#define implement_iterable(T, Name, get_iterator_f)                                                     \
iterable Name(T* x)                                                                                     \
{                                                                                                       \
    iterator (*const get_iterator_)(T* e) = (get_iterator_f);                                           \
    (void)get_iterator_;                                                                                \
                                                                                                        \
    static iterable_f const functions_ ={                                                               \
                                    .get_iterator = (iterator (*const)(void* self))(get_iterator_f)     \
                                };                                                                      \
    return (iterable){.iterable_f = &functions_, .self = x};                                            \
}

iterator estd_iterable_get_iterator(iterable iterable);
iterable estd_iterator_as_iterable(iterator iterator);
#endif