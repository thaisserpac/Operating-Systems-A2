#include <stdlib.h>
#include "proc.h"

static int latest_pid = 0; // Use a single pool of valid PIDs

/*
 * Allocates and returns a new Proc representing a process based on the passed Program. Initializes internal values.
 */
struct Proc *proc_create(struct Program *p)
{

    struct Proc *proc = calloc(1, sizeof(struct Proc));

    // Invalid Input
    if (proc == NULL)
    {
        return NULL; // Handle memory allocation failure
    }

    proc->state = STATE_NEW; // Use enum value to initialize state
    proc->program = p;
    proc->pid = ++latest_pid;
    proc->bc = 0;

    proc->duration = (p->num_blocks > 0) ? p->blocks[0] : 0;

    proc->predicted_burst = proc->duration;
    proc->actual_burst = 0;
    proc->priority = proc->predicted_burst;
    proc->priority_flag = 0;
    return proc;
}

/*
 * Advance the internal program counter of passed Proc 'p'. The effect of calling this function is to decrease the
 * remaining 'duration' for the current instruction block by 1. When the remaining duration hits 0, the program counter
 * is advanced to the next instruction block and the duration re-initialized to the duration for that block. This
 * function returns 1 if the current instruction block finished or 0 if the current instruction block has positive
 * duration remaining.
 */
int proc_advance_pc(struct Proc *p)
{
    // Invalid input
    if (p == NULL || p->program == NULL)
    {
        return 0;
    }

    if (p->duration > 0)
    {
        p->duration--;
    }

    if (p->duration == 0)
    {
        // Record burst time
        p->actual_burst = p->program->blocks[p->bc];

        // Update predicted burst
        p->predicted_burst = 0.5 * p->actual_burst + 0.5 * p->predicted_burst;

        // Next instruction block
        p->bc++;

        // Check if all instruction blocks have finished
        if (p->bc >= p->program->num_blocks)
        {
            p->state = STATE_TERMINATED;
            return 1; // Process has finished execution
        }

        p->duration = p->program->blocks[p->bc];
        // Block finished
        return 1;
    }

    return 0;
}

/*
 * Checks if the passed Proc 'p' is finished executing all of its instruction blocks. Returns 1 if all instruction
 * blocks have finished executing or 0 if some instruction blocks still have positive duration remaining.
 */
int proc_finished(struct Proc *p)
{

    // Invalid input
    if (p == NULL || p->program == NULL)
    {
        return 1;
    }

    if (p->bc >= p->program->num_blocks)
    {
        // Finished process
        return 1;
    }

    return 0;
}

/*
 *  Function modifies the priority of the given process to the given value
 */
void proc_nice(struct Proc *proc, int priority)
{
    // Invalid input
    if (proc == NULL)
    {
        return;
    }
    proc->priority = priority;
    proc->priority_flag = 1;
}
