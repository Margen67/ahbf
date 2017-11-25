#pragma once

/* Collection of simulator routines for the instructions in the isa. */
static int neg(int x, int y, int z) { return -x; }

static int _not(int x, int y, int z) { return ~x; }

static int pop(int xx, int y, int z) {
    return __builtin_popcount(xx);
}

static int nlz(int xx, int y, int z) {
    return __builtin_clz(xx);
}


static int rbyte(int x, int y, int z) {
    return __builtin_bswap32(x);
}

static int rbyte16(int x, int y, int z) {
    return ((unsigned int) __builtin_bswap16((unsigned short) x)) | (x & 0xFFFF0000);
}

static int movzxb(int x, int y, int z) {
    return (unsigned int) static_cast<unsigned char>(x);
}

static int movzxw(int x, int y, int z) {
    return (unsigned int) static_cast<unsigned short>(x);
}

static int movsxw(int x, int y, int z) {
    return (signed int) static_cast<signed short>(x);
}

static int movsxb(int x, int y, int z) {
    return (signed int) static_cast<signed char>(x);
}

static int floatrep(int x, int y, int z) {
    float f = x;
    return *(int *) &f;
}

static int asFloat(int x, int y, int z) {
    return *(float*)&x;
}

static int lodbl(int x, int y, int z) {
    double f = x;
    return *(int *) &f;
}

static int hidbl(int x, int y, int z) {
    double f = x;
    return ((int *) &f)[1];
}

static int bzhi(int x, int y, int z) {
    return __builtin_ia32_bzhi_si(x, y);
}

static int pdep(int x, int y, int z) {
    return __builtin_ia32_pdep_si(x, y);
}

static int pext(int x, int y, int z) {
    return __builtin_ia32_pext_si(x, y);
}

static int highmul(int x, int y, int z) {
    return (((unsigned long long) x) * ((unsigned long long) y)) >> 32UL;
}

static int lzcnt(int x, int y, int z) {
    return __builtin_ia32_lzcnt_u32(x);
}

static int cmpx86(int x, int y, int z) {
    unsigned char ah;

    __asm__ volatile(
    "cmp %2, %1\n\t"
            "lahf\n\t"
            "mov %0, %%ah\n\t"
    :  "=r"(ah)
    : "r"(y), "r"(x));
    return ah;
}


static int add(int x, int y, int z) { return x + y; }

static int sub(int x, int y, int z) { return x - y; }

static int mul(int x, int y, int z) { return x * y; }

/* For division overflow we return arbitrary values, hoping they fail
to be part of a solution.  (User must check solutions, in general.) */
static int div_(int x, int y, int z) {
    if (!y || (y == -1 && x == (int) 0x80000000)) {
        unacceptable++;
        return 0;
    } else return x / y;
}

static int divu(int x, int y, int z) {
    if (!y) {
        unacceptable++;
        return y;
    } else return (unsigned) x / (unsigned) y;
}

static int _and(int x, int y, int z) { return x & y; }

static int _or(int x, int y, int z) { return x | y; }

static int _xor(int x, int y, int z) { return x ^ y; }

static int rotl(int x, int y, int z) {
    int s = y & NBSM;
    return x << s | (unsigned) x >> (32 - s);
}

static int shl(int x, int y, int z) {
    int s = y & NBSM;
    if (s >= 32) return 0; else return x << s;
}

static int shr(int x, int y, int z) {
    int s = y & NBSM;
    if (s >= 32) return 0; else return (unsigned) x >> s;
}

static int shrs(int x, int y, int z) {
    //int s = y & NBSM;
    //return x >> s;
    int result;
    __asm__(
    "shrx %0, %1, %2\n\t"
    : "=r"(result)
    : "r"(x), "r"(y)
    );
    return result;
}

static int cmpeq(int x, int y, int z) { return x == y; }

static int cmplt(int x, int y, int z) { return x < y; }

static int cmpltu(int x, int y, int z) { return (unsigned) (x) < (unsigned) (y); }


