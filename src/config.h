#ifndef RAYLIB_TEXT_BOX_CONFIG_H
#define RAYLIB_TEXT_BOX_CONFIG_H


#ifndef UPTR
#define UPTR(type) type *
#endif


#ifndef size_t
#include <stdint.h>
#endif


#ifndef RAYLIB_TEXT_BOX_ALLOC
#include <stdlib.h>
#define RAYLIB_TEXT_BOX_ALLOC(type, n) \
    (type *) malloc(sizeof(type) * ((size_t) n))
#endif
#ifndef RAYLIB_TEXT_BOX_REALLOC
#include <stdlib.h>
#define RAYLIB_TEXT_BOX_REALLOC(ptr, type, n) \
    (type *) realloc(ptr, sizeof(type) * ((size_t) n))
#endif
#ifndef RAYLIB_TEXT_BOX_FREE
#include <stdlib.h>
#define RAYLIB_TEXT_BOX_FREE(ptr) \
    if ( (ptr) != NULL) free(ptr)
#endif


#endif
