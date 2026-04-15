#include "pool.h"
#include <stdint.h>

int pool_init(Pool *p, void *buffer, size_t obj_size, size_t capacity)
{
    // Basic requirement: free list needs space for a pointer inside each block
    if (obj_size < sizeof(void *)) return -1;
    if (!buffer || capacity == 0) return -1;
    // Also check alignment if needed
    if ((uintptr_t)buffer % _Alignof(void *) != 0) return -1;

    p->obj_size = obj_size;
    p->capacity = capacity;
    p->buffer = (char *)buffer;
    p->free_list = buffer;
    p->free_count = capacity;

    // Link all free blocks together
    char *block = (char *)buffer;
    for (size_t i = 0; i < capacity - 1; i++)
    {
        void **next = (void **)block; // next pointer lives at start of free block
        *next = block + obj_size;
        block += obj_size;
    }
    void **last = (void **)block;
    *last = NULL; // end of free list
    return 0;
}

void *pool_malloc(Pool *p)
{
    if (!p->free_list)
        return NULL;

    void *obj = p->free_list;
    // Advance free_list to the next free block
    p->free_list = *(void **)obj;
    p->free_count--;
    return obj;
}

void pool_free(Pool *p, void *obj)
{
    if (obj == NULL) return;

    // Place the freed block at the head of the free list
    *(void **)obj = p->free_list;
    p->free_list = obj;
    p->free_count++;
}

void pool_reset(Pool *p)
{
    if (!p) return;

    /* Re‑link all blocks as a free list */
    p->free_list = p->buffer;
    p->free_count = p->capacity;

    char *block = p->buffer;
    for (size_t i = 0; i < p->capacity - 1; i++)
    {
        void **next = (void **)block;
        *next = block + p->obj_size;
        block += p->obj_size;
    }
    void **last = (void **)block;
    *last = NULL;
}

size_t pool_free_count(const Pool *p)
{
    return p ? p->free_count : 0;
}