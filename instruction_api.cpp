#include <iostream>

#include "instruction_api.hpp"

int sort_I(const uint32_t instruction, const char type){
    int return_code;
    switch(instruction >> 26){
        case 4 :
        case 5 :
        case 6 :
        case 7 :
            return_code = branch_instruction(instruction, type);
            break;
        case 8 :
        case 9 :
            return_code = add_instruction(instruction, type);
            break;
        case 10 :
        case 11 :
            return_code = set_instruction(instruction, type);
            break;
        case 12 :
            return_code = and_instruction(instruction, type);
            break;
        case 13 :
            return_code = or_instruction(instruction, type);
            break;
        case 14 :
            return_code = xor_instruction(instruction, type);
            break;
        case 15 :
            return_code = load_instruction(instruction, type);
            break;
    }
    return return_code;
}

int sort_R(const uint32_t instruction, const char type){
    uint32_t funct = instruction & FUNCT_MASK;
    int return_code;
    switch(funct){
        case 0 :
        case 2 :
        case 3 :
        case 4 :
        case 6 :
        case 7 :
            return_code = shift_instruction(instruction, type);
            break;
        case 8 :
        case 9 :
            return_code = jump_instruction(instruction, type);
            break;
        case 16 :
        case 17 :
        case 18 :
        case 19 :
            return_code = mov_instruction(instruction, type);
            break;
        case 24 :
        case 25 :
            return_code = mul_instruction(instruction, type);
            break;
        case 26 :
        case 27 :
            return_code = div_instruction(instruction, type);
            break;
        case 32 :
        case 33 :
            return_code = add_instruction(instruction, type);
            break;
        case 34 :
        case 35 :
            return_code = sub_instruction(instruction, type);
            break;
        case 36 :
            return_code = and_instruction(instruction, type);
            break;
        case 37 :
            return_code = or_instruction(instruction, type);
            break;
        case 38 :
            return_code = xor_instruction(instruction, type);
            break;
        case 42 :
        case 43 :
            return_code = set_instruction(instruction, type);
            break;
    }
}

int add_instruction(const uint32_t instruction, const char type){
    return 0;
}

int jump_instruction(const uint32_t instruction, const char type){
    return 0;
}

int and_instruction(const uint32_t instruction, const char type){
    uint32_t rs, rt, rd, immediate;
    switch(type){
        case 'R':
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            REG[rd] = REG[rs] & REG[rt];
            break;
        case 'I' :
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            immediate = instruction & IMMEDIATE_MASK;
            REG[rt] = REG[rs] & immediate;
            break;
    }
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
