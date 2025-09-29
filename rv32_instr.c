#define OP_STRINGIFY(op) case op: return # op

static const char* op_string(Op const op) {
    switch (op) {
        OP_STRINGIFY(ADD);
        OP_STRINGIFY(SUB);
        OP_STRINGIFY(XOR);
        OP_STRINGIFY(OR);
        OP_STRINGIFY(AND);
        OP_STRINGIFY(SLL);
        OP_STRINGIFY(SRL);
        OP_STRINGIFY(SRA);
        OP_STRINGIFY(SLT);
        OP_STRINGIFY(SLTU);
        OP_STRINGIFY(ADDI);
        OP_STRINGIFY(XORI);
        OP_STRINGIFY(ORI);
        OP_STRINGIFY(ANDI);
        OP_STRINGIFY(SLLI);
        OP_STRINGIFY(SRLI);
        OP_STRINGIFY(SRAI);
        OP_STRINGIFY(SLTI);
        OP_STRINGIFY(SLTIU);
        OP_STRINGIFY(LB);
        OP_STRINGIFY(LH);
        OP_STRINGIFY(LW);
        OP_STRINGIFY(LBU);
        OP_STRINGIFY(LHU);
        OP_STRINGIFY(SB);
        OP_STRINGIFY(SH);
        OP_STRINGIFY(SW);
        OP_STRINGIFY(BEQ);
        OP_STRINGIFY(BNE);
        OP_STRINGIFY(BLT);
        OP_STRINGIFY(BGE);
        OP_STRINGIFY(BLTU);
        OP_STRINGIFY(BGEU);
        OP_STRINGIFY(JAL);
        OP_STRINGIFY(JALR);
        OP_STRINGIFY(LUI);
        OP_STRINGIFY(AUIPC);
        OP_STRINGIFY(ECALL);
        OP_STRINGIFY(EBREAK);
        default:
            assert(0 && "Invalid op");
    }
}

#undef OP_STRINGIFY

static Instr instr_nop() {
    // addi zero, zero, 0
    return (Instr) {
        .op = ADDI,
        .rd = 0,
        .rs1 = 0,
        .imm = 0
    };
}

static void instr_print(Instr const *instr) {
    printf(
        "Instr{ .op = %s, .rd = x%d, .rs1 = x%d, .rs2 = x%d, .imm = %i }\n",
        op_string(instr->op),
        instr->rd,
        instr->rs1,
        instr->rs2,
        (int32_t) instr->imm
    );
}

static void instr_sprintf(char* buffer, size_t size, Instr const *instr) {
    char const* fmt = "Instr{ .op = %s, .rd = x%d, .rs1 = x%d, .rs2 = x%d, .imm = %i }\n";
    int const required_size = snprintf(
        NULL,
        0,
        fmt,
        op_string(instr->op),
        instr->rd,
        instr->rs1,
        instr->rs2,
        (int32_t) instr->imm
    );

    assert(required_size <= size);
    sprintf(
        buffer,
        fmt,
        op_string(instr->op),
        instr->rd,
        instr->rs1,
        instr->rs2,
        (int32_t) instr->imm
    );
}
