#include "allocator.h"

#include <stdlib.h>

struct tls_arena_pool
{
    size_t arena_capacity;
};

/* One arena per thread */
static _Thread_local arena *tls_arena = NULL;

/* Track which pool created it */
static _Thread_local const tls_arena_pool *tls_owner = NULL;

tls_arena_pool *tls_arena_pool_create(size_t arena_capacity)
{
    if (arena_capacity == 0)
        return NULL;

    tls_arena_pool *pool =
        (tls_arena_pool *)malloc(sizeof(*pool));

    if (!pool)
        return NULL;

    pool->arena_capacity = arena_capacity;
    return pool;
}

static arena *get_or_create_tls_arena(const tls_arena_pool *pool)
{
    if (!tls_arena)
    {
        tls_arena = arena_create(pool->arena_capacity);
        tls_owner = pool;
    }
    return tls_arena;
}

void *tls_arena_malloc(tls_arena_pool *pool, size_t size, size_t alignment)
{
    if (!pool)
        return NULL;

    arena *arena = get_or_create_tls_arena(pool);
    if (!arena)
        return NULL;

    return arena_malloc(arena, size, alignment);
}

void tls_arena_reset(tls_arena_pool *pool)
{
    if (!pool)
        return;

    if (tls_arena && tls_owner == pool)
    {
        arena_reset(tls_arena);
    }
}

void tls_arena_pool_destroy(tls_arena_pool *pool)
{
    if (!pool)
        return;

    /* If current thread owns an arena from this pool, destroy it */
    if (tls_arena && tls_owner == pool)
    {
        arena_destroy(tls_arena);
        tls_arena = NULL;
        tls_owner = NULL;
    }

    free(pool);
}