#include <assert.h>
#include "disposable.h"

void estd_disposable_dispose(disposable disposable){
    assert(disposable.self != NULL);
    assert(disposable.disposable_f->dispose != NULL);
    disposable.disposable_f->dispose(disposable.self);
}