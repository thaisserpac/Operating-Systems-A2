#ifndef CSCI3431_LINKEDLIST_H
#define CSCI3431_LINKEDLIST_H

#include "proc.h"

struct Node
{
    struct Node *next;
    struct Proc *proc;
};

struct List
{
    struct Node *head;
    struct Node *tail;
};

struct Node *node_create(struct Proc *proc);

struct List *list_create();
void list_add(struct List *list, struct Node *n);
int list_contains(struct List *list, int pid);
int list_remove(struct List *list, int pid);
struct Node *list_remove_head(struct List *list);

void list_print(struct List *list);

void list_insert_prio(struct List *list, struct Node *n);

#endif // CSCI3431_LINKEDLIST_H
