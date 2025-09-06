#include "rv32_decode.h"

static uint32_t instr_decode_imm_extend(uint8_t const opcode, uint32_t imm) {
    switch (opcode) {
        case OPCODE_I_TYPE_ALU:
        case OPCODE_S_TYPE:
            return (imm ^ (1u << 11)) - (1u << 11);
        case OPCODE_B_TYPE:
            return (imm ^ (1u << 12)) - (1u << 12);
        case OPCODE_JAL:
            return (imm ^ (1u << 20)) - (1u << 20);
        default:
            break;
    }
    return imm;
}

static Instr instr_decode_r_type(uint32_t const instr) {
    uint8_t const rd = (instr >> 7) & 0x1Fu;
    uint8_t const funct3 = (instr >> 12) & 0x7u;
    uint8_t const rs1 = (instr >> 15) & 0x1Fu;
    uint8_t const rs2 = (instr >> 20) & 0x1Fu;
    uint8_t const funct7 = (instr >> 25) & 0x7Fu;

    Instr result = { 0 };
    result.rd = rd;
    result.rs1 = rs1;
    result.rs2 = rs2;

    switch (funct3) {
        case 0b000:
            switch (funct7) {
                case 0b0000000u: result.op = ADD; break;
                case 0b0100000u: result.op = SUB; break;
                default:
                    assert(0 && "Invalid funct7 for add/sub");
            }
            break;
        case 0b100u: result.op = XOR; break;
        case 0b110u: result.op = OR; break;
        case 0b111u: result.op = AND; break;
        case 0b001u: result.op = SLL; break;
        case 0b101u:
            switch (funct7 & (1u << 5)) {
                case 0u: result.op = SRL; break;
                case 1u: result.op = SRA; break;
                default: break;
            }
            break;
        case 0b010u: result.op = SLT; break;
        case 0b011u: result.op = SLTU; break;
        default:
            assert(0 && "Invalid funct3 for r-type instruction");
    }

    return result;
}

static Instr instr_decode_i_type_alu(uint32_t const instr) {
    uint8_t const rd = (instr >> 7) & 0x1Fu;
    uint8_t const funct3 = (instr >> 12) & 0x7u;
    uint8_t const rs1 = (instr >> 15) & 0x1Fu;
    uint16_t const imm = (instr >> 20) & 0xFFFu;

    // upper seven bits of immediate
    uint8_t const funct7 = (imm >> 5) & 0x7Fu;

    Instr result = { 0 };
    result.rd = rd;
    result.rs1 = rs1;
    result.imm = instr_decode_imm_extend(OPCODE_I_TYPE_ALU, imm);

    switch (funct3) {
        case 0b000u: result.op = ADDI; break;
        case 0b100u: result.op = XORI; break;
        case 0b110u: result.op = ORI; break;
        case 0b111u: result.op = ANDI; break;
        case 0b001u: result.op = SLLI; break;
        case 0b101u:
            switch (funct7 & (1u << 5)) {
                case 0u: result.op = SRLI; break;
                case 1u: result.op = SRAI; break;
                default: break;
            }
            break;
        case 0b010u: result.op = SLTI; break;
        case 0b011u: result.op = SLTIU; break;
        default:
            assert(0 && "Invalid funct3 for i-type alu instruction");
    }

    return result;
}

static Instr instr_decode_i_type_mem(uint32_t const instr) {
    uint8_t const rd = (instr >> 7) & 0x1Fu;
    uint8_t const funct3 = (instr >> 12) & 0x7u;
    uint8_t const rs1 = (instr >> 15) & 0x1Fu;
    uint16_t const imm = (instr >> 20) & 0xFFFu;

    Instr result = { 0 };
    result.rd = rd;
    result.rs1 = rs1;
    result.imm = instr_decode_imm_extend(OPCODE_I_TYPE_ALU, imm);
    switch (funct3) {
        case 0b000u: result.op = LB; break;
        case 0b001u: result.op = LH; break;
        case 0b010u: result.op = LW; break;
        case 0b100u: result.op = LBU; break;
        case 0b101u: result.op = LHU; break;
        default:
            assert(0 && "Invalid funct3 for i-type mem instruction");
    }

    return result;
}
static Instr instr_decode_s_type(uint32_t const instr) {
    uint16_t const imm = ((instr >> 7) & 0x1Fu) | ((instr >> 25) << 5);
    uint8_t const funct3 = (instr >> 12) & 0x7u;
    uint8_t const rs1 = (instr >> 15) & 0x1Fu;
    uint8_t const rs2 = (instr >> 20) & 0x1Fu;

    Instr result = { 0 };
    result.rs1 = rs1;
    result.rs2 = rs2;
    result.imm = instr_decode_imm_extend(OPCODE_S_TYPE, imm);

    switch (funct3) {
        case 0b000u: result.op = SB; break;
        case 0b001u: result.op = SH; break;
        case 0b010u: result.op = SW; break;
        default:
            assert(0 && "Invalid funct3 for s-type instruction");
    }
    return result;
}

