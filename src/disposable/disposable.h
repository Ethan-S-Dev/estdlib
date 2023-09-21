#ifndef DISPOSABLE_H
#define DISPOSABLE_H

#include <stdbool.h>

#define using(disposable) for(bool once = true;once;once = (estd_disposable_dispose(disposable), false))

typedef struct disposable_f{
    void (*dispose)(void* self);
} disposable_f;

typedef struct disposable{
    void* self;
    disposable_f const* disposable_f;
} disposable;

#define implement_disposable(T, Name, dispose_f)                                            \
    disposable Name(T* x)                                                                   \
    {                                                                                       \
        void (*const dispose_)(T* e) = (dispose_f);                                         \
        (void)dispose_;                                                                     \
                                                                                            \
        static disposable_f const functions_ ={                                             \
                                        .dispose = (void (*const)(void* self))(dispose_f)   \
                                    };                                                      \
        return (disposable){.disposable_f = &functions_, .self = x};                        \
    }

void estd_disposable_dispose(disposable self);
#endif