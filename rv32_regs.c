static uint32_t reg_file_read(RegFile *file, Reg reg) {
    return file->regs[reg];
}

static uint32_t *reg_file_read_ptr(RegFile *file, Reg reg) {
    return &file->regs[reg];
}

static void reg_file_write(RegFile *file, Reg reg, uint32_t value) {
    file->regs[reg] = value;
}
