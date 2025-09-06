typedef struct InstrMem {
    uint32_t *base;
    size_t size;
} InstrMem;

typedef enum InstrMemFileType : uint8_t {
    INSTR_MEM_FILE_TYPE_BIN = 0,
    INSTR_MEM_FILE_TYPE_TEXT = 1,
} InstrMemFileType;

static void instr_mem_load(InstrMem *mem, const char* file, InstrMemFileType type);
static void instr_mem_destroy(InstrMem *mem);

typedef struct DataMem {
    uint32_t base[0x8000];
} DataMem;
