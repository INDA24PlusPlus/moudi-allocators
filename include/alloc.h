#pragma once
#include <stddef.h>
#include <unistd.h>
#include <dlfcn.h>

#define ARENA_ALLOCATOR

#ifdef ARENA_ALLOCATOR
#include "arena.h"
#endif

#ifdef MY_ALLOCATOR
#include "my_allocator.h"
#endif

void ** malloc(size_t size);
void ** calloc(size_t n, size_t size);
void ** realloc(void ** ptr, size_t new_size);
void free(void ** ptr);
