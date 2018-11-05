class memory{
    private:
        uint32_t         INST[4194304];
        int32_t          DATA_MEM[16777216];
        uint32_t         ADDR_NULL;
    public:
        int set_instructions(std::string filename);
        uint32_t get_instruction(int32_t address);
        bool check_instruction_address(int32_t address);
};
