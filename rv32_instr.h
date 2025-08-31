#pragma once

#include "rv32.h"

typedef enum Op : uint8_t {
    ADD, SUB, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU,
    ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU,
    LB, LH, LW, LBU, LHU, SB, SH, SW,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    JAL,
    JALR,
    LUI,
    AUIPC
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
