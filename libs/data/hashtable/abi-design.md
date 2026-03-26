# ABI Design for `hashtable` – SwissTable‑based Hash Map

This library provides a hash map (`ht`) that stores arbitrary pointer values keyed by NUL‑terminated strings. It is implemented on top of `cwisstable` [source](https://github.com/google/cwisstable), a C port of Abseil’s SwissTable and utilizing `HighwayHash` [source](https://github.com/google/highwayhash) hash function atm.

## Public API Functions

All functions are declared with C linkage (extern "C"). The signatures are:

```c
ht *ht_create(size_t initial_capacity);
void ht_destroy(ht *map);
int ht_put(ht *map, const char *key, void *value);
void *ht_get(const ht *map, const char *key);
int ht_contains(const ht *map, const char *key);
int ht_remove(ht *map, const char *key);
```

## Opaque Type `ht`

The `ht` structure is declared in the public header as an incomplete type. Its full definition (in `hashtable.c`) is:

```c
struct ht {
    CWISS_RawTable table;      // SwissTable core
};
```

- **CWISS_RawTable** is defined in `cwisstable.h` and has the following layout (platform‑dependent, but fixed for a given build):

    ```c
    typedef struct {
        CWISS_ControlByte *ctrl_;   // pointer to control bytes + backing memory
        char *slots_;                // pointer to slot array
        size_t size_;                // number of occupied slots
        size_t capacity_;            // total slot capacity (must be 2^k - 1)
        size_t growth_left_;         // slots that can be filled before rehash
    } CWISS_RawTable;
    ```

- `ctrl_` points to a memory block containing:

  - `capacity_` control bytes (one per slot)
  - 1 sentinel byte (`CWISS_kSentinel`)
  - `CWISS_NumClonedBytes()` cloned control bytes (for SIMD probing)
  - Padding for slot alignment
  - `capacity_` slots, each of size `policy->slot->size` (here `sizeof(ht_entry_t)`)

- The control byte values are:

  - `-128` : empty
  - `-2`   : deleted
  - `-1`   : occupied (with hash prefix in upper bits)
  - `0..127` : full slot with 7‑bit hash fragment (H2)

## Hash Function and Key Equality

- Hash function (`str_hash`):

  - Computes a 64‑bit hash using HighwayHash (either official `HighwayHash64` or built‑in scalar `hw_hash`).
  - Fixed global seeds are used; the hash is independent of the map instance.
  - Seeds are expanded into a 256‑bit key via XOR with fixed constants.
  - Input: a pointer to either:
    - a `const char*` (needle).
    - an `ht_entry_t` (stored entry) – the first 8 bytes are interpreted as the key pointer.

- Equality function (`str_eq`):

  - Compares the needle’s string (via `const char*`) with the stored entry’s key using strcmp.

## Memory Layout and Alignment

- `ht` structure:

  - `table`: offset `0`, size depends on `CWISS_RawTable` (usually 5 pointers = 40 bytes on 64‑bit).

  - No other fields; total size is exactly `sizeof(CWISS_RawTable)`.

- Backing memory block (allocated via `alloc_policy`):

  - Control bytes: `capacity_ + 1 + (kWidth - 1)` bytes, where `kWidth` is 16 (SSE2) or 8 (fallback).

  - Slots start at offset `CWISS_SlotOffset(capacity_, alignof(ht_entry_t))`.

  - Each slot is `sizeof(ht_entry_t)` bytes, currently 16 bytes on 64‑bit (two pointers).

- `ht_entry_t`:

    ```c
    typedef struct {
        char *key;   // owned copy of the key string
        void *value; // user value (not owned)
    } ht_entry_t;
    ```

  - `key` at offset 0, `value` at offset 8 (on 64‑bit).
  - The hash function relies on this layout: when passed a pointer to an entry, it reads the first 8 bytes as the key pointer.

## Thread Safety

- The library is thread‑safe for operations on distinct `ht` instances
- Concurrent operations on the same `ht` instance are not thread‑safe and must be protected by a mutex or other synchronization mechanism.
