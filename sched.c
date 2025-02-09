#include <stdlib.h>
#include "sched.h"
#include "linkedlist.h"

static struct List *ready_q = NULL; // Use a single linked list for our ready queue

void sched_init()
{
    ready_q = list_create();
}

void sched_free()
{
    if (ready_q != NULL)
    {
        while (!sched_queue_empty())
        {
            sched_dequeue(0);
        }
        free(ready_q);
        ready_q = NULL;
    }
}

/*
 *  Function admits the passed Proc at the given clock time. This adds the
 *  given process to the internal ready queue by creating a new Node,
 *  updates the process state as necessary, and updates internal time tracking
 *  values as necessary.
 */
void sched_admit(struct Proc *proc, int clock)
{
    // Invalid Input
    if (proc == NULL || ready_q == NULL)
    {
        return;
    }

    // If priority is not set by proc_nice(), use SJF prediction
    if (proc->priority_flag == 0)
    {
        proc->priority = proc->predicted_burst;
    }

    proc->state = STATE_READY;
    proc->last_ready_time = clock;

    struct Node *node = node_create(proc);
    list_insert_prio(ready_q, node);
}

/*
 *  Terminate the passed Proc by updating its internal state and time tracking
 *  values as necessary
 */
void sched_terminate(struct Proc *proc, int clock)
{
    // Invalid Input
    if (proc == NULL)
    {
        return;
    }

    // Update process state and time info
    proc->state = STATE_TERMINATED;
    proc->finish_time = clock;
}

/*
 * Dequeues and returns the next process from the ready queue. If the ready
 * queue is empty then the function returns NULL.
 */
struct Proc *sched_dequeue(int clock)
{
    // Invalid Input or Empty Queue
    if (ready_q == NULL || ready_q->head == NULL)
    {
        return NULL;
    }

    // Remove the head node from the ready queue
    struct Node *node = list_remove_head(ready_q);
    // No process
    if (node == NULL)
    {
        return NULL;
    }

    struct Proc *proc = node->proc;

    // Update process and time info
    proc->state = STATE_RUNNING;
    proc->total_wait_time += (clock - proc->last_ready_time);

    // Deallocate memory
    free(node);

    return proc;
}

/* Returns 1 if the ready queue is not yet initialized or if the ready queue has
 *  no elements and 0 otherwise
 */
int sched_queue_empty()
{
    // Queue is not yet initialized or has no elements
    if (ready_q == NULL || ready_q->head == NULL)
    {
        return 1;
    }
    // Otherwise
    return 0;
}
