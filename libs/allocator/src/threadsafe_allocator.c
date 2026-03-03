#include "allocator.h"
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

struct threadsafe_arena
{
    arena *arena;
    #ifdef _WIN32
        CRITICAL_SECTION mutex;
    #else
        pthread_mutex_t mutex;
    #endif
};

threadsafe_arena *threadsafe_arena_create(size_t capacity)
{
    threadsafe_arena *tsa =
        (threadsafe_arena *)malloc(sizeof(*tsa));

    if (!tsa)
        return NULL;

    tsa->arena = arena_create(capacity);
    if (!tsa->arena)
    {
        free(tsa);
        return NULL;
    }

    #ifdef _WIN32
        InitializeCriticalSection(&tsa->mutex);
    #else
        if (pthread_mutex_init(&tsa->mutex, NULL) != 0)
        {
            arena_destroy(tsa->arena);
            free(tsa);
            return NULL;
        }
    #endif

    return tsa;
}

void *threadsafe_arena_malloc(threadsafe_arena *tsa, size_t size, size_t alignment)
{
    if (!tsa)
        return NULL;

    #ifdef _WIN32
        EnterCriticalSection(&tsa->mutex);
    #else
        pthread_mutex_lock(&tsa->mutex);
    #endif

    void *result = arena_malloc(tsa->arena, size, alignment);

    #ifdef _WIN32
        LeaveCriticalSection(&tsa->mutex);
    #else
        pthread_mutex_unlock(&tsa->mutex);
    #endif

    return result;
}

void threadsafe_arena_reset(threadsafe_arena *tsa)
{
    if (!tsa)
        return;

    #ifdef _WIN32
        EnterCriticalSection(&tsa->mutex);
    #else
        pthread_mutex_lock(&tsa->mutex);
    #endif

    arena_reset(tsa->arena);

    #ifdef _WIN32
        LeaveCriticalSection(&tsa->mutex);
    #else
        pthread_mutex_unlock(&tsa->mutex);
    #endif
}

void threadsafe_arena_destroy(threadsafe_arena *tsa)
{
    if (!tsa)
        return;

    #ifdef _WIN32
        DeleteCriticalSection(&tsa->mutex);
    #else
        pthread_mutex_destroy(&tsa->mutex);
    #endif

    arena_destroy(tsa->arena);
    free(tsa);
}