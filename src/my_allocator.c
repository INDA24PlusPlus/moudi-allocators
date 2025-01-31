#include "alloc.h"
#include "fmt.h"
#include <string.h>
#include <unistd.h>

#define START_ARENA_SIZE 100 // Number of blocks at start
#define BLOCK_ALIGNMENT 16 // Block alignment in bytes
#define GROWTH_SPEED 2

#define NOT_AVAILABLE 0
#define AVAILABLE 1
#define ARENA_CAPACITY (((size_t)my_alloc.end_of_heap - (size_t)my_alloc.start) / BLOCK_ALIGNMENT)
#define GET_BLOCK_AT_INDEX(index) ((_my_alloc_block *) (((size_t)my_alloc.start) + index * BLOCK_ALIGNMENT))
#define GET_BLOCK_SIZE(index) (((GET_BLOCK_AT_INDEX(index)->next_index) - index) * BLOCK_ALIGNMENT)
#define GET_BLOCK_AVAILABLE_SIZE(index) (GET_BLOCK_SIZE(index) - offsetof(_my_alloc_block, data))
#define IS_INBOUNDS(index) (index < ARENA_CAPACITY)

#define IS_FREE(block) ((block).is_available == AVAILABLE)

typedef struct _my_alloc_block {
    unsigned int prev_index, next_index;
    char is_available;
    char data[];
} _my_alloc_block;

typedef struct _my_alloc_info {
    _my_alloc_block * start;    // the start of the my_alloc
    void * end_of_heap;         // the end of the heap region
    size_t occupied_blocks;     // the amount of blocks that are not free
    size_t free_blocks;         // the amount of blocks that are free
    size_t last_block;          // the index of the last occupied block in the my_alloc
} _my_alloc_info;

_my_alloc_info my_alloc = {
    .start = NULL,
};

void my_alloc_init() {
    if (BLOCK_ALIGNMENT < sizeof(_my_alloc_block)) {
        println("Allocator error: Block alignment must be greater than or equal to block size");
        _exit(1);
    }
    my_alloc.start = sbrk(0);
    sbrk(BLOCK_ALIGNMENT * START_ARENA_SIZE);
    my_alloc.end_of_heap = sbrk(0);

    my_alloc.free_blocks = 0;
    my_alloc.occupied_blocks = 0;
    my_alloc.last_block = 0;
}

void print_block(_my_alloc_block * block, size_t index) {
    println("Block: [addr=%p, type=%s, index=%d, size=%d]", &block->data, IS_FREE(*block) ? "Free" : "Used", index, GET_BLOCK_SIZE(index));
}

void my_alloc_print_blocks() {
    println("======");
    size_t index = 0;
    _my_alloc_block * block = GET_BLOCK_AT_INDEX(index);
    do {
        print_block(block, index);
        index = block->next_index;
        block = GET_BLOCK_AT_INDEX(index);
    } while (index <= my_alloc.last_block);
    println("======");
}

void my_alloc_grow(size_t min_blocks_capacity) {
    size_t delta = ARENA_CAPACITY;
    while (delta < min_blocks_capacity) { delta *= GROWTH_SPEED; } // make sure that the we grow to support our new block
    my_alloc.end_of_heap = sbrk(delta * BLOCK_ALIGNMENT);
}

void * my_alloc_mark_free_block(size_t index, size_t size) {
    _my_alloc_block * old_block = GET_BLOCK_AT_INDEX(index);
    size_t next_index = index + (size + BLOCK_ALIGNMENT - 1) / BLOCK_ALIGNMENT; // index of the block after the new block

    if (ARENA_CAPACITY <= next_index) {
        println("Allocator error: Marking free block causes heap overflow");
        _exit(1);
    } else if (old_block->next_index < next_index) {
        println("Allocator error: Trying to use more than available in free block");
        _exit(1);
    } else if (next_index < old_block->next_index) { // New block doesn't consume whole free block: add a new free block after
        _my_alloc_block * free_block = GET_BLOCK_AT_INDEX(next_index);
        free_block->next_index = old_block->next_index;
        free_block->prev_index = index;
        free_block->is_available = AVAILABLE;
    } else {
        my_alloc.free_blocks -= 1; // the whole free block is consumed so remove it
    }

    my_alloc.occupied_blocks += 1;

    if (my_alloc.last_block < index) {
        my_alloc.last_block = index;
    }

    old_block->next_index = next_index;
    old_block->is_available = NOT_AVAILABLE;

    return &old_block->data;
}

