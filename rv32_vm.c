#include <raylib.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "rv32_instr.h"
#include "rv32_instr.c"

#include "rv32_decode.h"
#include "rv32_decode.c"

#include "rv32_mem.h"
#include "rv32_mem.c"

#include "rv32_regs.h"
#include "rv32_regs.c"

#include <string.h>

enum {
    FONT_SIZE = 16,
    PAGE_SIZE = 20
};

static uint32_t u32_min(uint32_t const a, uint32_t const b) {
    return a < b ? a : b;
}

typedef struct Vm {
    InstrMem instr_mem;
    RegFile reg_file;
    DataMem data_mem;

    uint32_t pc;
    bool pc_update;
} Vm;

static void vm_load(Vm *vm, const char* file, InstrMemFileType const type) {
    instr_mem_load(&vm->instr_mem, file, type);
}

static void vm_destroy(Vm *vm) {
    instr_mem_destroy(&vm->instr_mem);
}

static uint32_t vm_fetch(Vm const *vm) {
    return vm->instr_mem.base[vm->pc];
}

static uint32_t vm_fetch_offset(Vm const *vm, uint32_t const offset) {
    return vm->instr_mem.base[vm->pc + offset];
}

static uint32_t vm_fetch_offset_absolute(Vm const *vm, uint32_t const offset) {
    return vm->instr_mem.base[offset];
}

static void vm_execute_ecall(Vm *vm) {
    uint32_t const id = reg_file_read(&vm->reg_file, A0);
    uint32_t const value = reg_file_read(&vm->reg_file, A1);
    switch (id) {
        case 1: printf("%d", value); break;
        case 4: printf("%s", (char *) data_mem_read(&vm->data_mem, value)); break;
        case 10: exit(0);
        case 11: putchar((int) value); break;
        case 17: exit((int) value);
        default: assert(0 && "Unknown id for ecall");
    }
}

static void vm_execute(Vm *vm, Instr const *instr) {
    vm->pc_update = false;
    uint32_t const rd1 = reg_file_read(&vm->reg_file, instr->rs1);
    uint32_t const rd2 = reg_file_read(&vm->reg_file, instr->rs2);
    uint32_t *rd = reg_file_read_ptr(&vm->reg_file, instr->rd);

    switch (instr->op) {
        case ADD:  *rd = rd1 + rd2; break;
        case SUB:  *rd = rd1 - rd2; break;
        case XOR:  *rd = rd1 ^ rd2; break;
        case OR:   *rd = rd1 | rd2; break;
        case AND:  *rd = rd1 & rd2; break;
        case SLL:  *rd = rd1 << rd2; break;
        case SRL:  *rd = rd1 >> rd2; break;
        case SRA:  *rd = (int32_t) rd1 >> rd2; break;
        case SLT:  *rd = (int32_t) rd1 < (int32_t) rd2; break;
        case SLTU: *rd = rd1 < rd2; break;

        // I-Type
        case ADDI:  *rd = rd1 + instr->imm; break;
        case XORI:  *rd = rd1 ^ instr->imm; break;
        case ORI:   *rd = rd1 | instr->imm; break;
        case ANDI:  *rd = rd1 & instr->imm; break;
        case SLLI:  *rd = rd1 << instr->imm; break;
        case SRLI:  *rd = rd1 >> instr->imm; break;
        case SRAI:  *rd = (int32_t) rd1 >> instr->imm; break;
        case SLTI:  *rd = (int32_t) rd1 < (int32_t) instr->imm; break;
        case SLTIU: *rd = rd1 < instr->imm; break;

        // I-Type MEM
        case LB: *rd = *(int8_t *)  data_mem_read(&vm->data_mem, instr->rd + (int32_t) instr->imm); break;
        case LH: *rd = *(int16_t *) data_mem_read(&vm->data_mem, instr->rd + (int32_t) instr->imm); break;
        case LW: *rd = *(int32_t *) data_mem_read(&vm->data_mem, instr->rd + (int32_t) instr->imm); break;
        case LBU: *rd = *(uint8_t *)  data_mem_read(&vm->data_mem, instr->rd + (int32_t) instr->imm); break;
        case LHU: *rd = *(uint16_t *) data_mem_read(&vm->data_mem, instr->rd + (int32_t) instr->imm); break;
        case SB: data_mem_write8(&vm->data_mem, instr->rs1 + instr->imm, instr->rs2); break;
        case SH: data_mem_write16(&vm->data_mem, instr->rs1 + instr->imm, instr->rs2); break;
        case SW: data_mem_write32(&vm->data_mem, instr->rs1 + instr->imm, instr->rs2); break;
        case BEQ: {
            vm->pc_update = !(instr->rs1 - instr->rs2);
            vm->pc += vm->pc_update * instr->imm;
            break;
        }
        case BNE: {
            vm->pc_update = (instr->rs1 - instr->rs2);
            vm->pc += vm->pc_update * instr->imm;
            break;
        }
        case BLT: {
            // TODO(elias): may be optimized
            vm->pc_update = (int32_t) instr->rs1 < (int32_t) instr->rs2;
            vm->pc += vm->pc_update * instr->imm;
            break;
        }
        case BGE: {
            vm->pc_update = (int32_t) instr->rs1 >= (int32_t) instr->rs2;
            vm->pc += vm->pc_update * instr->imm;
            break;
        }
        case BLTU: {
            vm->pc_update = instr->rs1 < instr->rs2;
            vm->pc += vm->pc_update * instr->imm;
            break;
        }
        case BGEU: {
            vm->pc_update = instr->rs1 >= instr->rs2;
            vm->pc += vm->pc_update * instr->imm;
            break;
        }
        case JAL: {
            *rd = vm->pc + 4;
            vm->pc_update = true;
            vm->pc += instr->imm;
            break;
        }
        case JALR: {
            *rd = vm->pc + 4;
            vm->pc_update = true;
            vm->pc = instr->rs1 + instr->imm;
            break;
        }
        case LUI: *rd = instr->imm << 12; break;
        case AUIPC: *rd = vm->pc + (instr->imm << 12); break;
        case ECALL: vm_execute_ecall(vm); break;
        default: break;
    }
    instr_print(instr);
}

