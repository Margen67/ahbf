#pragma once

struct Program {                        // The current program.
    unsigned int op;                      // Index into isa.
    unsigned int opnd[3];                 // Operands of op.  Register numbers
    // except if negative, it's the negative
    // of a shift amount.
};