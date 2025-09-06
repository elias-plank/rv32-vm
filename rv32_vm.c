#include "rv32.h"

#include "rv32_instr.h"
#include "rv32_instr.c"

#include "rv32_decode.h"
#include "rv32_decode.c"

// TODO(elias):
// - [ ] Instruction fuzzer
// - [ ] Instruction decode test based on fuzzer
// - [ ] Write Single Cycle Architecture:
//       - Fetch
//       - Decode
//       - Execute
//       - Memory
//       - Writeback

static void instr_pd(uint32_t const instr) {
    Instr const decoded = instr_decode(instr);
    instr_print(&decoded);
}

int main(void) {
    instr_pd(0x10000517);
    instr_pd(0x00050513);
    instr_pd(0x00800593);
    instr_pd(0x10000617);
    instr_pd(0xFFC60613);
    instr_pd(0x00C000EF);
    instr_pd(0x00A00513);
    // ecall
    // instr_pd(0x00000073);
    instr_pd(0x000002B3);
    instr_pd(0x02B2D063);
    instr_pd(0x00050383);
    instr_pd(0xFFF3C313);
    instr_pd(0x00661023);
    instr_pd(0x00128293);
    instr_pd(0x00150513);
    instr_pd(0x00260613);
    instr_pd(0xFE5FF06F);
    instr_pd(0x00008067);
    return 0;
}
