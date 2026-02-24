# Arena Allocator ABI Design

- All memory returned by arena_malloc is owned by the arena.
- arena_reset invalidates all outstanding pointers.
- After arena_destroy, all pointers are invalid.
- Individual frees are not supported.
- The tls_allocator does not automatically reclaim memory when threads exit, it assumes controlled thread lifetimes or explicit cleanup.

```plaintext
| Allocator              | Locking | Contention      | Use Case             |
| ---------------------- | ------- | --------------- | -------------------- |
| allocator              | none    | N/A             | single-thread        |
| threadsafe_allocator   | mutex   | high under load | shared global state  |
| tls_allocator          | none    | none            | per-thread workloads |
```
