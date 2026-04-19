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
     * Allocates and initializes a new linked list with the given allocator.
     * @return Pointer to initialized list.
     */
    LinkedList *linkedlist_init();

    /**
     * Frees the linked list structure and all nodes.
     * @param list Pointer to the linked list
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

    /**
     * Gets the data pointer from a node. O(1)
     * @param node Pointer to LinkedListNode (from linkedlist_get)
     * @return Data pointer stored in the node, or NULL if node is NULL
     */
    void *linkedlist_node_data(const LinkedListNode *node);

    /**
     * Removes first item and returns its data. O(1)
     * @param list Pointer to the linked list
     * @return Data from the first node, or NULL if list is empty
     */
    void *linkedlist_fpop_data(LinkedList *list);

    /**
     * Removes last item and returns its data. O(1)
     * @param list Pointer to the linked list
     * @return Data from the last node, or NULL if list is empty
     */
    void *linkedlist_pop_data(LinkedList *list);

    /**
     * Returns data from first item without removing. O(1)
     * @param list Pointer to the linked list
     * @return Data from the first node, or NULL if list is empty
     */
    void *linkedlist_fpeek(const LinkedList *list);

    /**
     * Returns data from last item without removing. O(1)
     * @param list Pointer to the linked list
     * @return Data from the last node, or NULL if list is empty
     */
    void *linkedlist_peek(const LinkedList *list);

#ifdef __cplusplus
}
#endif

#endif
