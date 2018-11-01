

//////////////////////////MACROS//////////////////////////////////////////////////////////
const int FUNCT_MASK     = 63;
const int REG_MASK       = 31;
const int IMMEDIATE_MASK = 65535;

uint32_t         INST[4194304];
int32_t          REG[32];
int32_t          DATA_MEM[16777216];
const uint32_t   ADDR_NULL = 0;






//////////////////////////FUNCTIONS//////////////////////////////////////////////////////

int sort_I(const uint32_t instruction, const char type);

int sort_R(const uint32_t instruction, const char type);

int add_instruction(const uint32_t instruction, const char type);

int jump_instruction(const uint32_t instruction, const char type);

int and_instruction(const uint32_t instruction, const char type);

int div_instruction(const uint32_t instruction, const char type);

int mul_instruction(const uint32_t instruction, const char type);

int or_instruction(const uint32_t instruction, const char type);

int store_instruction(const uint32_t instruction, const char type);

int shift_instruction(const uint32_t instruction, const char type);

int xor_instruction(const uint32_t instruction, const char type);

int sub_instruction(const uint32_t instruction, const char type);

int mov_instruction(const uint32_t instruction, const char type);

int branch_instruction(const uint32_t instruction, const char type);

int load_instruction(const uint32_t instruction, const char type);

int set_instruction(const uint32_t instruction, const char type);

