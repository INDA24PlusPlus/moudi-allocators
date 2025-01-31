#pragma once
#include <unistd.h>

typedef struct _pool_block {
    struct _pool_block * next;
    char data[];
} _pool_block;

typedef struct _pool_info {
    _pool_block * head;        // the start of the pool
    void * start;
    void * end_of_mapping;      // The end of the available memory

    size_t mapped_size;
    size_t block_count;         // The number of blocks used

    const size_t block_size;    // the amount of bytes that a block is said to be
} Pool;

Pool pool_init();

void pool_free(Pool * pool);

void * pool_malloc(Pool * pool, size_t size);
void * pool_calloc(Pool * pool, size_t n, size_t size);

void * pool_realloc(Pool * pool, void * ptr, size_t new_size);
