#ifndef HTABLE_H
#define HTABLE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Opaque hash map type */
    typedef struct ht ht;

    /** Return codes */
    #define HT_OK 0            /**< success */
    #define HT_ERR_NULL -1     /**< null argument (map or key) */
    #define HT_ERR_NOMEM -2    /**< memory allocation failed */
    #define HT_ERR_NOTFOUND -3 /**< key not found */

    /**
     * Creates a new hash map with the given initial capacity.
     * @param initial_capacity  hint for the number of buckets (0 = default)
     * @return  pointer to the new map, or NULL on allocation failure
     */
    ht *ht_create(size_t initial_capacity);

    /**
     * Destroys the map and frees all copied keys. Does not free the values.
     */
    void ht_destroy(ht *map);

    /**
     * Inserts or updates a key‑value pair. The key is copied internally.
     * @param map    target map
     * @param key    NUL‑terminated string (must not be NULL)
     * @param value  arbitrary pointer (may be NULL)
     * @return       HT_OK on success, or a negative error code.
     */
    int ht_put(ht *map, const char *key, void *value);

    /**
     * Retrieves the value associated with a key.
     * @return  pointer to the value, or NULL if the key is not found.
     *          NULL may also indicate that the value itself is NULL;
     *          use ht_contains() to disambiguate if necessary.
     */
    void *ht_get(const ht *map, const char *key);

    /**
     * Checks whether a key exists in the map.
     * @return  true if key exists, false otherwise.
     */
    int ht_contains(const ht *map, const char *key);

    /**
     * Removes a key‑value pair from the map.
     * @param map  target map
     * @param key  key to remove
     * @return     HT_OK if found and removed,
     *             HT_ERR_NULL if map or key is NULL,
     *             HT_ERR_NOTFOUND if key not found.
     */
    int ht_remove(ht *map, const char *key);

#ifdef __cplusplus
}
#endif

#endif