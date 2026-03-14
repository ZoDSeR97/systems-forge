#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief Test if a number is a power of two.
     */
    static inline bool is_power_of_two(size_t x)
    {
        return x && ((x & (x - 1)) == 0);
    }

    /**
     * @brief Align a pointer forward to the given alignment (must be power of two).
     */
    static inline uintptr_t align_forward(uintptr_t ptr, size_t alignment)
    {
        return (ptr + (alignment - 1)) & ~(uintptr_t)(alignment - 1);
    }

    /**
     * @brief Opaque type representing a fixed-capacity arena allocator.
     */
    typedef struct arena arena;

    /**
     * @brief Creates a heap‑allocated arena.
     * @param capacity Total bytes.
     * @return Pointer to arena, or NULL on failure.
     */
    arena *arena_init(size_t capacity);

    /**
     * @brief Initialises a static arena from user‑supplied memory.
     * @param arena   Pointer to arena structure (must be allocated by caller).
     * @param buffer  Pointer to memory block for allocations.
     * @param capacity Size of buffer in bytes.
     * @return true on success, false if buffer is NULL or capacity==0.
     */
    bool arena_init_static(arena *arena, void *buffer, size_t capacity);

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
     * @brief Resets and securely erases all memory in the arena.
     * @param arena Pointer to arena.
     */
    void arena_reset_secure(arena *arena);

    /**
     * @brief Frees the memory used by the arena.
     * @param arena Pointer to the arena to free.
     */
    void arena_free(arena *arena);

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
#ifdef __cplusplus
}
#endif

#endif