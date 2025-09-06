typedef enum Op : uint8_t {
    // R-Type
    ADD, SUB, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU,
    // I-Type ALU
    ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU,
    // I-Type MEM
    LB, LH, LW, LBU, LHU, SB, SH, SW,
    // B-Type
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    // J-Type
    JAL,
    // I-Type
    JALR,
    // U-Type
    LUI, AUIPC,
    // Privileged
    ECALL, EBREAK
} Op;

static const char* op_string(Op op);

typedef struct Instr {
    Op op;
    uint8_t rd : 5;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint32_t imm;
} Instr;

static Instr instr_nop();
static void instr_print(Instr const *instr);
