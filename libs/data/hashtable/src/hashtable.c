#include "hashtable.h"
#include "cwisstable.h"
#include "highwayhash.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ---------------------------------------------------------------------
// Fixed hash seeds (digits of pi)
// ---------------------------------------------------------------------
static const uint64_t kHashSeed0 = 0xbea8f0c8f6c7f0d1ULL;
static const uint64_t kHashSeed1 = 0xab7c6f9e8f0c2b4aULL;
static const uint64_t kHashSeed2 = 0x123456789abcdef0ULL; // (arbitrary)
static const uint64_t kHashSeed3 = 0xfedcba9876543210ULL; // (arbitrary)

// ---------------------------------------------------------------------
// Entry stored in the map
// ---------------------------------------------------------------------
typedef struct
{
    char *key;   // owned copy of the key string
    void *value; // user value (not owned)
} ht_entry_t;

// ---------------------------------------------------------------------
// Portable strdup
// ---------------------------------------------------------------------
static char *ht_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *p = (char *)malloc(len);
    if (p) memcpy(p, s, len);
    return p;
}

// ---------------------------------------------------------------------
// Hash function for a C string – uses fixed seeds.
// The argument is either a pointer to a const char* (needle) or
// a pointer to an ht_entry_t (stored entry).
// In both cases, reading the first 8 bytes yields the string pointer.
// ---------------------------------------------------------------------
static size_t str_hash(const void *p)
{
    const char *str = *(const char **)p;
    uint64_t key[4] = {
        kHashSeed0,
        kHashSeed1,
        kHashSeed2 ^ 0xbea8f0c8f6c7f0d1ULL,
        kHashSeed3 ^ 0xab7c6f9e8f0c2b4aULL
    };
    return (size_t)HighwayHash64((const uint8_t *)str, strlen(str), key);
}

// ---------------------------------------------------------------------
// Equality: needle is a pointer to const char*, entry is ht_entry_t*
// ---------------------------------------------------------------------
static bool str_eq(const void *needle, const void *entry)
{
    const char *key = *(const char **)needle;
    const ht_entry_t *e = (const ht_entry_t *)entry;
    return strcmp(key, e->key) == 0;
}

// ---------------------------------------------------------------------
// Slot policy functions
// ---------------------------------------------------------------------
static void slot_init(void *slot)
{
    (void)slot;
}

static void slot_dtor(void *slot)
{
    ht_entry_t *e = (ht_entry_t *)slot;
    if (e->key)
    {
        free(e->key);
        e->key = NULL;
    }
}

static void slot_transfer(void *dst, void *src)
{
    memcpy(dst, src, sizeof(ht_entry_t));
}

static void *slot_get(void *slot)
{
    return slot;
}

// ---------------------------------------------------------------------
// Object policy (only used for size/align)
// ---------------------------------------------------------------------
static void obj_copy(void *dst, const void *src)
{
    memcpy(dst, src, sizeof(ht_entry_t));
}

static const CWISS_ObjectPolicy obj_policy = {
    .size = sizeof(ht_entry_t),
    .align = alignof(ht_entry_t),
    .copy = obj_copy,
    .dtor = NULL,
};

// ---------------------------------------------------------------------
// Key policy
// ---------------------------------------------------------------------
static const CWISS_KeyPolicy key_policy = {
    .hash = str_hash,
    .eq = str_eq,
};

// ---------------------------------------------------------------------
// Allocator policy (default malloc/free)
// ---------------------------------------------------------------------
static const CWISS_AllocPolicy alloc_policy = {
    .alloc = CWISS_DefaultMalloc,
    .free = CWISS_DefaultFree,
};

// ---------------------------------------------------------------------
// Slot policy
// ---------------------------------------------------------------------
static const CWISS_SlotPolicy slot_policy = {
    .size = sizeof(ht_entry_t),
    .align = alignof(ht_entry_t),
    .init = slot_init,
    .del = slot_dtor,
    .transfer = slot_transfer,
    .get = slot_get,
};

// ---------------------------------------------------------------------
// Complete policy for the hash map
// ---------------------------------------------------------------------
static const CWISS_Policy ht_policy = {
    .obj = &obj_policy,
    .key = &key_policy,
    .alloc = &alloc_policy,
    .slot = &slot_policy,
};

// ---------------------------------------------------------------------
// Opaque map definition
// ---------------------------------------------------------------------
struct ht
{
    CWISS_RawTable table;
};

// ---------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------
ht *ht_create(size_t initial_capacity)
{
    ht *map = (ht *)malloc(sizeof(ht));
    if (!map) return NULL;

    map->table = CWISS_RawTable_new(&ht_policy, initial_capacity);
    return map;
}

void ht_destroy(ht *map)
{
    if (!map) return;
    CWISS_RawTable_destroy(&ht_policy, &map->table);
    free(map);
}

int ht_put(ht *map, const char *key, void *value)
{
    if (!map || !key) return HT_ERR_NULL;

    // Duplicate key first – fail early if out of memory.
    char *key_copy = ht_strdup(key);
    if (!key_copy) return HT_ERR_NOMEM;

    CWISS_PrepareInsert res = CWISS_RawTable_FindOrPrepareInsert(
        &ht_policy,
        &key_policy,
        &map->table,
        &key
    );

    if (!res.inserted)
    {
        // Key already exists – update
        void *slot = map->table.slots_ + res.index * ht_policy.slot->size;
        ht_entry_t *entry = (ht_entry_t *)ht_policy.slot->get(slot);
        free(entry->key);
        entry->key = key_copy;
        entry->value = value;
    }
    else
    {
        // New key – insert
        void *slot = CWISS_RawTable_PreInsert(&ht_policy, &map->table, res.index);
        ht_entry_t *entry = (ht_entry_t *)slot;
        entry->key = key_copy;
        entry->value = value;
    }
    return HT_OK;
}

void *ht_get(const ht *map, const char *key)
{
    if (!map || !key) return NULL;

    size_t hash = str_hash(&key);
    CWISS_RawIter it = CWISS_RawTable_find_hinted(
        &ht_policy,
        &key_policy,
        (CWISS_RawTable *)&map->table,
        &key, hash
    );

    if (it.slot_ == NULL) return NULL;
    ht_entry_t *entry = (ht_entry_t *)CWISS_RawIter_get(&ht_policy, &it);
    return entry->value;
}

int ht_contains(const ht *map, const char *key)
{
    if (!map || !key) return 0;
    size_t hash = str_hash(&key);
    CWISS_RawIter it = CWISS_RawTable_find_hinted(
        &ht_policy,
        &key_policy,
        (CWISS_RawTable *)&map->table,
        &key, hash
    );
    return (it.slot_ != NULL);
}

int ht_remove(ht *map, const char *key)
{
    if (!map || !key) return HT_ERR_NULL;

    size_t hash = str_hash(&key);
    CWISS_RawIter it = CWISS_RawTable_find_hinted(
        &ht_policy,
        &key_policy,
        &map->table,
        &key, hash
    );

    if (it.slot_ == NULL) return HT_ERR_NOTFOUND;
    CWISS_RawTable_erase_at(&ht_policy, it);
    return HT_OK;
}