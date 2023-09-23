#ifndef ESTD_H
#define ESTD_H

#include <stdio.h>

#define ESTD_VERSION "0.0.1"

#ifndef ESTD_MALLOC
#define ESTD_MALLOC malloc
#endif
#ifndef ESTD_FREE
#define ESTD_FREE free
#endif
#ifndef ESTD_REALLOC
#define ESTD_REALLOC realloc
#endif
#ifndef ESTD_CALLOC
#define ESTD_CALLOC calloc
#endif

#endif // ESTD_H
