#pragma once

#define NARGS 2                // Number of args in userfun, 1 or 2.


thread_local ptrdiff_t unacceptable = 0;

/* A note about the registers:

They are divided into four groups.  The first group, starting with
register 0, holds ordinary immediate values.  The second group, starting
with register NIM, holds the shift immediate values.  The next 1 or 2
regs are the arguments to the user-defined function.  The last group
holds the results of computations done by the trial programs.

0        Start of ordinary immediate values (those given by IMMEDS)
NIM      Start of shift immediate values    (those given by SHIMMEDS)
RX       First (or only) user function argument
RY       Second user function argument
RI0      Result of instruction 0 goes here
RI0 + i  Result of instruction i goes here
where:
NIM   = number of ordinary immediate values
NSHIM = number of shift immediate values
*/

#define MAXNEG 0x80000000
#define MAXPOS 0x7FFFFFFF
#define NBSM 31                // Shift mask.  Use 63 for mod 64
// shifts, or 31 for mod 32.

#include "trials.h"

// First three values of IMMEDS must be 0, -1, and 1.
#define IMMEDS 0, -1, 1, MAXNEG, -2, 2, 3, 0x55555555, 0x0F0F0F0F,0x33333333, 0xFF, 0xFF00, \
0xFF0000, 0xFF000000, 0x7F7F7F7F, 0x7F800000, 0x0f0f0f0f, 0xf0f0f0f0, 0xcccccccc, 0xff00ff00, \
0x00ff00ff, 0xaaaaaaaa, 0x007fffff, 0x07060504, 0x03020100

#define SHIMMEDS 1, 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29, 30, 31

static constexpr int dummy1[] = {IMMEDS};        // These get optimized out of existence.
static constexpr int dummy2[] = {SHIMMEDS};

#define NIM (ptrdiff_t)(sizeof(dummy1)/sizeof(dummy1[0]))
#define NSHIM (ptrdiff_t)(sizeof(dummy2)/sizeof(dummy2[0]))

#define RX (NIM + NSHIM)        // First (or only) user function argument
#define RY (RX + 1)             // Second user function argument
#define RI0 (RX + NARGS)        // Result of instruction 0 goes here



//static ptrdiff_t unacceptable;               // Code below sets this to 1 for an
// unacceptable operation, such as
// divide by 0.  It is initially 0.


#include "isa.h"

/* ------------------- End of user-setup Portion -------------------- */

static constexpr const ptrdiff_t numi = 5;                       // Current size of the trial programs,
// must be from 1 to MAXNUMI.

#if NARGS == 1

int userfun(int);

#else
int userfun(int, int);
#endif



#if NARGS == 1
static ptrdiff_t correct_result[NTRIALX];
#else
static ptrdiff_t correct_result[NTRIALX][NTRIALY];
#endif


#include "program.h"

//static ptrdiff_t corr_result;                // Correct result for current trial.
//static Program pgm[MAXNUMI];


/* GPR array:  First NIM slots hold ordinary immediate values (IMMEDS),
next NSHIM slots hold shift immediate values (SHIMMEDS), next NARGS
slots hold the arguments x and, optionally, y, and the last numi slots
hold the result of instructions 0 through numi - 1. */

//static ptrdiff_t r[NIM + NSHIM + NARGS + MAXNUMI] = {IMMEDS, SHIMMEDS};

#include "searcher.h"