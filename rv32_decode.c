#include "rv32_decode.h"

static uint32_t instr_decode_imm_extend(uint8_t const opcode, uint32_t imm) {
    switch (opcode) {
        case OPCODE_I_TYPE_ALU:
            return (imm ^ 0x800u) - 0x800u;
        default:
            break;
    }
    return imm;
}

static Instr instr_decode_r_type(uint32_t instr) {
    uint8_t const rd = (instr >> 7) & 0x1F;
    uint8_t const funct3 = (instr >> 12) & 0x7;
    uint8_t const rs1 = (instr >> 15) & 0x1F;
    uint8_t const rs2 = (instr >> 20) & 0x1F;
    uint8_t const funct7 = (instr >> 25) & 0x7F;

    Instr result = {};
    result.rd = rd;
    result.rs1 = rs1;
    result.rs2 = rs2;

    switch (funct3) {
        case 0b000:
            switch (funct7) {
                case 0b0000000: result.op = ADD; break;
                case 0b0100000: result.op = SUB; break;
                default:
                    assert(0 && "Invalid funct7 for add/sub");
                    break;
            }
            break;
        case 0b100: result.op = XOR; break;
        case 0b110: result.op = OR; break;
        case 0b111: result.op = AND; break;
        case 0b001: result.op = SLL; break;
        case 0b101:
            switch (funct7 & (1 << 5)) {
                case 0: result.op = SRL; break;
                case 1: result.op = SRA; break;
                default: break;
            }
            break;
        case 0b010: result.op = SLT; break;
        case 0b011: result.op = SLTU; break;
        default:
            assert(0 && "Invalid funct3 for r-type instruction");
            break;
    }

    return result;
}

static Instr instr_decode_i_type_alu(uint32_t instr) {
    uint8_t const rd = (instr >> 7) & 0x1F;
    uint8_t const funct3 = (instr >> 12) & 0x7;
    uint8_t const rs1 = (instr >> 15) & 0x1F;
    uint16_t const imm = (instr >> 20) & 0xFFF;

    // upper seven bits of immediate
    uint8_t const funct7 = (imm >> 5) & 0x7F;

    Instr result = {};
    result.rd = rd;
    result.rs1 = rs1;
    result.imm = instr_decode_imm_extend(OPCODE_I_TYPE_ALU, imm);

    switch (funct3) {
        case 0b000: result.op = ADDI; break;
        case 0b100: result.op = XORI; break;
        case 0b110: result.op = ORI; break;
        case 0b111: result.op = ANDI; break;
        case 0b001: result.op = SLLI; break;
        case 0b101:
            switch (funct7 & (1 << 5)) {
                case 0: result.op = SRLI; break;
                case 1: result.op = SRAI; break;
                default: break;
            }
            break;
        case 0b010: result.op = SLTI; break;
        case 0b011: result.op = SLTIU; break;
        default:
            assert(0 && "Invalid funct3 for i-type alu instruction");
            break;
    }

    return result;
}

static Instr instr_decode_i_type_mem(uint32_t instr) {
    uint8_t const rd = (instr >> 7) & 0x1F;
    uint8_t const funct3 = (instr >> 12) & 0x7;
    uint8_t const rs1 = (instr >> 15) & 0x1F;
    uint16_t const imm = (instr >> 20) & 0xFFF;

    Instr result = {};
    result.rd = rd;
    result.rs1 = rs1;
    result.imm = instr_decode_imm_extend(OPCODE_I_TYPE_ALU, imm);
    switch (funct3) {
        case 0b000: result.op = LB; break;
        case 0b001: result.op = LH; break;
        case 0b010: result.op = LW; break;
        case 0b100: result.op = LBU; break;
        case 0b101: result.op = LHU; break;
        default:
            assert(0 && "Invalid funct3 for i-type mem instruction");
            break;
    }

    return result;
}
static Instr instr_decode_s_type(uint32_t instr) {
    return instr_nop();
}

static Instr instr_decode_b_type(uint32_t instr) {
    return instr_nop();
}

static Instr instr_decode_jal(uint32_t instr) {
    return instr_nop();
}

static Instr instr_decode_jalr(uint32_t instr) {
    return instr_nop();
}

static Instr instr_decode_lui(uint32_t instr) {
    return instr_nop();
}

static Instr instr_decode_auipc(uint32_t instr) {
    return instr_nop();
}

static Instr instr_decode(uint32_t instr) {
    uint8_t const opcode = instr & 0x7F;    
    switch (opcode) {
        case OPCODE_R_TYPE: return instr_decode_r_type(instr);
        case OPCODE_I_TYPE_ALU: return instr_decode_i_type_alu(instr);
        case OPCODE_I_TYPE_MEM: return instr_decode_i_type_mem(instr);
        case OPCODE_S_TYPE: return instr_decode_s_type(instr);
        case OPCODE_B_TYPE: return instr_decode_b_type(instr);
        case OPCODE_JAL: return instr_decode_jal(instr);
        case OPCODE_JALR: return instr_decode_jalr(instr);
        case OPCODE_LUI: return instr_decode_lui(instr);
        case OPCODE_AUIPC: return instr_decode_auipc(instr);
        default:
            assert(0 && "Unknown instruction");
            break;
    }
    return instr_nop();
}
