#pragma once
#include <unistd.h>

#define POOL_ALLOCATOR
#define offsetof(T, el) ((size_t) &((T *)0)->el)

#ifdef POOL_ALLOCATOR
#include "pool.h"
#endif

#ifdef MY_ALLOCATOR
#include "my_allocator.h"
#endif

void * malloc(size_t size);
void * calloc(size_t n, size_t size);
void * realloc(void * ptr, size_t new_size);
void free(void * ptr);
