#include <iostream>

#include "instruction_api.hpp"
using namespace std;

const int FUNCT_MASK     = 0b111111;
const int REG_MASK       = 0b11111;
const int IMMEDIATE_MASK = 0b1111111111111111;

int32_t          REG[32];
int32_t          PROG_COUNTER = 0x10000000;
memory           MEMORY;
////////////////////////////////sort_I////////////////////////////////////////////////
int sort_I(const uint32_t instruction, const char type){
    int return_code;
    switch(instruction >> 26){
        case 1 :
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
        case 32 :
        case 33 :
        case 34 :
        case 35 :
        case 36 :
        case 37 :
        case 38 :
            return_code = load_instruction(instruction, type);
            break;
        case 40 :
        case 41 :
        case 43 :
            return_code = store_instruction(instruction, type);
            break;
    }
    return return_code;
}

///////////////////////////////sort_R/////////////////////////////////////////////////
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

    return return_code;
}

////////////////////////////add_instruction///////////////////////////////////////////
int add_instruction(const uint32_t instruction, const char type){
    uint32_t rs, rt, rd;
    int32_t immediate;
    int return_code = 0;
    switch(type){
        case 'R' :
            switch(instruction & FUNCT_MASK){
                case 32 :
                    //ADD
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    rd = (instruction >> 11) & REG_MASK;
                    if(check_overflow_add(REG[rs], REG[rt])){
                        return_code = -10;
                    }
                    else{
                        REG[rd] = REG[rs] + REG[rt];
                    }
                    break;
                case 33 :
                    //ADDU
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    rd = (instruction >> 11) & REG_MASK;
                    REG[rd] = REG[rs] + REG[rt];
                    break;
            }
        case 'I' :
            switch(instruction >> 26){
                case 8 :
                    //ADDI
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    immediate = sign_extend_immediate(instruction);
                    if(check_overflow_add(REG[rs], immediate)){
                        return_code = -10;
                    }
                    else{
                        REG[rt] = REG[rs] + immediate;
                    }
                    break;
                case 9 :
                    //ADDIU
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    immediate = sign_extend_immediate(instruction);
                    REG[rt] = REG[rs] + immediate;
                    break;
            }
    }
    return return_code;
}

//////////////////////////jump_instruction////////////////////////////////////////////
int jump_instruction(const uint32_t instruction, const char type){
    int return_code = 0;
    uint32_t rs, branch_delay;
    switch(type){
        case 'J' :
            switch(instruction >> 26){
                case 2 :
                    //J
                    break;
                case 3 :
                    //JAL
                    break;
            }
        case 'R' :
            switch(instruction & FUNCT_MASK){
                case 8 :
                    //JR
                    rs = (instruction >> 21) & REG_MASK;
                    //std::cout << REG[rs] << std::endl;
                    if(MEMORY.check_instruction_address(REG[rs])){
                        PROG_COUNTER = PROG_COUNTER + 4;
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = REG[rs] - 4;
                    }
                    else{
                        PROG_COUNTER = PROG_COUNTER + 4;
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        if(return_code != 0){
                            return_code = -11;
                        }
                    }
                    break;
                case 9 :
                    //JALR
                    break;
            }
    }
    return return_code;
}

/////////////////////////////////and_instruction//////////////////////////////////////
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

/////////////////////////////div_instruction//////////////////////////////////////////
int div_instruction(const uint32_t instruction, const char type){
    return 0;
}

/////////////////////////////mul_instruction//////////////////////////////////////////
int mul_instruction(const uint32_t instruction, const char type){
    return 0;
}

/////////////////////////////or_instruction///////////////////////////////////////////
int or_instruction(const uint32_t instruction, const char type){
    uint32_t rs, rt, rd, immediate;
    switch(type){
        case 'R':
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            REG[rd] = REG[rs] | REG[rt];
            break;
        case 'I' :
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            immediate = instruction & IMMEDIATE_MASK;
            REG[rt] = REG[rs] | immediate;
            break;
    }

    return 0;
}

///////////////////////////store_instruction/////////////////////////////////////////
int store_instruction(const uint32_t instruction, const char type){
    return 0;
}

///////////////////////////shift_instruction//////////////////////////////////////////
int shift_instruction(const uint32_t instruction, const char type){
    return 0;
}

//////////////////////////xor_instruction////////////////////////////////////////////
int xor_instruction(const uint32_t instruction, const char type){
    uint32_t rs, rt, rd, immediate;
    switch(type){
        case 'R':
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            REG[rd] = REG[rs] ^ REG[rt];
            break;
        case 'I' :
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            immediate = instruction & IMMEDIATE_MASK;
            REG[rt] = REG[rs] ^ immediate;
            break;
    }
    return 0;
}

