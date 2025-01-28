#pragma once
#include <stddef.h>

void free(void * ptr);

void * alloc(size_t size);

void * resize(void ** ptr, size_t new_size);
