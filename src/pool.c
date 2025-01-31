#include "pool.h"
#include "alloc.h"
#include "fmt.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

const size_t START_ARENA_SIZE = 512;
const size_t BLOCK_DATA_SIZE = 32;
#define ARENA_CAPACITY (pool.occupied_blocks + arena.free_blocks)

const size_t GROWTH_SPEED = 2;
const size_t init_size = 50;

Pool pool_init() {
    Pool pool = {0};

    pool.mapped_size = START_ARENA_SIZE;
    pool.head = pool.start = sbrk(0);
    sbrk(pool.mapped_size);
    pool.end_of_mapping = sbrk(0);

    pool.block_count = 0;

    return pool;
}

void pool_grow(Pool * pool) {
    do {
        pool->mapped_size += pool->mapped_size;
        sbrk(pool->mapped_size);
        pool->end_of_mapping = sbrk(0);
    } while (pool->end_of_mapping <= (void*) pool->head);
}


void * pool_malloc(Pool * pool, size_t size) {
    if (size == 0 || pool == NULL || pool->head == NULL) {
        return NULL;
    }

    pool->block_count += 1;

    _pool_block * block = pool->head;
    pool->head += size + offsetof(_pool_block, data);

    if (pool->end_of_mapping <= (void*) pool->head) {
        pool_grow(pool);
    }

    return &block->data;
}

void * pool_calloc(Pool * pool, size_t n, size_t el_size) {
    const size_t size = n * el_size;
    void * ptr = pool_malloc(pool, size);

    memset(ptr, 0, size);

    return ptr;
}

void * pool_realloc(Pool * pool, void * ptr, size_t new_size) {
    _pool_block * block = ptr - offsetof(_pool_block, data);
    const size_t size = (void*) block->next - ptr;

    void * new = pool_malloc(pool, new_size);
    memcpy(new, ptr, size);

    return new;
}


void pool_free(Pool * pool) {
    pool->head = pool->start;
}
