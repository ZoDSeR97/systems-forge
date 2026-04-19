# Fixed‑Capacity Memory Pool ABI Design

## Overview

A fixed-capacity object pool that allocates and manages a set number of fixed-size blocks. Uses an embedded free list for O(1) allocation and deallocation.

## Architecture

All pool memory is allocated contiguously. Free blocks are tracked via a singly-linked list embedded directly in the pool memory:

- Block size is automatically adjusted to at least `sizeof(void*)` to accommodate free-list pointers
- Free-list nodes stored at the start of each free block
- All `num_blocks` are pre-allocated at initialization
- Simple linked-list traversal (pop from front to allocate, push to front to deallocate)

## API Design

### Initialization

- `pool_init(block_size, num_blocks)` - Creates and initializes a new pool
  - Returns an opaque `Pool*` handle
  - Allocates contiguous memory for all blocks
  - `block_size` is automatically adjusted to at least `sizeof(void*)` for free-list tracking
  - All `num_blocks` are pre-allocated and linked in free list
  - Returns `NULL` on failure (memory exhaustion or overflow)

### Allocation & Deallocation

- `pool_malloc(pool)` - Allocates one block
  - Pops from front of free list: O(1)
  - Returns pointer to block or `NULL` if exhausted
  
- `pool_pop(pool, ptr)` - Returns block to pool
  - Pushes to front of free list: O(1)
  - Silently ignores if `ptr` is `NULL`
  - Silently ignores if `ptr` is out of pool bounds
  - Behavior undefined if `ptr` not from this pool or already freed

### Lifecycle

- `pool_free(pool)` - Deallocates all resources
  - Frees pool metadata and memory
  - Caller responsible for cleanup of user data within blocks

## Ownership & Lifetime

- **Pool Structure**: Allocated internally via `malloc`. Caller must call `pool_free()`.
- **Pool Memory**: Single contiguous allocation. All blocks managed by pool.
- **Blocks**: Lifetime tied to pool. Calling `pool_free()` invalidates all blocks.
- **User Data**: If blocks contain pointers to external data, caller is responsible for cleanup.
- **Free-List Overhead**: First `sizeof(void*)` bytes of each free block used for next-pointer. For allocated blocks, this space is available to caller.

## Memory Safety

- Requires `block_size >= sizeof(void*)` for free-list nodes (auto-adjusted at init)
- Free-list nodes stored inline within free blocks
- Contiguous allocation eliminates fragmentation and corruption
- Blocks validated at deallocation (bounds check; silently reject invalid pointers)
- Thread safety: External synchronization required for concurrent access

## Error Handling

- `pool_init` returns `NULL` on allocation failure or overflow
- `pool_malloc` returns `NULL` when no free blocks remain
- `pool_pop(pool, NULL)` is safe (no-op)
- `pool_pop` silently ignores pointers outside pool bounds
- Freeing already-freed or never-allocated blocks: undefined behavior

## Performance Characteristics

- **Allocation**: O(1) - pop from free-list front
- **Deallocation**: O(1) - push to free-list front
- **Init**: O(num_blocks) - initialize free list chain
- **Memory Overhead**: One `Pool` struct (~40 bytes) + one `sizeof(void*)` per block (for free-list nodes)
