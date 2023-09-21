#include <assert.h>
#include "iterator.h"

bool estd_iterator_move_next(iterator iter){
    assert(iter.self != NULL);
    assert(iter.iterator_f->move_next != NULL);
    bool result = iter.iterator_f->move_next(iter.self);
    return result;
}
void* estd_iterator_current(iterator iter){
    assert(iter.self != NULL);
    assert(iter.iterator_f->get_current != NULL);
    void* current = iter.iterator_f->get_current(iter.self);
    return current;
}
void estd_iterator_reset(iterator iter){
    assert(iter.self != NULL);
    assert(iter.iterator_f->reset != NULL);
    iter.iterator_f->reset(iter.self);
}
disposable estd_iterator_get_disposable(iterator iter){
    assert(iter.self != NULL);
    assert(iter.iterator_f->get_disposable != NULL);
    disposable disposable = iter.iterator_f->get_disposable(iter.self);
    return disposable;
}