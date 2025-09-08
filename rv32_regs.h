typedef enum Reg {
    X0  = 0,  ZERO = X0,
    X1  = 1,  RA   = X1,
    X2  = 2,  SP   = X2,
    X3  = 3,  GP   = X3,
    X4  = 4,  TP   = X4,
    X5  = 5,  T0   = X5,
    X6  = 6,  T1   = X6,
    X7  = 7,  T2   = X7,
    X8  = 8,  S0   = X8, FP = X8,
    X9  = 9,  S1   = X9,
    X10 = 10, A0   = X10,
    X11 = 11, A1   = X11,
    X12 = 12, A2   = X12,
    X13 = 13, A3   = X13,
    X14 = 14, A4   = X14,
    X15 = 15, A5   = X15,
    X16 = 16, A6   = X16,
    X17 = 17, A7   = X17,
    X18 = 18, S2   = X18,
    X19 = 19, S3   = X19,
    X20 = 20, S4   = X20,
    X21 = 21, S5   = X21,
    X22 = 22, S6   = X22,
    X23 = 23, S7   = X23,
    X24 = 24, S8   = X24,
    X25 = 25, S9   = X25,
    X26 = 26, S10  = X26,
    X27 = 27, S11  = X27,
    X28 = 28, T3   = X28,
    X29 = 29, T4   = X29,
    X30 = 30, T5   = X30,
    X31 = 31, T6   = X31,
    REG_COUNT = 32
} Reg;

typedef struct RegFile {
    uint32_t regs[REG_COUNT];
} RegFile;

static uint32_t reg_file_read(RegFile *file, Reg reg);
static void reg_file_write(RegFile *file, Reg reg, uint32_t value);
