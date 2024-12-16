#ifndef RAYLIB_TEXT_BOX_PROTECTED_H
#define RAYLIB_TEXT_BOX_PROTECTED_H


#include "./config.h"


#define ALLOC(type, n) RAYLIB_TEXT_BOX_ALLOC(type, n)
#define REALLOC(ptr, type, n) RAYLIB_TEXT_BOX_REALLOC(ptr, type, n)
#define FREE(ptr) RAYLIB_TEXT_BOX_FREE(ptr)


#ifndef MAX
#define MAX(a, b) (a < b ? b : a)
#endif


#endif
