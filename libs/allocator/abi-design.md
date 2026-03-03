# Arena Allocator ABI Design

## Ownership & Lifetime

- All memory returned by `arena_malloc` is owned by the arena instance.
- Calling `arena_reset` invalidates all outstanding pointers previously returned by `arena_malloc`.
- After `arena_destroy`, all pointers and the arena instance itself are invalid.
- Individual memory frees are not supported; only bulk reset and destroy operations.

## Threading & Safety

- The basic allocator (`arena`) is not thread-safe; intended for single-threaded use.
- For multi-threaded scenarios, use `threadsafe_allocator` (with internal mutex) or `tls_allocator` (per-thread instance).
- The `tls_allocator` does not automatically reclaim memory when threads exit; explicit cleanup is required.

## Alignment & Portability

- All allocations are aligned to at least `sizeof(max_align_t)`, ensuring portability and correctness across platforms.
- If a requested alignment is not a power of two, the allocator defaults to `sizeof(max_align_t)`.

## Process Control

- The allocator library does not control process lifetime or thread management.
- All resource management is explicit and under user control.

## Component Table

```plaintext
| Allocator              | Locking | Contention      | Use Case             |
| ---------------------- | ------- | --------------- | -------------------- |
| allocator              | none    | N/A             | single-thread        |
| threadsafe_allocator   | mutex   | high under load | shared global state  |
| tls_allocator          | none    | none            | per-thread workloads |
```
