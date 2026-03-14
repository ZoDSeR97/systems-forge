# Arena Allocator ABI Design

## Ownership & Lifetime

- All memory returned by `arena_malloc` is owned by the arena instance.
- Calling `arena_reset` invalidates all outstanding pointers previously returned by `arena_malloc`.
- After `arena_free`, all pointers and the arena instance itself are invalid.
- Individual memory frees are not supported; only bulk reset and free operations.

## Alignment & Portability

- All allocations are aligned to at least `sizeof(max_align_t)`, ensuring portability and correctness across platforms.
- If a requested alignment is not a power of two, the allocator defaults to `sizeof(max_align_t)`.

## Process Control

- The allocator library does not control process lifetime or thread management.
- All resource management is explicit and under user control.
