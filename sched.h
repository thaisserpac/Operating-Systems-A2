#ifndef CSCI3431_SCHED_H
#define CSCI3431_SCHED_H

#include "proc.h"

void sched_init();
void sched_admit(struct Proc *p, int clock);
void sched_terminate(struct Proc *p, int clock);
struct Proc *sched_dequeue(int clock);
int sched_queue_empty();
void sched_free();

#endif // CSCI3431_SCHED_H
