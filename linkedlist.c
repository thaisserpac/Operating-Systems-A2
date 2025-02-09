#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct List *list_create()
{
    struct List *new = calloc(1, sizeof(struct List));
    new->head = NULL;
    new->tail = NULL;
    return new;
}

void list_add(struct List *list, struct Node *n)
{
    if (list->head == NULL)
    {
        list->head = n;
    }
    else
    {
        list->tail->next = n;
    }
    list->tail = n;
}

void list_print(struct List *list)
{
    struct Node *n = list->head;
    while (n != NULL)
    {
        printf("%d ", n->proc->pid);
        n = n->next;
    }
    printf("\n");
}

struct Node *node_create(struct Proc *proc)
{
    struct Node *node = calloc(1, sizeof(struct Node));

    // Invalid input
    if (node == NULL)
    {
        return NULL;
    }

    node->proc = proc;
    return node;
}

int list_contains(struct List *list, int pid)
{

    // Invalid Input
    if (list == NULL || list->head == NULL)
    {
        return 0;
    }

    struct Node *current = list->head;
    while (current != NULL)
    {

        // Process matching PID
        if (current->proc->pid == pid)
        {
            return 1;
        }
        current = current->next;
    }

    // PID not found
    return 0;
}

int list_remove(struct List *list, int pid)
{
    // Invalid input
    if (list == NULL || list->head == NULL)
    {
        return 0;
    }

    struct Node *current = list->head;
    struct Node *previous = NULL;

    while (current != NULL)
    {
        if (current->proc->pid == pid)
        {
            // Remove head node
            if (previous == NULL)
            {
                list->head = current->next;
                if (list->head == NULL)
                {
                    list->tail = NULL; // List is now empty
                }
            }
            else
            {
                // Node to be removed is not head node
                previous->next = current->next;
                // Update tail node
                if (current->next == NULL)
                {
                    list->tail = previous;
                }
            }

            free(current);
            return 1;
        }

        previous = current;
        current = current->next;
    }

    // Matching PID not found
    return 0;
}

struct Node *list_remove_head(struct List *list)
{
    // Invalid input
    if (list == NULL || list->head == NULL)
    {
        return NULL;
    }

    struct Node *head = list->head;
    // Update head node
    list->head = head->next;

    // Set an empty list
    if (list->head == NULL)
    {
        list->tail = NULL;
    }

    head->next = NULL;
    return head;
}

/*
 *  inserts a passed Node at the first position (starting at the head) within
 *  the linked list where the next Node has a priority value that is strictly
 *  greater (so has a lower priority), or at the tail if no such position exists
 */
void list_insert_prio(struct List *list, struct Node *n)
{
    // Invalid input
    if (list == NULL || n == NULL)
        return;

    struct Node *current = list->head;
    struct Node *prev = NULL;

    // loop until the current node's priority is higher than the new node's
    while (current != NULL && current->proc->priority <= n->proc->priority)
    {
        prev = current;
        current = current->next;
    }

    if (prev == NULL)
    { // Insert at head
        n->next = list->head;
        list->head = n;
        if (list->tail == NULL)
            list->tail = n;
    }
    else
    { // Insert after prev
        prev->next = n;
        n->next = current;
        if (current == NULL)
            // Update tail if necessary
            list->tail = n;
    }
}