#include <assert.h>
#include "disposable.h"

void estd_disposable_dispose(disposable disposable){
    assert(disposable.self != NULL && "Diposable with NULL reference!");
    assert(disposable.disposable_f != NULL && "Disposable with NULL vTable!");
    assert(disposable.disposable_f->dispose != NULL && "Disposable with NULL dispose function pointer!");
    disposable.disposable_f->dispose(disposable.self);
}
