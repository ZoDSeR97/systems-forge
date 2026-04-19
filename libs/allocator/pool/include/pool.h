#ifndef POOL_H
#define POOL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /* Opaque pool type */
    typedef struct Pool Pool;

    /**
     * Initialize a new pool allocator.
     *
     * @param block_size   Size of each fixed block (bytes). If < sizeof(void*),
     *                     it will be rounded up to sizeof(void*).
     * @param num_blocks   Number of blocks in the pool.
     * @return             Initialized pool, or NULL on failure.
     */
    Pool *pool_init(size_t block_size, size_t num_blocks);

    /**
     * Allocate one block from the pool.
     *
     * @param pool   Pointer to the pool.
     * @return       Pointer to allocated block, or NULL if pool is exhausted.
     */
    void *pool_malloc(Pool *pool);

    /**
     * Free a previously allocated block back to the pool.
     *
     * @param pool   Pointer to the pool.
     * @param ptr    Pointer to the block to free (must have been returned by pool_alloc).
     */
    void pool_pop(Pool *pool, void *ptr);

    /**
     * Destroy the pool and free all associated memory.
     *
     * @param pool   Pointer to the pool (becomes invalid after call).
     */
    void pool_free(Pool *pool);

#ifdef __cplusplus
}
#endif

#endif
