static uint32_t alu_execute(uint32_t a, uint32_t b, AluControl op) {
    switch (op) {
        case ALU_ADD: return a + b;
        case ALU_SUB: return a + (int32_t) b;
        case ALU_AND: return a & b;
        case ALU_OR: return a | b;
        case ALU_XOR: return a ^ b;
        case ALU_SLT: return a < b ? 1 : 0;
        default: assert(0 && "Invalid ALU control");
    }
}
