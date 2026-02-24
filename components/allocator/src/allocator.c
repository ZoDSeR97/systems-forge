#include "allocator.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct arena
{
    size_t capacity;
    size_t offset;
    unsigned char *memory;
};

/* -------- Internal Utilities -------- */

static int is_power_of_two(size_t x)
{
    return x && ((x & (x - 1)) == 0);
}

static uintptr_t align_forward(uintptr_t ptr, size_t alignment)
{
    return (ptr + (alignment - 1)) & ~(uintptr_t)(alignment - 1);
}

/* -------- Public API -------- */

arena *arena_create(size_t capacity)
{
    if (capacity == 0)
        return NULL;

    /* allocate the struct itself */
    arena *block = (arena *)malloc(sizeof(*block));
    if (!block)
        return NULL;

    block->memory = (unsigned char *)malloc(capacity);
    if (!block->memory)
    {
        free(block);
        return NULL;
    }

    block->capacity = capacity;
    block->offset = 0;

    return block;
}

void *arena_malloc(arena *arena, size_t size, size_t alignment)
{
    if (!arena || size == 0)
        return NULL;

    if (alignment == 0)
        alignment = sizeof(max_align_t);

    if (!is_power_of_two(alignment))
        alignment = 1;

    uintptr_t base = (uintptr_t)arena->memory;
    uintptr_t current = base + arena->offset;
    uintptr_t aligned = align_forward(current, alignment);

    size_t padding = (size_t)(aligned - current);

    /* Overflow protection */
    if (padding > SIZE_MAX - arena->offset)
        return NULL;

    size_t new_offset = arena->offset + padding;

    if (size > SIZE_MAX - new_offset)
        return NULL;

    new_offset += size;

    if (new_offset > arena->capacity)
        return NULL;

    void *result = (void *)(base + arena->offset + padding);
    arena->offset = new_offset;

    return result;
}

void arena_reset(arena *arena)
{
    if (!arena)
        return;

    arena->offset = 0;
}

void arena_destroy(arena *arena)
{
    if (!arena)
        return;

    free(arena->memory);
    arena->memory = NULL;
    arena->capacity = 0;
    arena->offset = 0;

    free(arena);
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
    if (!arena)
        return 0;
    return arena->capacity - arena->offset;
}