void * my_alloc_add_block(size_t prev_index, size_t index, size_t size) {
    size_t next_index = index + (size + BLOCK_ALIGNMENT - 1) / BLOCK_ALIGNMENT; // index of the block after the new block

    if (ARENA_CAPACITY <= next_index) {
        my_alloc_grow(next_index);
    }

    my_alloc.occupied_blocks += 1;
    my_alloc.last_block = index;

    _my_alloc_block * block = GET_BLOCK_AT_INDEX(index);
    block->is_available = NOT_AVAILABLE;
    block->prev_index = prev_index;
    block->next_index = next_index;

    return &block->data;
}

void * my_alloc_malloc(size_t size) {
    if (my_alloc.start == NULL) {
        my_alloc_init();
    }

    if (size == 0) {
        return NULL;
    }

    size += offsetof(_my_alloc_block, data); // allocate memory for allocator info as well

    if (my_alloc.free_blocks == 0) {
        // add block after the last block
        return my_alloc_add_block(my_alloc.last_block, GET_BLOCK_AT_INDEX(my_alloc.last_block)->next_index, size);
    }

    // Check if there is a free block that can be occupied
    size_t index = 0;
    _my_alloc_block * block = GET_BLOCK_AT_INDEX(index);

    // Make sure that the block is within the arena and then keep going if is not free or block size is less than size to alloc
    while (index < my_alloc.last_block && (!IS_FREE(*block) || GET_BLOCK_SIZE(index) < size)) {
        index = block->next_index;
        block = GET_BLOCK_AT_INDEX(index);
    }

    // Found a free block that can be occupied
    if (IS_FREE(*block) && size <= GET_BLOCK_SIZE(index)) {
        return my_alloc_mark_free_block(index, size);
    }

    return my_alloc_add_block(index, block->next_index, size);
}

void * my_alloc_calloc(size_t n, size_t el_size) {
    const size_t size = n * el_size;

    void * ptr = malloc(size); 
    memset(ptr, 0, size);

    return ptr;
}

void * my_alloc_realloc(void * ptr, size_t new_size) {
    _my_alloc_block * block = ptr - offsetof(_my_alloc_block, data);
    if (IS_FREE(*block)) {
        println("Allocator error: double free occured");
        _exit(1);
    }

    // find the block index of the ptr
    size_t index;
    if (block != my_alloc.start) {
        index = GET_BLOCK_AT_INDEX(block->prev_index)->next_index;
    } else {
        index = 0;
    }

    const size_t old_capacity = GET_BLOCK_AVAILABLE_SIZE(index); // ptr block data capacity

    if (new_size <= old_capacity) {
        return ptr;
    }

    free(ptr);
    void * new_ptr = malloc(new_size);

    if (new_ptr == ptr) {
        return new_ptr;
    }

    // write data from ptr to new_ptr
    memcpy(new_ptr, ptr, old_capacity);

    return new_ptr;
}

void my_alloc_free(void * ptr) {
    _my_alloc_block * block = ptr - offsetof(_my_alloc_block, data);
    block->is_available = AVAILABLE;

    char is_joined = 0;
    char is_last_block = GET_BLOCK_AT_INDEX(my_alloc.last_block) == block;
    my_alloc.occupied_blocks -= 1;

    // Merge if block after is a free block
    _my_alloc_block * next = GET_BLOCK_AT_INDEX(block->next_index);
    if (IS_FREE(*next)) {
        block->next_index = next->next_index;
        is_joined = 1;
    }

    // Merge if block before is a free block
    _my_alloc_block * prev = GET_BLOCK_AT_INDEX(block->prev_index);
    if (IS_FREE(*prev)) {
        prev->next_index = block->next_index;
        is_joined = 1;
    }

    if (!is_joined) {
        my_alloc.free_blocks += 1;
    }

    // Update last block if the freed block was the last block
    if (is_last_block) {
        if (IS_FREE(*prev)) {
            my_alloc.last_block = prev->prev_index;
        } else {
            my_alloc.last_block = block->prev_index;
        }
    }


}
