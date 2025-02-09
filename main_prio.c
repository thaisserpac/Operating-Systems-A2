#include <stdio.h>
#include <stdlib.h>
#include "program.h"
#include "proc.h"
#include "sched.h"

struct Proc **generate_processes();
int test_basics();

int main()
{
    struct Proc *active, *swap;
    int clock = 0;

    struct Proc **p = generate_processes(); // Array of pointers to struct Proc!

    if (test_basics() > 0)
    {
        return 0;
    }

    /*
     * ---------- Begin main simulation ----------
     */

    sched_init();

    // Admit all processes into the scheduling system
    for (int i = 0; i < 4; i++)
    {
        sched_admit(p[i], clock);
    }

    // Determine the process that will first be active
    active = sched_dequeue(clock);

    // Loop until ready_q is empty and there is no active process
    while (!sched_queue_empty() || active != NULL)
    {
        clock++;

        // Modify the next line to also print the PID of the currently active process
        printf("[%03d]\t Running P(%02d)\n", clock, active->pid);

        // Advance PC of active process
        if (proc_advance_pc(active))
        {
            // If we have advanced to next instruction block, first:

            // Copy pointer to active process into swap
            swap = active;

            // Then determine next active process and over-write pointer to active
            active = sched_dequeue(clock);

            // Finally, check if previously active process is finished: terminate if so, otherwise re-admit
            if (proc_finished(swap))
            {
                sched_terminate(swap, clock);
            }
            else
            {
                sched_admit(swap, clock);
                if (active == NULL)
                { // Edge case: this is the only process left
                    active = sched_dequeue(clock);
                }
            }

            swap = NULL; // Swap pointer should not be used after here
        }
    }

    /*
     * ---------- End main simulation ----------
     */

    printf("Final clock: %d\n", clock);
    for (int i = 0; i < 4; i++)
    {
        /*
         *  Modified next line to use PID of process, finish time (on clock),
         *  total wait time (in ready queue), and average wait time
         *  (time spent waiting in ready queue between active periods)
         */
        double avg_wait = (double)p[i]->total_wait_time / p[i]->program->num_blocks;
        printf("P(%02d)\nFinish time: %d, Total Wait: %d, Avg. Wait: %lf\n\n",
               p[i]->pid, p[i]->finish_time, p[i]->total_wait_time, avg_wait);
    }

    double total_avg = 0.0;
    for (int i = 0; i < 4; i++)
    {
        total_avg += (double)p[i]->total_wait_time / p[i]->program->num_blocks;
    }
    total_avg /= 4;
    printf("Average wait times across all processes: %lf\n", total_avg);

    return 0;
}

struct Proc **generate_processes()
{
    int p1[5] = {5, 4, 9, 5, 9};
    int p2[5] = {2, 2, 2, 2, 2};
    int p3[7] = {2, 3, 3, 3, 3, 3, 2};
    int p4[5] = {1, 1, 1, 1, 1};

    struct Proc **p = calloc(4, sizeof(struct Proc *)); // Array of pointers to struct Proc!
    p[0] = proc_create(program_new(p1, 5));
    p[1] = proc_create(program_new(p2, 5));
    p[2] = proc_create(program_new(p3, 7));
    p[3] = proc_create(program_new(p4, 5));

    proc_nice(p[0], 3);
    proc_nice(p[1], 1);
    proc_nice(p[2], 4);
    proc_nice(p[3], 3);

    return p;
}

int test_basics()
{
    struct Proc **p = calloc(2, sizeof(struct Proc *));
    struct Proc *q;
    struct Proc *r;

    int p1[3] = {1, 10, 1};
    int p2[1] = {5};

    p[0] = proc_create(program_new(p1, 3));
    p[1] = proc_create(program_new(p2, 1));

    // Sanity check empty ready queue
    if (!sched_queue_empty() || sched_dequeue(1) != NULL)
    {
        printf("Problem: non-empty schedule queue should be empty.\n");
        sched_free();
        free(p[0]);
        free(p[1]);
        free(p);
        return 1;
    }

    sched_init();
    sched_admit(p[0], 0);
    sched_admit(p[1], 0);
    sched_free();

    // Sanity check empty ready queue
    if (!sched_queue_empty() || sched_dequeue(1) != NULL)
    {
        printf("Problem: non-empty schedule queue should be empty.\n");
        sched_free();
        free(p[0]);
        free(p[1]);
        free(p);
        return 2;
    }

    sched_init();
    sched_admit(p[0], 0);
    sched_admit(p[1], 0);
    q = sched_dequeue(1);
    proc_advance_pc(q);
    sched_admit(q, 1);
    q = sched_dequeue(2);
    proc_advance_pc(q);
    sched_admit(q, 2);
    q = sched_dequeue(3);
    r = sched_dequeue(3);

    // Sanity check program counters
    if (q->bc != 1 || q->duration != 9 || r->bc != 0 || r->duration != 5)
    {
        printf("Problem: program counters not advancing as expected.\n");
        sched_free();
        free(p[0]);
        free(p[1]);
        free(p);
        return 3;
    }

    sched_free();
    free(p[0]);
    free(p[1]);
    free(p);
    return 0;
}