#include "alloc.h"
#include "arena.h"
#include <dlfcn.h>

void ** malloc(size_t size) {
#ifdef ARENA_ALLOCATOR
    return arena_malloc(size);
#endif
}

void ** calloc(size_t n, size_t size) {
#ifdef ARENA_ALLOCATOR
    return arena_calloc(n, size);
#endif
}

void ** realloc(void ** ptr, size_t new_size) {
#ifdef ARENA_ALLOCATOR
    return arena_realloc(ptr, new_size);
#endif
}

void free(void ** ptr) {
#ifdef ARENA_ALLOCATOR
    arena_free(ptr);
#endif
}
