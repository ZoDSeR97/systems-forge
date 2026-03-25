# Hashmap ABI Design

A Wrapper for Rust implementation of a hash map, designed to be used in C.

## ABI Principles

- **Stable C ABI:** All functions and types are exposed using the C ABI (`extern "C"`), ensuring compatibility across C, C++, Rust, and other languages.
- **Opaque Types:** The hash map is represented as an opaque pointer (`hmap*`) in C. Internal structure is hidden from consumers.
- **Explicit Memory Management:** Creation and destruction of the hash map are explicit. The API does not manage the memory of values stored in the map; this is the caller's responsibility.
- **Ownership & Lifetimes:**
  - The hash map instance is owned by the caller and must be destroyed with `hmap_destroy`.
  - Keys are copied into the map; callers may free key memory after insertion.
  - Values are stored as raw pointers; the caller is responsible for their allocation and deallocation.
- **Null Safety:** All API functions validate pointers and return error codes or null pointers on invalid input.
- **No Process Control:** The library does not terminate or control the process.

## C ABI

### Types

```c
typedef struct hmap hmap;
```

### Functions

```c
hmap *hmap_create(size_t initial_capacity);
int hmap_put(hmap *map, const char *key, void *value);
void *hmap_get(const hmap *map, const char *key);
int hmap_remove(hmap *map, const char *key);
void hmap_destroy(hmap *map);
```

## Memory Safety

- **Hash Map Memory:** Allocated and freed only via `hmap_create` and `hmap_destroy`.
- **Key Memory:** Copied into the map; caller may free after insertion.
- **Value Memory:** Not managed by the map; caller must ensure validity for the map's lifetime.
- **No Double Free:** Destroying the map does not free value pointers.
- **No Dangling Pointers:** Callers must not use value pointers after freeing them or after removing from the map.

## Error Handling

- All functions return error codes or null pointers on failure.
- Error codes are documented in the C header.

## Interoperability

- The ABI is stable and portable across C, C++, and Rust.
- No language-specific features are exposed at the ABI boundary.
- All cross-language calls go through the C ABI.

## Example Usage

```c
hmap *map = hmap_create(16);
int value = 123;
hmap_put(map, "key", &value);
int *retrieved = (int *)hmap_get(map, "key");
hmap_remove(map, "key");
hmap_destroy(map);
```

## Future Extensions

- Iteration API (with explicit iterator type)
- Custom value destructors (optional, for managed value memory)
- Thread safety (optional, via explicit API)
