# Fixed‑Capacity Memory Pool ABI Design

The allocator manages objects of a fixed size, using a free list stored inside the free blocks themselves.

## Ownership & Lifetime

- The caller is responsible for providing a buffer that remains valid for the lifetime of the pool.
- The pool does not take ownership of the buffer; it only manages the allocation and deallocation of objects within it.
- The pool does not perform any dynamic memory allocation; all memory management is done by the caller.
- The pool does not control the process and does not have any side effects outside of managing the provided buffer.
