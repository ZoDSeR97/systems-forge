#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Allocator */
    typedef struct arena arena;

    /* Creates a fixed-capacity arena */
    arena *arena_create(size_t capacity);

    /* Allocates memory with specified alignment (0 = default alignment) */
    void *arena_malloc(arena *arena, size_t size, size_t alignment);

    /* Resets arena (invalidates all previous allocations) */
    void arena_reset(arena *arena);

    /* Frees memory */
    void arena_destroy(arena *arena);

    /* Inspection helpers */
    size_t arena_capacity(const arena *arena);
    size_t arena_used(const arena *arena);
    size_t arena_available(const arena *arena);

    /* Mutex-Based Thread-safe arena wrapper */
    typedef struct threadsafe_arena threadsafe_arena;

    threadsafe_arena *threadsafe_arena_create(size_t capacity);

    void *threadsafe_arena_malloc(threadsafe_arena *arena, size_t size, size_t alignment);

    void threadsafe_arena_reset(threadsafe_arena *arena);

    void threadsafe_arena_destroy(threadsafe_arena *arena);

    typedef struct tls_arena_pool tls_arena_pool;

    /* Thread-local arena pool (one arena per thread) */
    tls_arena_pool *tls_arena_pool_create(size_t arena_capacity);

    void *tls_arena_malloc(tls_arena_pool *pool, size_t size, size_t alignment);

    void tls_arena_reset(tls_arena_pool *pool);

    void tls_arena_pool_destroy(tls_arena_pool *pool);

#ifdef __cplusplus
}
#endif

#endif