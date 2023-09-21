#include <assert.h>
#include "iterable.h"

iterator estd_iterable_get_iterator(iterable iter){
    assert(iter.self != NULL);
    assert(iter.iterable_f->get_iterator != NULL);
    return iter.iterable_f->get_iterator(iter.self);
}