// The machine's instruction set:
// Note: Commutative ops are commutative in operands 0 and 1.
constexpr static const struct {
    int (*const proc)(int, int, int); // Procedure for simulating the op.

    const ptrdiff_t numopnds;
    const ptrdiff_t commutative;
    const ptrdiff_t opndstart[3];           // Starting reg no. for each operand.
    const char *const mnemonic;              // Name of op, for printing.
    const char *const fun_name;              // Function name, for printing.
    const char *const op_name;               // Operator name, for printing.

} isa[] = {
        {neg,      1, 0, {RX, 0,  0}, "neg",      "-(",          ""},  // Negate.
        {pop,      1, 0, {RX, 0,  0}, "pop",      "pop(",        ""},  // Population count.
        {lzcnt,    1, 0, {RX, 0,  0}, "lzcnt",    "lzcnt(",      ""},


        {bzhi,     2, 0, {1, NIM, 0}, "bzhi",     "bzhi(",       ""},
        {floatrep, 1, 0, {RX, 0,  0}, "floatrep", "floatrep(",   ""},  // floating point representation
        {asFloat, 1, 0, {RX, 0,  0}, "asfloat", "asfloat(",   ""},  // floating point representation
        {cmpeq,    2, 1, {RX, 0,  0}, "cmpeq",    "(",           " == "},  // Compare equal.
        {cmplt,    2, 0, {0,  0,  0}, "cmplt",    "(",           " < "},  // Compare less than.
        {cmpltu,   2, 0, {1,  1,  0}, "cmpltu",   "(",           " <u "},  // Compare less than unsigned.
        {_and,     2, 1, {RX, 2,  0}, "and",      "(",           " & "},  // AND.
        {_or,      2, 1, {RX, 2,  0}, "or",       "(",           " | "},  // OR.
        {_xor,     2, 1, {RX, 2,  0}, "xor",      "(",           " ^ "},  // XOR.
        {hidbl,    1, 0, {RX, 0,  0}, "hidbl",    "hidbl(",      ""},  // floating point representation
        {lodbl,    1, 0, {RX, 0,  0}, "lodbl",    "lodbl(",      ""},  // floating point representation

        {sub,      2, 0, {2,  2,  0}, "sub",      "(",           " - "},  // Subtract.
        {add,      2, 1, {RX, 2,  0}, "add",      "(",           " + "},  // Add.
        {_not,     1, 0, {RX, 0,  0}, "not",      "~(",          ""},  // One's-complement.
       /* {movzxb,   1, 0, {RX, 0,  0}, "movzxb",   "(uint8_t)(",  ""},  // One's-complement.
        {movzxw,   1, 0, {RX, 0,  0}, "movzxw",   "(uint16_t)(", ""},  // One's-complement.
        {movsxb,   1, 0, {RX, 0,  0}, "movsxb",   "(int8_t)(",   ""},  // One's-complement.
        {movsxw,   1, 0, {RX, 0,  0}, "movsxw",   "(int16_t)(",  ""},  // One's-complement.*/
        {rotl,     2, 0, {1, NIM, 0}, "rotl",     "(",           " <<r "},  // Rotate shift left.
        {shl,      2, 0, {1, NIM, 0}, "shl",      "(",           " << "},  // Shift left.
        {shr,      2, 0, {1, NIM, 0}, "shr",      "(",           " >>u "},  // Shift right.

        {shrs,     2, 0, {3, NIM, 0}, "shrs",     "(",           " >>s "},  // Shift right signed.

        //{div_,     2, 0, {1,  3,  0}, "div",      "(",           "/"},  // Divide signed.
        //{divu,     2, 0, {1,  1,  0}, "divu",     "(",           " /u "},  // Divide unsigned.
        {mul,      2, 1, {RX, 3,  0}, "mul",      "(",           "*"},  // Multiply.
        {highmul,  2, 1, {RX, 3,  0}, "highmul",  "(",           "**"},  // Multiply.

        {pdep,     2, 1, {RX, 3,  0}, "pdep",     "pdep(",       ""},
        {pext,     2, 1, {RX, 3,  0}, "pext",     "pext(",       ""},
        //{cmpx86,   2, 1, {RX, 2,  0}, "cmpx86",   "cmpx86(",     ""},  // Add.


        {rbyte,    1, 0, {RX, 0,  0}, "bswap32",  "bswap32(",    ""},  // reverse bytes
       // {rbyte16,  1, 0, {RX, 0,  0}, "bswap16",  "bswap16(",    ""},
};

static constexpr ptrdiff_t NUM_INSNS_IN_ISA = sizeof(isa) / sizeof(isa[0]);

