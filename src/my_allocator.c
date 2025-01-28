#include "alloc.h"

#ifdef MY_ALLOCATOR

void free(void * ptr) {

}

void * alloc(size_t size) {
    return NULL;
}

void * resize(void ** ptr, size_t new_size) {
    return NULL;
}

#endif
