#include <iostream>

#include "instruction_api.hpp"

int sort_I(const uint32_t instruction, const char type){
    int return_code;
    switch(instruction >> 26){
        case 4 :
            return_code = branch_instruction(instruction, type);
        case 5 :
            return_code = branch_instruction(instruction, type);
        case 6 :
            return_code = branch_instruction(instruction, type);
        case 7 :
            return_code = branch_instruction(instruction, type);
        case 8 :
            return_code = add_instruction(instruction, type);
        case 9 :
            return_code = add_instruction(instruction, type);
        case 10 :
            return_code = shift_instruction(instruction, type);
        case 11 :
            return_code = shift_instruction(instruction, type);
        case 12 :
            return_code = and_instruction(instruction, type);
        case 13 :
            return_code = or_instruction(instruction, type);
        case 14 :
            return_code = xor_instruction(instruction, type);
        case 15 :
            return_code = load_instruction(instruction, type);
    }
    return return_code;
}

int sort_R(const uint32_t instruction, const char type){
    uint32_t funct = instruction & FUNCT_MASK;
    switch(funct){
        
    }
}

int add_instruction(const uint32_t instruction, const char type){
    return 0;
}

int jump_instruction(const uint32_t instruction, const char type){
    return 0;
}

int and_instruction(const uint32_t instruction, const char type){
    return 0;
}

int div_instruction(const uint32_t instruction, const char type){
    return 0;
}

int mul_instruction(const uint32_t instruction, const char type){
    return 0;
}

int or_instruction(const uint32_t instruction, const char type){
    return 0;
}

int store_instruction(const uint32_t instruction, const char type){
    return 0;
}

int shift_instruction(const uint32_t instruction, const char type){
    return 0;
}

int xor_instruction(const uint32_t instruction, const char type){
    return 0;
}

int sub_instruction(const uint32_t instruction, const char type){
    return 0;
}

int mov_instruction(const uint32_t instruction, const char type){
    return 0;
}

int branch_instruction(const uint32_t instruction, const char type){
    return 0;
}

int load_instruction(const uint32_t instruction, const char type){
    return 0;
}
