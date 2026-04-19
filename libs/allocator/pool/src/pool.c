#include "pool.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct Pool
{
    void *memory;      /* pointer to allocated memory pool */
    void *free_list;   /* head of free list (singly linked) */
    size_t block_size; /* size of each block (bytes) */
    size_t capacity;   /* total number of blocks */
    size_t used;       /* number of currently allocated blocks */
};

/* Ensure block size is at least large enough to hold a free-list pointer */
static size_t adjust_block_size(size_t block_size)
{
    if (block_size < sizeof(void *))
    {
        return sizeof(void *);
    }
    /* Optionally align to pointer size for better performance */
    size_t alignment = sizeof(void *);
    return (block_size + alignment - 1) & ~(alignment - 1);
}

Pool *pool_init(size_t block_size, size_t num_blocks)
{
    if (num_blocks == 0)
    {
        return NULL;
    }

    block_size = adjust_block_size(block_size);

    /* Check for overflow in total memory allocation */
    size_t total_size;
    if (__builtin_mul_overflow(block_size, num_blocks, &total_size))
    {
        return NULL; /* Overflow */
    }

    Pool *pool = (Pool *)malloc(sizeof(Pool));
    if (!pool)
    {
        return NULL;
    }

    pool->memory = malloc(total_size);
    if (!pool->memory)
    {
        free(pool);
        return NULL;
    }

    pool->block_size = block_size;
    pool->capacity = num_blocks;
    pool->used = 0;

    /* Initialize free list as a linked list inside the memory pool */
    char *block = (char *)pool->memory;
    for (size_t i = 0; i < num_blocks; ++i)
    {
        void **next_ptr = (void **)block;
        if (i == num_blocks - 1)
        {
            *next_ptr = NULL;
        }
        else
        {
            *next_ptr = block + block_size;
        }
        block += block_size;
    }
    pool->free_list = pool->memory;

    return pool;
}

void *pool_malloc(Pool *pool)
{
    if (!pool || !pool->free_list)
    {
        return NULL;
    }

    void *block = pool->free_list;
    pool->free_list = *(void **)block;
    pool->used++;

    return block;
}

void pool_pop(Pool *pool, void *ptr)
{
    if (!pool || !ptr)
    {
        return;
    }

    /* Basic validation: check if ptr lies within pool memory */
    if (ptr < pool->memory || (char *)ptr >= (char *)pool->memory + pool->capacity * pool->block_size)
    {
        return; /* Not from this pool – ignore */
    }

    *(void **)ptr = pool->free_list;
    pool->free_list = ptr;
    pool->used--;
}

void pool_free(Pool *pool)
{
    if (pool)
    {
        free(pool->memory);
        free(pool);
    }
}