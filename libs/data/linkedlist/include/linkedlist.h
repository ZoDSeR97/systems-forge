#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Opaque linked list type */
    typedef struct LinkedListNode LinkedListNode;
    typedef struct LinkedList LinkedList;

    /**
     * Allocates a new node for the linked list.
     * @param list Pointer to the linked list.
     * @return Pointer to the allocated node, or NULL on failure.
     */
    static LinkedListNode *allocate_node(LinkedList *list);

    /**
     * Initializes a linked list with the given allocator.
     * @param list Pointer to linkedlist structure to initialize.
     * @param allocator Pointer to arena allocator, or NULL to use malloc.
     *                  When using arena, caller is responsible for arena lifecycle.
     * @return Pointer to initialized list (same as list parameter).
     */
    LinkedList *linkedlist_init(LinkedList *list, void *allocator);

    /**
     * Frees the linked list and all nodes (when using malloc).
     * When allocator==NULL, all nodes are freed.
     * When allocator!=NULL (arena), only list metadata is cleared;
     * caller is responsible for freeing the arena.
     * Does not free user data pointed to by nodes.
     */
    void linkedlist_free(LinkedList *list);

    /**
     * Adds an item to the end of the list. O(1)
     */
    void linkedlist_push(LinkedList *list, void *data);

    /**
     * Adds an item to the start of the list. O(1)
     */
    void linkedlist_fpush(LinkedList *list, void *data);

    /**
     * Removes the last item from the list. O(1)
     */
    void linkedlist_pop(LinkedList *list);

    /**
     * Removes the first item from the list. O(1)
     */
    void linkedlist_fpop(LinkedList *list);

    /**
     * Removes the item at the specified index (0-based). O(n)
     * Returns the removed node, or NULL if index is out of bounds or list is NULL.
     */
    void *linkedlist_npop(LinkedList *list, size_t index);

    /**
    * Gets the item at the specified index (0-based). O(n)
    * Returns NULL if index is out of bounds or list is NULL.
    */
    LinkedListNode *linkedlist_get(const LinkedList *list, size_t index);

    /**
    * Checks if the list is empty. O(1)
    */
    bool linkedlist_is_empty(const LinkedList *list);

    /**
     * Returns the number of items in the list. O(1)
     */
    size_t linkedlist_size(const LinkedList *list);

#ifdef __cplusplus
}
#endif

#endif
