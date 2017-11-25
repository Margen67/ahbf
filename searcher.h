#pragma once


struct Searcher {
    //ptrdiff_t unacceptable = 0;
    ptrdiff_t corr_result = -1;
    Program pgm[numi] = {};
    ptrdiff_t r[NIM + NSHIM + NARGS + numi] = {IMMEDS, SHIMMEDS};
    ptrdiff_t itrialx = 0;
    ptrdiff_t iend = NUM_INSNS_IN_ISA;
    #if NARGS == 2
    ptrdiff_t itrialy = 0;

    #endif
    inline void simulate_one_instruction(ptrdiff_t i);
    ptrdiff_t unacceptableHandler();
    ptrdiff_t check(ptrdiff_t i);

    inline void fix_operands(ptrdiff_t i);

    inline ptrdiff_t increment();
    void foundOne();

    ptrdiff_t search();

    void set_starting_instruction(ptrdiff_t x);
    void print_expr(int pr, int opn);
    void print_pgm(int pr);
};