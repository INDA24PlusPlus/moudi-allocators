#pragma once
#include <unistd.h>

void my_alloc_print_blocks();

void * my_alloc_malloc(size_t size);

void * my_alloc_calloc(size_t n, size_t size);

void * my_alloc_realloc(void * ptr, size_t new_size);

void my_alloc_free(void * ptr);
