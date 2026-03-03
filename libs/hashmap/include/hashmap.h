#ifndef HMAP_H
#define HMAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Opaque type representing a hash map.
     */
    typedef struct hmap hmap;

    /**
     * @brief Creates a hash map with the specified initial capacity.
     * @param initial_capacity The initial number of buckets in the hash map.
     * @return Pointer to the created hash map, or NULL on failure.
     */
    hmap *hmap_create(size_t initial_capacity);

    /**
     * @brief Inserts a key-value pair into the hash map. If the key already exists, its value is updated.
     * @param map Pointer to the hash map.
     * @param key Null-terminated string key (must not be NULL).
     * @param value Pointer to the value associated with the key (can be NULL).
     * @return 0 on success, non-zero on failure (e.g., memory allocation failure).
     */
    int hmap_put(hmap *map, const char *key, void *value);

    /**
     * @brief Retrieves the value associated with a given key in the hash map.
     * @param map Pointer to the hash map.
     * @param key Null-terminated string key to look up (must not be NULL).
     * @return Pointer to the value associated with the key, or NULL if the key is not found.
     */
    void *hmap_get(const hmap *map, const char *key);

    /**
     * @brief Removes a key-value pair from the hash map by key.
     * @param map Pointer to the hash map.
     * @param key Null-terminated string key to remove (must not be NULL).
     * @return 0 if the key was found and removed, non-zero if the key was not found.
     */
    int hmap_remove(hmap *map, const char *key);

    /**
     * @brief Frees all memory used by the hash map. Does not free memory pointed to by values.
     * @param map Pointer to the hash map to destroy.
     */
    void hmap_destroy(hmap *map);
#ifdef __cplusplus
}
#endif

#endif