static Instr instr_decode_b_type(uint32_t const instr) {
    uint8_t const imml = (instr >> 7) & 0x1Fu;
    uint8_t const immh = (instr >> 25) & 0x7Fu;

    uint8_t const funct3 = (instr >> 12) & 0x7u;
    uint8_t const rs1 = (instr >> 15) & 0x1Fu;
    uint8_t const rs2 = (instr >> 20) & 0x1Fu;

    // TODO(elias): test complex immediate
    uint16_t const imm = (imml & 0x1Eu)
        | ((immh & 0x1Fu) << 5)
        | ((imml & 1u) << 11)
        | (((immh >> 7u) & 1u) >> 12);

    Instr result = { 0 };
    result.rs1 = rs1;
    result.rs2 = rs2;
    result.imm = instr_decode_imm_extend(OPCODE_B_TYPE, imm);

    switch (funct3) {
        case 0b000u: result.op = BEQ; break;
        case 0b001u: result.op = BNE; break;
        case 0b100u: result.op = BLT; break;
        case 0b101u: result.op = BGE; break;
        case 0b110u: result.op = BLTU; break;
        case 0b111u: result.op = BGEU; break;
        default:
            assert(0 && "Invalid funct3 for b-type instruction");
    }

    return result;
}

static Instr instr_decode_jal(uint32_t const instr) {
    uint8_t const rd = (instr >> 7) & 0x1Fu;

    // read bits from raw immediate
    uint32_t const immr = ((instr >> 12) & 0xFFFFFu);
    uint32_t const imm = ((immr & (1u << 19)) << 1)
        | ((immr >> 8) & 0x7FEu)
        | ((immr & (1u << 8)) << 3)
        | ((immr & 0xFFu) << 12);

    Instr result = { 0 };
    result.rd = rd;
    result.imm = instr_decode_imm_extend(OPCODE_JAL, imm);
    result.op = JAL;

    return result;
}

static Instr instr_decode_jalr(uint32_t const instr) {
    uint8_t const rd = (instr >> 7) & 0x1Fu;
    uint8_t const rs1 = (instr >> 15) & 0x1Fu;
    uint16_t const imm = (instr >> 20) & 0xFFFu;

    Instr result = { 0 };
    result.rd = rd;
    result.rs1 = rs1;
    result.imm = instr_decode_imm_extend(OPCODE_JALR, imm);
    result.op = JALR;
    
    return result;
}

static Instr instr_decode_u_type(uint32_t const instr) {
    uint8_t const rd = (instr >> 7) & 0x1Fu;
    uint32_t const imm = ((instr >> 12) & 0xFFFFFu) << 12;

    Instr result = { 0 };
    result.rd = rd;
    result.imm = imm;

    switch ((instr >> 6) & 1) {
        case 0u: result.op = AUIPC; break;
        case 1u: result.op = LUI; break;
        default: break;
    }

    return result;
}

static Instr instr_decode(uint32_t const instr) {
    uint8_t const opcode = instr & 0x7Fu;
    switch (opcode) {
        case OPCODE_R_TYPE: return instr_decode_r_type(instr);
        case OPCODE_I_TYPE_ALU: return instr_decode_i_type_alu(instr);
        case OPCODE_I_TYPE_MEM: return instr_decode_i_type_mem(instr);
        case OPCODE_S_TYPE: return instr_decode_s_type(instr);
        case OPCODE_B_TYPE: return instr_decode_b_type(instr);
        case OPCODE_JAL: return instr_decode_jal(instr);
        case OPCODE_JALR: return instr_decode_jalr(instr);
        case OPCODE_LUI: return instr_decode_u_type(instr);
        case OPCODE_AUIPC: return instr_decode_u_type(instr);
        default:
            assert(0 && "Unknown instruction");
    }
}
