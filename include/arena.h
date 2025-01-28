#pragma once
#include <stddef.h>

void arena_free(void ** ptr);

void ** arena_malloc(size_t size);
void ** arena_calloc(size_t n, size_t size);

void ** arena_realloc(void ** ptr, size_t new_size);
