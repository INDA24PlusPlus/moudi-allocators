#include "alloc.h"

#ifdef POOL_ALLOCATOR
#include "pool.h"
#endif
#ifdef MY_ALLOCATOR
#include "my_allocator.h"
#endif

void * malloc(size_t size) {
#ifdef MY_ALLOCATOR
    return my_alloc_malloc(size);
#endif
    return NULL;
}

void * calloc(size_t n, size_t size) {
#ifdef MY_ALLOCATOR
    return my_alloc_calloc(n, size);
#endif
    return NULL;
}

void * realloc(void * ptr, size_t new_size) {
#ifdef MY_ALLOCATOR
    return my_alloc_realloc(ptr, new_size);
#endif
    return NULL;
}

void free(void * ptr) {
#ifdef MY_ALLOCATOR
    my_alloc_free(ptr);
#endif
}
