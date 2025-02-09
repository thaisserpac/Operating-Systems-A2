#include <stdlib.h>
#include <string.h>
#include "program.h"

/*
 * Create and return a new program strut based on the passed integer array parameter 'inst' with number of entries
 * given by 'num_blocks'. The entry in the integer array will represent blocks of CPU instructions with the value
 * specifying the length of time needed to execute that block.
 */
struct Program *program_new(int inst[], int num_blocks)
{
    struct Program *p = calloc(1, sizeof(struct Program));
    p->num_blocks = num_blocks;
    p->blocks = calloc(num_blocks, sizeof(int));
    memcpy(p->blocks, inst, num_blocks * sizeof(int)); // Copies memory contents from 'inst' into 'blocks'
    return p;
}