static void vm_run(Vm *vm) {
    InitWindow(1280, 720, "rv32_vm");
    SetTargetFPS(60);

    while (vm->pc < vm->instr_mem.size && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        uint32_t const start = vm->pc / PAGE_SIZE * PAGE_SIZE;
        uint32_t const end = u32_min(vm->instr_mem.size, start + PAGE_SIZE);
        uint32_t const count = end - start;
        bool const want_step = IsKeyPressed(KEY_F10);

        for (uint32_t i = 0; i < count; ++i) {
            uint32_t const current = start + i;
            uint32_t const instr = vm_fetch_offset_absolute(vm, current);
            Instr const decoded = instr_decode(instr);

            if (current == vm->pc && want_step) {
                vm_execute(vm, &decoded);
                ++vm->pc;
                break;
            }

            char instr_text_buffer[128] = { 0 };
            instr_sprintf(instr_text_buffer, sizeof instr_text_buffer, &decoded);

            int const text_y_position = (int) (10.0f + (float) i * 1.25f * FONT_SIZE);
            Color const text_color = i != vm->pc ? WHITE : (Color) { 255, 153, 0, 255 };
            DrawText(instr_text_buffer, 10, text_y_position, FONT_SIZE, text_color);
        }

        for (uint32_t i = 0; i < REG_COUNT; ++i) {
            char reg_text_buffer[128] = { 0 };
            snprintf(reg_text_buffer, sizeof reg_text_buffer, "x%u = 0x%08X", i, vm->reg_file.regs[i]);

            int const text_y_position = (int) (10.0f + (float) i * 1.25f * FONT_SIZE);
            DrawText(reg_text_buffer, 1000, text_y_position, FONT_SIZE / 2, YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
}

int main(void) {
    Vm vm = { 0 };
    vm_load(&vm, "data/instructions.txt", INSTR_MEM_FILE_TYPE_TEXT);
    vm_run(&vm);
    vm_destroy(&vm);
    return 0;
}
