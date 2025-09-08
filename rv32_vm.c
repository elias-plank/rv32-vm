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

typedef struct Vm {
    InstrMem instr_mem;
    RegFile reg_file;
    DataMem data_mem;

    uint32_t pc;
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

static void vm_execute_ecall(Vm *vm) {
    uint32_t const id = reg_file_read(&vm->reg_file, A0);
    uint32_t const value = reg_file_read(&vm->reg_file, A1);
    switch (id) {
        case 1: printf("%d", value); break;
        case 4: printf("%s", (char *) data_mem_read(&vm->data_mem, value)); break;
        case 10: exit(0);
        case 11: putchar((int) value); break;
        case 17: exit((int) value);
        default: break;
    }
}

static void vm_execute(Vm *vm, Instr const *instr) {
    switch (instr->op) {
        case ECALL: vm_execute_ecall(vm); break;
        default: break;
    }
    instr_print(instr);
}

static void vm_run(Vm *vm) {
    while (vm->pc < vm->instr_mem.size) {
        // Fetch
        uint32_t const instr = vm_fetch(vm);
        // Decode
        Instr const decoded = instr_decode(instr);
        // Execute
        vm_execute(vm, &decoded);
        ++vm->pc;
    }
}

int main(void) {
    Vm vm = { 0 };
    vm_load(&vm, "data/instructions.txt", INSTR_MEM_FILE_TYPE_TEXT);
    vm_run(&vm);
    vm_destroy(&vm);
    return 0;
}
