#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Opaque type representing a fixed-capacity arena allocator.
     */
    typedef struct arena arena;

    /**
     * @brief Creates a fixed-capacity arena.
     * @param capacity The total number of bytes the arena can allocate.
     * @return Pointer to the created arena, or NULL on failure.
     */
    arena *arena_create(size_t capacity);

    /**
     * @brief Allocates memory from the arena with specified alignment.
     * @param arena Pointer to the arena.
     * @param size Number of bytes to allocate.
     * @param alignment Alignment in bytes (0 = default alignment).
     * @return Pointer to allocated memory, or NULL on failure.
     */
    void *arena_malloc(arena *arena, size_t size, size_t alignment);

    /**
     * @brief Resets the arena, invalidating all previous allocations.
     * @param arena Pointer to the arena.
     */
    void arena_reset(arena *arena);

    /**
     * @brief Frees the memory used by the arena.
     * @param arena Pointer to the arena to destroy.
     */
    void arena_destroy(arena *arena);

    /**
     * @brief Gets the total capacity of the arena.
     * @param arena Pointer to the arena.
     * @return Total capacity in bytes.
     */
    size_t arena_capacity(const arena *arena);

    /**
     * @brief Gets the number of bytes used in the arena.
     * @param arena Pointer to the arena.
     * @return Number of bytes used.
     */
    size_t arena_used(const arena *arena);

    /**
     * @brief Gets the number of bytes available in the arena.
     * @param arena Pointer to the arena.
     * @return Number of bytes available.
     */
    size_t arena_available(const arena *arena);

    /**
     * @brief Opaque type representing a thread-safe arena allocator.
     */
    typedef struct threadsafe_arena threadsafe_arena;

    /**
     * @brief Creates a thread-safe arena with fixed capacity.
     * @param capacity The total number of bytes the arena can allocate.
     * @return Pointer to the created thread-safe arena, or NULL on failure.
     */
    threadsafe_arena *threadsafe_arena_create(size_t capacity);

    /**
     * @brief Allocates memory from the thread-safe arena with specified alignment.
     * @param arena Pointer to the thread-safe arena.
     * @param size Number of bytes to allocate.
     * @param alignment Alignment in bytes (0 = default alignment).
     * @return Pointer to allocated memory, or NULL on failure.
     */
    void *threadsafe_arena_malloc(threadsafe_arena *arena, size_t size, size_t alignment);

    /**
     * @brief Resets the thread-safe arena, invalidating all previous allocations.
     * @param arena Pointer to the thread-safe arena.
     */
    void threadsafe_arena_reset(threadsafe_arena *arena);

    /**
     * @brief Frees the memory used by the thread-safe arena.
     * @param arena Pointer to the thread-safe arena to destroy.
     */
    void threadsafe_arena_destroy(threadsafe_arena *arena);

    /**
     * @brief Opaque type representing a thread-local arena pool.
     */
    typedef struct tls_arena_pool tls_arena_pool;

    /**
     * @brief Creates a thread-local arena pool (one arena per thread).
     * @param arena_capacity The capacity of each arena in the pool.
     * @return Pointer to the created thread-local arena pool, or NULL on failure.
     */
    tls_arena_pool *tls_arena_pool_create(size_t arena_capacity);

    /**
     * @brief Allocates memory from the thread-local arena pool with specified alignment.
     * @param pool Pointer to the thread-local arena pool.
     * @param size Number of bytes to allocate.
     * @param alignment Alignment in bytes (0 = default alignment).
     * @return Pointer to allocated memory, or NULL on failure.
     */
    void *tls_arena_malloc(tls_arena_pool *pool, size_t size, size_t alignment);

    /**
     * @brief Resets all arenas in the thread-local arena pool.
     * @param pool Pointer to the thread-local arena pool.
     */
    void tls_arena_reset(tls_arena_pool *pool);

    /**
     * @brief Frees the memory used by the thread-local arena pool.
     * @param pool Pointer to the thread-local arena pool to destroy.
     */
    void tls_arena_pool_destroy(tls_arena_pool *pool);

#ifdef __cplusplus
}
#endif

#endif