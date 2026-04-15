#ifndef POOL_H
#define POOL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief Pool – fixed‑capacity memory pool for objects of any type.
     *
     * The pool uses a free list stored inside free blocks, so each object
     * must be at least as large as a pointer (sizeof(void*)).
     *
     * Thread safety: not thread‑safe. Protect with external mutex if needed.
     */
    typedef struct
    {
        size_t obj_size;   // size of each object
        size_t capacity;   // total number of objects
        void *free_list;   // head of the free list
        char *buffer;      // pointer to the actual memory (for optional checks)
        size_t free_count; // number of free objects
    } Pool;

    /**
     * @brief Initialises a pool.
     *
     * @param p         pointer to the Pool structure to initialise
     * @param buffer    pointer to a memory area large enough for 'capacity' objects.
     *                  The buffer must be correctly aligned for the object type.
     * @param obj_size  size (in bytes) of each object. Must be >= sizeof(void*).
     * @param capacity  number of objects in the pool
     */
    int pool_init(Pool *p, void *buffer, size_t obj_size, size_t capacity);

    /**
     * @brief Allocates one object from the pool.
     *
     * @param p  pointer to the Pool
     * @return   pointer to an unused object, or NULL if the pool is exhausted
     */
    void *pool_malloc(Pool *p);

    /**
     * @brief Returns an object to the pool.
     *
     * @param p   pointer to the Pool
     * @param obj pointer previously obtained from pool_alloc().
     *            The behaviour is undefined if obj is not a valid pool object
     *            or if it is already free.
     */
    void pool_free(Pool *p, void *obj);

    /**
     * @brief Resets the pool to its initial (all free) state.
     */
    void pool_reset(Pool *p);

    /**
     * @brief Returns the number of free objects currently available.
     */
    size_t pool_free_count(const Pool *p);

#ifdef __cplusplus
}
#endif

#endif