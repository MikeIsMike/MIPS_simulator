

//////////////////////////MACROS//////////////////////////////////////////////////////////
const int FUNCT_MASK = 63;








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