///////////////////////sub_instruction//////////////////////////////////////////////
int sub_instruction(const uint32_t instruction, const char type){
    uint32_t rs, rt, rd;
    int return_code = 0;
    switch(instruction & FUNCT_MASK){
        case 34:
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            if(check_overflow_sub(REG[rs], REG[rt])){
                return_code = -10;
            }
            else{
                REG[rd] = REG[rs] - REG[rt];
                cout << rs << " " << rt << " " << rd;
            }
            break;
        case 35:
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            REG[rd] = REG[rs] - REG[rt];
            break;

    }
    return return_code;
}


///////////////////////mov_instruction//////////////////////////////////////////////
int mov_instruction(const uint32_t instruction, const char type){
    return 0;
}

//////////////////////branch_instruction////////////////////////////////////////////
int branch_instruction(const uint32_t instruction, const char type){
    return 0;
}

/////////////////////////load_instruction///////////////////////////////////////////
int load_instruction(const uint32_t instruction, const char type){
    return 0;
}

////////////////////////////set_instruction///////////////////////////////////////////
int set_instruction(const uint32_t instruction, const char type){
    uint32_t rd, rs, rt, immediate;
    uint32_t rs_val, rt_val;
    int return_code = 0, signed_immediate;
    switch(type){
        case 'R' :
            switch(instruction & FUNCT_MASK){
                case 42 :
                    //SLT
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    rd = (instruction >> 11) & REG_MASK;
                    if(REG[rs] < REG[rt]){
                        REG[rd] = 1;
                    }
                    else{
                        REG[rd] = 0;
                    }
                    break;
                case 43 :
                    //SLTU
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    rd = (instruction >> 11) & REG_MASK;
                    rs_val = REG[rs];
                    rt_val = REG[rt];
                    if(rs_val < rt_val){
                        REG[rd] = 1;
                    }
                    else{
                        REG[rd] = 0;
                    }
                    break;
            }
        case 'I' :
            switch(instruction >> 26){
                case 10 :
                    //SLTI
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    signed_immediate = sign_extend_immediate(instruction);
                    if(REG[rs] < signed_immediate){
                        REG[rt] = 1;
                    }
                    else{
                        REG[rt] = 0;
                    }
                    break;
                case 11 :
                    //SLTIU
                    rs = (instruction >> 21) & REG_MASK;
                    rt = (instruction >> 16) & REG_MASK;
                    rs_val = REG[rs];
                    signed_immediate = sign_extend_immediate(instruction);
                    immediate = signed_immediate;
                    if(rs_val < immediate){
                        REG[rt] = 1;
                    }
                    else{
                        REG[rt] = 0;
                    }
                    break;
            }
    }
    return return_code;
}

//////////////////////////sign_extend_immediate//////////////////////////////////////
int32_t sign_extend_immediate(const uint32_t instruction){
    int32_t immediate;
    immediate = ((instruction & 0b1000000000000000) == 0) ? (instruction & IMMEDIATE_MASK) : ((instruction & IMMEDIATE_MASK) | 0xffff0000);

    return immediate;
}

/////////////////////////check_overflow/////////////////////////////////////////////
bool check_overflow_add(int32_t add1, int32_t add2){
    bool overflow;
    if(add1 > 0 && add2 > 0){
        if((add1 + add2) <= 0){
            overflow = true;
        }
        else{
            overflow = false;
        }
    }
    else if(add1 < 0 && add2 < 0){
        if((add1 + add2) >= 0){
            overflow = true;
        }
        else{
            overflow = false;
        }
    }
    else{
        overflow = false;
    }

    return overflow;
}

bool check_overflow_sub(int32_t sub1, int32_t sub2){
    bool overflow;
    if(sub1>0 && sub2<0){
        if((sub1 - sub2) <= 0){
            overflow = true;
        }
        else{
            overflow = false;
        }
    }
    else if(sub1<0 && sub2>0){
        if((sub1-sub2) >=0){
            overflow = true;
        }
        else{
            overflow = false;
        }
    }
    else{
        overflow = false;
    }



    return overflow;
}


////////////////////////////////////branch_delay//////////////////////////////////////
int execute_instruction(uint32_t instruction, bool branch_delay){
    uint32_t opcode = instruction >> 26;
    int return_code;
            //SORT THROUGH
        switch(opcode){
            case 0 :
                return_code = sort_R(instruction, 'R');
                break;
            case 2 :
            case 3 :
                return_code = jump_instruction(instruction, 'J');
                break;
            default :
                return_code = sort_I(instruction, 'I');
        }
    if(!branch_delay){
        PROG_COUNTER = PROG_COUNTER + 4;
    }
    return return_code;
}
