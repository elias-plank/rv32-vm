static const char* instr_mem_load_read_type(InstrMemFileType const type) {
    switch (type) {
        case INSTR_MEM_FILE_TYPE_BIN: return "rb";
        case INSTR_MEM_FILE_TYPE_TEXT: return "r";
        default: return "r";
    }
}

static void instr_mem_load_binary(InstrMem *mem, char* buffer, size_t size) {
    if (size % sizeof(uint32_t) != 0) {
        assert(0 && "Binary file does not align to 32 bits of RV32");
    }

    mem->base = (uint32_t*) buffer;
    mem->size = size / sizeof(uint32_t);
}

static void instr_mem_load_text(InstrMem *mem, char* buffer, size_t size) {
    buffer[size] = 0;

    const char *p = buffer;
    const char *end = buffer + size;

    size_t cap = 0;
    size_t len = 0;
    uint32_t *arr = NULL;

    while (p < end) {
        while (p < end && isspace((unsigned char) *p)) {
            p++;
        }

        if (p >= end) {
            break;
        }

        char *next;
        unsigned long val = strtoul(p, &next, 16);

        if (next == p) {
            assert(0 && "Invalid hex number in input");
        }

        if (val > UINT32_MAX) {
            assert(0 && "Hex number out of range for uint32_t");
        }

        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            uint32_t *tmp = realloc(arr, new_cap * sizeof(uint32_t));
            if (!tmp) {
                assert(0 && "Out of memory while reallocating");
            }
            arr = tmp;
            cap = new_cap;
        }

        arr[len++] = (uint32_t) val;
        p = next;
    }

    mem->base = arr;
    mem->size = len;
    free(buffer);
}

static void instr_mem_load(InstrMem *mem, const char* file, InstrMemFileType type) {
    FILE *fp = fopen(file, instr_mem_load_read_type(type));
    if (!fp) {
        assert(0 && "Provided instruction memory file does not exist");
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* buffer = malloc(size + (type == INSTR_MEM_FILE_TYPE_TEXT ? 1 : 0));
    if (!buffer) {
        assert(0 && "Failed to allocate buffer for memory file");
    }

    fread(buffer, 1, size, fp);
    fclose(fp);

    switch (type) {
        case INSTR_MEM_FILE_TYPE_BIN: return instr_mem_load_binary(mem, buffer, size);
        case INSTR_MEM_FILE_TYPE_TEXT: return instr_mem_load_text(mem, buffer, size);
        default: assert(0 && "Unsupported instruction memory file type");
    }
}

static void instr_mem_destroy(InstrMem *mem) {
    free(mem->base);
    mem->base = NULL;
    mem->size = 0;
}