#pragma once

#include "rv32_instr.h"

enum : uint8_t {
    OPCODE_R_TYPE = 0b0110011,
    OPCODE_I_TYPE_ALU = 0b0010011,
    OPCODE_I_TYPE_MEM = 0b0000011,
    OPCODE_S_TYPE = 0b0100011,
    OPCODE_B_TYPE = 0b1100011,
    OPCODE_JAL = 0b1101111,
    OPCODE_JALR = 0b1100111,
    OPCODE_LUI = 0b0110111,
    OPCODE_AUIPC = 0b0010111,
};

static uint32_t instr_decode_imm_extend(uint8_t const opcode, uint32_t imm);
static Instr instr_decode_r_type(uint32_t instr);
static Instr instr_decode_i_type_alu(uint32_t instr);
static Instr instr_decode_i_type_mem(uint32_t instr);
static Instr instr_decode_s_type(uint32_t instr);
static Instr instr_decode_b_type(uint32_t instr);
static Instr instr_decode_jal(uint32_t instr);
static Instr instr_decode_jalr(uint32_t instr);
static Instr instr_decode_lui(uint32_t instr);
static Instr instr_decode_auipc(uint32_t instr);
static Instr instr_decode(uint32_t instr);
