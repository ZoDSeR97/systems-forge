#include "arena.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

struct arena
{
    size_t capacity;
    size_t offset;
    unsigned char *memory;
    bool owns_memory; // true if memory was allocated by arena_init
};

/* -------- Public API -------- */

arena *arena_init(size_t capacity)
{
    if (capacity == 0) return NULL;

    /* allocate the struct itself */
    arena *a = (arena *)malloc(sizeof(arena));
    if (!a) return NULL;

    a->memory = (unsigned char *)malloc(capacity);
    if (!a->memory)
    {
        free(a);
        return NULL;
    }

    a->capacity = capacity;
    a->offset = 0;
    a->owns_memory = true;
    return a;
}

bool arena_init_static(arena *a, void *buffer, size_t capacity)
{
    if (!a || !buffer || capacity == 0) return false;

    // Check that buffer is suitably aligned for any type.
    // max_align_t is the strictest fundamental alignment.
    if ((uintptr_t)buffer % _Alignof(max_align_t) != 0) return false;

    a->memory = (unsigned char *)buffer;
    a->capacity = capacity;
    a->offset = 0;
    a->owns_memory = false;
    return true;
}

void *arena_malloc(arena *a, size_t size, size_t alignment)
{
    if (!a || size == 0) return NULL;

    if (alignment == 0) alignment = _Alignof(max_align_t);
    else if (!is_power_of_two(alignment)) alignment = _Alignof(max_align_t);

    uintptr_t base = (uintptr_t)a->memory;
    uintptr_t current = base + a->offset;
    uintptr_t aligned = align_forward(current, alignment);

    size_t padding = (size_t)(aligned - current);

    // Check for overflow
    if (padding > SIZE_MAX - a->offset) return NULL;
    size_t new_offset = a->offset + padding;

    if (size > SIZE_MAX - new_offset) return NULL;
    new_offset += size;

    if (new_offset > a->capacity) return NULL;

    void *result = (void *)(base + a->offset + padding);
    a->offset = new_offset;
    return result;
}

void arena_reset(arena *arena)
{
    if (!arena) return;

    arena->offset = 0;
}

void arena_reset_secure(arena *a)
{
    if (!a) return;
    // Overwrite entire arena memory with zeros (or a custom pattern)
    memset(a->memory, 0, a->capacity);
    a->offset = 0;
}

void arena_free(arena *a)
{
    if (!a) return;
    if (a->owns_memory)
    {
        free(a->memory);
        a->memory = NULL;
        a->capacity = 0;
        a->offset = 0;
        free(a);
    }
}

size_t arena_capacity(const arena *arena)
{
    return arena ? arena->capacity : 0;
}

size_t arena_used(const arena *arena)
{
    return arena ? arena->offset : 0;
}

size_t arena_available(const arena *arena)
{
    return arena ? arena->capacity - arena->offset : 0;
}