#include "linkedlist.h"

/* LinkedListNode structure definition */
struct LinkedListNode
{
    void *data;
    struct LinkedListNode *prev;
    struct LinkedListNode *next;
};

/* LinkedList structure definition */
struct LinkedList
{
    LinkedListNode *head;
    LinkedListNode *tail;
    size_t size;
};

/* Internal node allocator using the configurable macro */
static LinkedListNode *allocate_node(void)
{
    LinkedListNode *new_node = (LinkedListNode *)LINKEDLIST_MALLOC(sizeof(LinkedListNode));
    if (new_node)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->data = NULL;
    }
    return new_node;
}

/* Public API */

LinkedList *linkedlist_init()
{
    LinkedList *list = (LinkedList *)LINKEDLIST_MALLOC(sizeof(LinkedList));
    if (!list)
        return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void linkedlist_free(LinkedList *list)
{
    if (!list)
        return;

    LinkedListNode *current = list->head;
    while (current)
    {
        LinkedListNode *next = current->next;
        LINKEDLIST_FREE(current);
        current = next;
    }

    LINKEDLIST_FREE(list);
}

void linkedlist_push(LinkedList *list, void *data)
{
    if (!list)
        return;

    LinkedListNode *new_node = allocate_node();
    if (!new_node)
        return;

    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail)
    {
        list->tail->next = new_node;
    }
    else
    {
        list->head = new_node;
    }

    list->tail = new_node;
    list->size++;
}

void linkedlist_fpush(LinkedList *list, void *data)
{
    if (!list)
        return;

    LinkedListNode *new_node = allocate_node();
    if (!new_node)
        return;

    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = list->head;

    if (list->head)
    {
        list->head->prev = new_node;
    }
    else
    {
        list->tail = new_node;
    }

    list->head = new_node;
    list->size++;
}

void linkedlist_pop(LinkedList *list)
{
    if (!list || !list->tail)
        return;

    LinkedListNode *to_remove = list->tail;

    list->tail = to_remove->prev;
    if (list->tail)
    {
        list->tail->next = NULL;
    }
    else
    {
        list->head = NULL;
    }

    LINKEDLIST_FREE(to_remove);
    list->size--;
}

void linkedlist_fpop(LinkedList *list)
{
    if (!list || !list->head)
        return;

    LinkedListNode *to_remove = list->head;

    list->head = to_remove->next;
    if (list->head)
    {
        list->head->prev = NULL;
    }
    else
    {
        list->tail = NULL;
    }

    LINKEDLIST_FREE(to_remove);
    list->size--;
}

void *linkedlist_npop(LinkedList *list, size_t index)
{
    if (!list || index >= list->size)
        return NULL;

    LinkedListNode *current = list->head;
    size_t current_index = 0;

    while (current)
    {
        if (current_index == index)
        {
            /* Unlink the node */
            if (current->prev)
                current->prev->next = current->next;
            else
                list->head = current->next; // Removing head

            if (current->next)
                current->next->prev = current->prev;
            else
                list->tail = current->prev; // Removing tail

            void *data = current->data;
            LINKEDLIST_FREE(current);
            list->size--;
            return;
        }

        current = current->next;
        current_index++;
    }

    return NULL;
}

LinkedListNode *linkedlist_get(const LinkedList *list, size_t index)
{
    if (!list || index >= list->size)
        return NULL; // not valid list or index out of bounds

    LinkedListNode *current = list->head;
    size_t current_index = 0;

    while (current)
    {
        if (current_index == index)
            return current;

        current = current->next;
        current_index++;
    }

    return NULL; // Should not reach here if index is valid, but return NULL just in case
}

bool linkedlist_is_empty(const LinkedList *list)
{
    return (list == NULL || list->size == 0);
}

size_t linkedlist_size(const LinkedList *list)
{
    return (list == NULL) ? 0 : list->size;
}

void *linkedlist_node_data(const LinkedListNode *node)
{
    return (node == NULL) ? NULL : node->data;
}

void *linkedlist_fpop_data(LinkedList *list)
{
    if (!list || !list->head)
        return NULL;

    LinkedListNode *to_remove = list->head;
    void *data = to_remove->data;

    list->head = to_remove->next;
    if (list->head)
    {
        list->head->prev = NULL;
    }
    else
    {
        list->tail = NULL;
    }

    LINKEDLIST_FREE(to_remove);
    list->size--;
    return data;
}

void *linkedlist_pop_data(LinkedList *list)
{
    if (!list || !list->tail)
        return NULL;

    LinkedListNode *to_remove = list->tail;
    void *data = to_remove->data;

    list->tail = to_remove->prev;
    if (list->tail)
    {
        list->tail->next = NULL;
    }
    else
    {
        list->head = NULL;
    }

    LINKEDLIST_FREE(to_remove);
    list->size--;
    return data;
}

void *linkedlist_fpeek(const LinkedList *list)
{
    return (list == NULL || list->head == NULL) ? NULL : list->head->data;
}

void *linkedlist_peek(const LinkedList *list)
{
    return (list == NULL || list->tail == NULL) ? NULL : list->tail->data;
}