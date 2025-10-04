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

static uint8_t* data_mem_read(DataMem *mem, uint32_t addr);
static void data_mem_write8(DataMem *mem, uint32_t addr, uint8_t value);
static void data_mem_write16(DataMem *mem, uint32_t addr, uint16_t value);
static void data_mem_write32(DataMem *mem, uint32_t addr, uint32_t value);