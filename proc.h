#ifndef CSCI3431_PROC_H
#define CSCI3431_PROC_H

#include "program.h"

/*
 * Represents the possible states of a simulated process.
 */
enum ProcState
{
    STATE_NEW,
    STATE_READY,
    STATE_RUNNING,
    STATE_BLOCKED,
    STATE_TERMINATED
};

/*
 * Representation of a simulated process based on a set of instructions from a Program. Internal program counter is
 * split between a block counter 'bc' that tracks which instruction block is current along with a counter 'duration'
 * that tracks how many clock cycles remain to simulate execution within that block. Initial duration values are taken
 * from the internal Program representation.
 */
struct Proc
{
    int pid;                 // Assigned PID used to distinguish from other processes
    enum ProcState state;    // Current state of the process (assign using ALL_CAPS values given in enum above)
    struct Program *program; // Representation of a written program, given as a sequence of CPU burst durations
    int bc;                  // Block counter indicating the current instruction block being simulated
    int duration;            // Duration (time) remaining in current instruction block

    int admission_time;  // Clock time when the process was admitted to the ready queue
    int finish_time;     // Clock time when the process was terminated
    int total_wait_time; // Total time spent in the ready queue
    int last_ready_time; // Last time the process entered the ready queue
    int priority;        // Priority value (lower = higher priority)
    int priority_flag;   // 1 if priority was set via proc_nice(), 0 otherwise

    int predicted_burst; // Predicted next CPU burst (τₙ)
    int actual_burst;    // Actual last CPU burst (tₙ)
};

struct Proc *proc_create(struct Program *p);
int proc_advance_pc(struct Proc *p);
int proc_finished(struct Proc *p);
void proc_nice(struct Proc *proc, int priority);

#endif // CSCI3431_PROC_H
