#include "alloc.h"
#include "fmt.h"

typedef struct _arena_block {
    void * ptr;
} _arena_block;

typedef struct _arena_info {
    _arena_block * start;
    size_t blocks;
    size_t free_index;
    const size_t block_size;
} _arena_info;

const size_t growing_speed = 2;
const size_t init_size = 50;

_arena_info arena = {
    .block_size = sizeof(_arena_block),
    .start = NULL,
};

void arena_init() {
    arena.start = sbrk(0);
    arena.free_index = 0;
    arena.blocks = 0;
}

void ** arena_malloc(size_t size) {
    if (arena.start == NULL) {
        arena_init();
    }

    println("arena_malloc has not been implemented");
    return NULL;
}

void ** arena_calloc(size_t n, size_t size) {
    println("arena_calloc has not been implemented");
    return NULL;
}

void ** arena_realloc(void ** ptr, size_t new_size) {
    println("arena_realloc has not been implemented");
    return NULL;
}


void arena_free(void ** ptr) {
    println("arena_free has not been implemented");
}
