#include "rv32_decode.h"

static DecodeResult decode_result_make(Instr const instr) {
    return (DecodeResult) {
        .instr = instr,
        .error = { 0 }
    };
}

static DecodeResult decode_result_make_error(char const *error, uint8_t const length) {
    assert(length <= 128);
    DecodeResult result = {};
    result.instr = instr_nop();
    strncpy(result.error, error, length);
    return result;
}

static uint32_t instr_read_bits(uint32_t instr, uint8_t const offset, uint8_t const length) {
    uint8_t const shift_offset = 32 - length;
    instr = instr >> offset;
    instr = instr << shift_offset;
    instr = instr >> shift_offset;
    return instr;
}

static uint32_t instr_consume_bits(uint32_t *instr, uint8_t const length) {
    uint32_t const mask = (length == 32) ? 0xFFFFFFFFu : ((1u << length) - 1u);
    uint32_t const val  = *instr & mask;
    *instr >>= length;
    return val;
}

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
    uint8_t const rd = instr_consume_bits(&instr, 5);
    uint8_t const funct3 = instr_consume_bits(&instr, 3);
    uint8_t const rs1 = instr_consume_bits(&instr, 5);
    uint8_t const rs2 = instr_consume_bits(&instr, 5);
    uint8_t const funct7 = instr_consume_bits(&instr, 7);

    Instr result = {};
    result.rd = rd;
    result.rs1 = rs1;
    result.rs2 = rs2;

    switch (funct3) {
        case 0b000: {
            switch (funct7) {
                case 0b0000000:
                    result.op = ADD;
                    return result;
                case 0b0100000:
                    result.op = SUB;
                    return result;
                default:
                    assert(0 && "Invalid funct7 for add/sub");
                    break;
            }
            break;
        }
        case 0b100:
            result.op = XOR;
            return result;
        case 0b110:
            result.op = OR;
            return result;
        case 0b111:
            result.op = AND;
            return result;
        case 0b001:
            result.op = SLL;
            return result;
        case 0b101:
            switch (funct7 & (1 << 5)) {
                case 0:
                    result.op = SRL;
                    return result;
                case 1:
                    result.op = SRA;
                    return result;
                default: break;
            }
            break;
        case 0b010:
            result.op = SLT;
            return result;
        case 0b011:
            result.op = SLTU;
            return result;
        default:
            assert(0 && "Invalid funct3 for r-type instruction");
            break;
    }

    return instr_nop();
}

static Instr instr_decode_i_type_alu(uint32_t instr) {
    uint8_t const rd = instr_consume_bits(&instr, 5);
    uint8_t const funct3 = instr_consume_bits(&instr, 3);
    uint8_t const rs1 = instr_consume_bits(&instr, 5);
    uint16_t const imm = instr_consume_bits(&instr, 12);

    // upper seven bits of immediate
    uint8_t const funct7 = (imm >> 5) & 0x7F;

    Instr result = {};
    result.rd = rd;
    result.rs1 = rs1;
    result.imm = instr_decode_imm_extend(OPCODE_I_TYPE_ALU, imm);

    switch (funct3) {
        case 0b000:
            result.op = ADDI;
            return result;
        case 0b100:
            result.op = XORI;
            return result;
        case 0b110:
            result.op = ORI;
            return result;
        case 0b111:
            result.op = ANDI;
            return result;
        case 0b001:
            result.op = SLLI;
            return result;
        case 0b101:
            switch (funct7 & (1 << 5)) {
                case 0:
                    result.op = SRLI;
                    return result;
                case 1:
                    result.op = SRAI;
                    return result;
                default: break;
            }
        case 0b010:
            result.op = SLTI;
            return result;
        case 0b011:
            result.op = SLTIU;
            return result;
        default:
            assert(0 && "Invalid funct3 for i-type alu instruction");
            break;
    }

    return instr_nop();
}

static Instr instr_decode_i_type_mem(uint32_t instr) {
    return instr_nop();
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
    uint8_t const opcode = instr_consume_bits(&instr, 7);
    switch (opcode) {
        case OPCODE_R_TYPE:
            return instr_decode_r_type(instr);
        case OPCODE_I_TYPE_ALU:
            return instr_decode_i_type_alu(instr);
        case OPCODE_I_TYPE_MEM:
            return instr_decode_i_type_mem(instr);
        case OPCODE_S_TYPE:
            return instr_decode_s_type(instr);
        case OPCODE_B_TYPE:
            return instr_decode_b_type(instr);
        case OPCODE_JAL:
            return instr_decode_jal(instr);
        case OPCODE_JALR:
            return instr_decode_jalr(instr);
        case OPCODE_LUI:
            return instr_decode_lui(instr);
        case OPCODE_AUIPC:
            return instr_decode_auipc(instr);
        default:
            assert(0 && "Unknown instruction");
            break;
    }
    return instr_nop();
}
