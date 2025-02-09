#ifndef CSCI3431_PROGRAM_H
#define CSCI3431_PROGRAM_H

/*
 * A Program struct stores a representation of a written program. Each entry in the integer array represents a block of
 * CPU instructions that require simulated execution with the integer value giving the number of CPU clock cycles
 * required to finish the block. In between instruction blocks, the program will need to block for 0 cycles - that is,
 * when simulating its execution, we should remove it from active execution but spend no time with it in the Blocked
 * state.
 */
struct Program
{
    int *blocks;    // Instruction blocks
    int num_blocks; // Number of instruction blocks
};

struct Program *program_new(int inst[], int num_blocks);

#endif // CSCI3431_PROGRAM_H
