#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>
#include <cstddef>

extern "C"
{
    #include "arena.h"
    #include "pool.h"
}

static constexpr size_t ALLOC_SIZE = 64;
static constexpr size_t NUM_ALLOCS = 5'000'000;
static constexpr size_t ARENA_CAPACITY = NUM_ALLOCS * ALLOC_SIZE + 1024;

using clock_type = std::chrono::high_resolution_clock;

static void benchmark_malloc()
{
    auto start = clock_type::now();

    for (size_t i = 0; i < NUM_ALLOCS; ++i)
    {
        void *ptr = std::malloc(ALLOC_SIZE);
        if (!ptr) std::abort();
        std::free(ptr);
    }

    auto end = clock_type::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "malloc/free: " << diff.count() << " s\n";
}

static void benchmark_arena()
{
    Arena *arena = arena_init(ARENA_CAPACITY);
    if (!arena) std::abort();

    auto start = clock_type::now();

    for (size_t i = 0; i < NUM_ALLOCS; ++i)
    {
        void *ptr = arena_malloc(arena, ALLOC_SIZE, 0);
        if (!ptr) std::abort();
    }

    auto end = clock_type::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "arena: " << diff.count() << " s\n";

    arena_free(arena);
}

static void benchmark_pool()
{
    // Create pool with single block; allocate then immediately free
    Pool *pool = pool_init(ALLOC_SIZE, 1);
    if (!pool) std::abort();

    auto start = clock_type::now();

    for (size_t i = 0; i < NUM_ALLOCS; ++i)
    {
        void *ptr = pool_malloc(pool);
        if (!ptr) std::abort();
        pool_pop(pool, ptr);
    }

    auto end = clock_type::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "pool: " << diff.count() << " s\n";

    pool_free(pool);
}

int main()
{
    std::cout << "Allocator benchmark\n";
    std::cout << "Alloc size: " << ALLOC_SIZE << " bytes\n";
    std::cout << "Allocations: " << NUM_ALLOCS << "\n\n";

    benchmark_malloc();
    benchmark_arena();
    benchmark_pool();

    return 0;
}