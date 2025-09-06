#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void vm_load(Vm *vm, const char* file, InstrMemFileType type) {
    instr_mem_load(&vm->instr_mem, file, type);
}

static void vm_destroy(Vm *vm) {
    instr_mem_destroy(&vm->instr_mem);
}

static uint32_t vm_fetch(Vm *vm) {
    return vm->instr_mem.base[vm->pc];
}

static void vm_execute(Vm *vm, Instr const *instr) {
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
