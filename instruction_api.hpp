

//////////////////////////MACROS//////////////////////////////////////////////////////////
extern const int FUNCT_MASK;
extern const int REG_MASK;
extern const int IMMEDIATE_MASK;

extern uint32_t         INST[4194304];
extern int32_t          REG[32];
extern int32_t          DATA_MEM[16777216];
extern const uint32_t   ADDR_NULL;
extern int32_t          BRANCH_DELAY;
extern int32_t          PROG_COUNTER;






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

int32_t sign_extend_immediate(const uint32_t instruction);

bool check_overflow(int32_t add1, int32_t add2);

bool check_instruction_address(int32_t address);

