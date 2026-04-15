#include "linkedlist.h"
#include <stdlib.h>
#include <stdbool.h>

struct LinkedListNode
{
    void *data;
    struct LinkedListNode *prev;
    struct LinkedListNode *next;
};

struct LinkedList
{
    LinkedListNode *head;
    LinkedListNode *tail;
    size_t size;
    void *allocator; // Points to Arena if using Arena, NULL if using malloc
};

LinkedList *linkedlist_init(LinkedList *list, void *allocator)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->allocator = allocator;
    return list;
}

static LinkedListNode *allocate_node(LinkedList *list)
{
    LinkedListNode *new_node = NULL;
    if (list->allocator != NULL)
    {
        new_node = (LinkedListNode *)arena_malloc(list->allocator, sizeof(LinkedListNode), 0);
    }
    else
    {
        new_node = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    }

    if (new_node)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->data = NULL;
    }
    return new_node;
}

void linkedlist_free(LinkedList *list)
{
    if (!list)
        return;

    if (list->allocator == NULL)
    {
        LinkedListNode *current = list->head;
        while (current)
        {
            LinkedListNode *next = current->next;
            if (list->allocator == NULL)
            {
                free(current);
            }
            current = next;
        }
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void linkedlist_push(LinkedList *list, void *data)
{
    if (!list)
        return;

    LinkedListNode *new_node = allocate_node(list);
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

    LinkedListNode *new_node = allocate_node(list);
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

    if (list->allocator == NULL)
    {
        free(to_remove);
    }

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

    if (list->allocator == NULL)
    {
        free(to_remove);
    }

    list->size--;
}

void *linkedlist_npop(LinkedList *list, size_t index)
{
    if (!list || index >= list->size)
        return; // not valid list or index out of bounds

    LinkedListNode *current = list->head;
    size_t current_index = 0;

    while (current)
    {
        if (current_index == index)
        {
            // Unlink the node
            if (current->prev)
                current->prev->next = current->next;
            else
                list->head = current->next; // Removing head

            if (current->next)
                current->next->prev = current->prev;
            else
                list->tail = current->prev; // Removing tail

            void *data = current->data;

            if (list->allocator == NULL)
            {
                free(current);
            }

            list->size--;
            return;
        }

        current = current->next;
        current_index++;
    }

    return; // Should not reach here if index is valid, but return NULL just in case
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
