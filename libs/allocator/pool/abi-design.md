# Fixed‑Capacity Memory Pool ABI Design

## Overview

A zero-allocation fixed-capacity object pool. Caller provides buffer; pool manages an embedded free list for O(1) allocation and deallocation. No malloc dependency—suitable for embedded, bare-metal, and deterministic systems.

## Architecture

Pool struct is caller-managed (stack, heap, static, or embedded). Caller provides contiguous buffer. Free blocks tracked via singly-linked list embedded directly in buffer:

- Objects stored contiguously in caller-provided buffer
- Object size must be at least `sizeof(void*)` to accommodate free-list pointers
- Free-list nodes stored at the start of each free block
- All objects pre-linked in free list at initialization
- Simple linked-list traversal (pop from front to allocate, push to front to deallocate)
- Caller retains full control over memory layout, allocation strategy, and lifecycle

## API Design

### Initialization

- `pool_init(p, buffer, obj_size, capacity)` - Initializes an existing pool structure
  - `p`: pointer to caller-allocated `Pool` struct (can be stack-allocated)
  - `buffer`: pointer to caller-allocated memory (must hold `capacity * obj_size` bytes)
  - `obj_size`: size of each object (must be >= `sizeof(void*)`)
  - `capacity`: number of objects in pool
  - Returns 0 on success, -1 on validation failure (bad alignment, invalid parameters)
  - No malloc performed

### Allocation & Deallocation

- `pool_malloc(p)` - Allocates one object
  - Pops from front of free list: O(1)
  - Returns pointer to object or `NULL` if exhausted
  
- `pool_free(p, obj)` - Returns object to pool
  - Pushes to front of free list: O(1)
  - Safe to call with `obj = NULL` (no-op)
  - Undefined behavior if `obj` not from this pool or already freed

### Utilities

- `pool_reset(p)` - Resets pool to initial state (all objects free)
  - Rebuilds free list chain in-place
  - Useful for restarting without reallocation
  
- `pool_free_count(p)` - Returns number of free objects currently available
  - Constant-time query

### Lifecycle

- Caller allocates `Pool` struct (stack or heap)
- Caller allocates buffer (arena, malloc, static, stack—caller's choice)
- Caller calls `pool_init()` to link pool to buffer
- Caller controls when pool and buffer are deallocated
- No cleanup function needed (no internal allocations)

## Ownership & Lifetime

- **Pool Structure**: Caller-allocated and managed. Can be stack (auto-freed at scope exit), heap (caller frees), or static.
- **Buffer**: Caller-allocated and managed. Separate lifetime from Pool struct. Can be from malloc, arena, stack, static, DMA region, etc.
- **Objects**: Lifetime tied to buffer. Caller responsible for buffer deallocation when done.
- **User Data**: Objects themselves can hold pointers to external data; caller is responsible for cleanup.
- **Free-List Overhead**: First `sizeof(void*)` bytes of each free block stores next-pointer. For allocated objects, this space is available to caller.
- **No Internal Allocations**: Pool performs zero allocations at runtime. Deterministic behavior.

## Memory Safety

- Requires `obj_size >= sizeof(void*)` for free-list nodes (validated at init)
- Buffer must be correctly aligned for object type
- Free-list nodes stored inline within free blocks
- Contiguous allocation eliminates fragmentation
- Caller responsible for double-free prevention; undefined behavior if violated
- Thread safety: External synchronization required for concurrent access
- No malloc failures at runtime (all allocations pre-linked at init)

## Error Handling

- `pool_init` returns -1 on validation failure (bad alignment, invalid parameters), 0 on success
- `pool_malloc` returns `NULL` when no free objects remain
- `pool_free(p, NULL)` is safe (no-op)
- Freeing already-freed or never-allocated objects: undefined behavior
- Invalid buffer pointer at init: validation may reject (alignment check)

## Performance Characteristics

- **Allocation**: O(1) - pop from free-list front, no synchronization overhead
- **Deallocation**: O(1) - push to free-list front, no validation
- **Initialization**: O(capacity) - link free list chain once
- **Memory Overhead**: One `Pool` struct (caller-managed, ~32 bytes) + one `sizeof(void*)` per free object (for free-list nodes)
- **No Runtime Allocator Work**: Zero malloc/free calls post-init. Deterministic timing.
