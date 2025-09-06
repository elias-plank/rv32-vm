typedef enum AluControl {
    ALU_ADD = 0b000u,
    ALU_SUB = 0b001,
    ALU_AND = 0b010,
    ALU_OR = 0b011,
    ALU_XOR = 0b100,
    ALU_SLT = 0b101,
} AluControl;

static uint32_t alu_execute(uint32_t a, uint32_t b, AluControl op);
