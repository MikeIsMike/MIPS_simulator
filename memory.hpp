class memory{
    private:
        uint32_t         INST[4194304];
        int32_t          DATA_MEM[16777216];
        uint32_t         ADDR_NULL = 0;
    public:
        int set_instructions(std::string filename);
        uint32_t get_instruction(int32_t address);
        int store_memory(int32_t address, int32_t rt, char method);
        int load_memory(int32_t address, uint32_t rt, char method, bool sign);
        int load_unaligned_memory(int32_t address, uint32_t rt, char method);
        std::string check_byte(int32_t address);
        std::string check_half(int32_t address);
        std::string check_word(int32_t address);
};
