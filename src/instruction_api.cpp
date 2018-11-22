#include <iostream>
#include <sstream>

#include "instruction_api.hpp"
using namespace std;

const int FUNCT_MASK     = 0b111111;
const int REG_MASK       = 0b11111;
const int IMMEDIATE_MASK = 0b1111111111111111;
const int SHIFT_MASK     = 0b11111;
const int BASE_MASK      = 0b11111;
const int OFFSET_MASK    = 0b1111111111111111;
const int BYTE_MASK      = 0b11111111;
const int HALFWORD_MASK  = 0b1111111111111111;

int32_t          REG[32];
int32_t          PROG_COUNTER = 0x10000000;
int32_t          HI;
int32_t          LO;
memory           MEMORY;

void execute_branch_delay(int32_t& PROG_COUNTER, int32_t& offset, uint32_t& branch_delay, int& return_code);
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
    int16_t signed_immediate;
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
                    if(check_overflow(REG[rs], REG[rt])){
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
                    signed_immediate = instruction & IMMEDIATE_MASK;
                    immediate = signed_immediate;
                    if(check_overflow(REG[rs], immediate)){
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
                    signed_immediate = instruction & IMMEDIATE_MASK;
                    immediate = signed_immediate;
                    REG[rt] = REG[rs] + immediate;
                    break;
            }
    }
    return return_code;
}

//////////////////////////jump_instruction////////////////////////////////////////////
int jump_instruction(const uint32_t instruction, const char type){
    int return_code = 0;
    uint32_t instr_index;
    uint32_t rs, branch_delay, rd;
    int32_t address;
    string check;
    switch(type){
        case 'J' :
            switch(instruction >> 26){
                case 2 :
                    //J
                    instr_index = (instruction << 6) >> 4;
                    PROG_COUNTER = PROG_COUNTER + 4;
                    address = (PROG_COUNTER & 0xF0000000) | instr_index;
                    check = MEMORY.check_word(address);
                    if(check == "inst"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = address - 4;
                    }
                    else if(check == "null"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = address - 4;
                    }
                    else{
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        if(return_code == 0){
                            return_code = -11;
                        }
                    }
                    break;
                case 3 :
                    //JAL
                    REG[31] = PROG_COUNTER + 8;
                    instr_index = (instruction << 6) >> 4;
                    PROG_COUNTER = PROG_COUNTER + 4;
                    address = (PROG_COUNTER & 0xF0000000) | instr_index;
                    check = MEMORY.check_word(address);
                    if(check == "inst"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = address - 4;
                    }
                    else if(check == "null"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = address - 4;
                    }
                    else{
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        if(return_code == 0){
                            return_code = -11;
                        }
                    }
                    break;
            }
        case 'R' :
            switch(instruction & FUNCT_MASK){
                case 8 :
                    //JR
                    rs = (instruction >> 21) & REG_MASK;
                    PROG_COUNTER = PROG_COUNTER + 4;
                    check = MEMORY.check_word(REG[rs]);
                    if(check == "inst"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = REG[rs] - 4;
                    }
                    else if(check == "null"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = REG[rs] - 4;
                    }
                    else{
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        if(return_code == 0){
                            return_code = -11;
                        }
                    }
                    break;
                case 9 :
                    //JALR
                    rs = (instruction >> 21) & REG_MASK;
                    rd = (instruction >> 11) & REG_MASK;
                    REG[rd] = PROG_COUNTER + 8;
                    PROG_COUNTER = PROG_COUNTER + 4;
                    check = MEMORY.check_word(REG[rs]);
                    if(check == "inst"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = REG[rs] - 4;
                    }
                    else if(check == "null"){
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        PROG_COUNTER = REG[rs] - 4;
                    }
                    else{
                        branch_delay = MEMORY.get_instruction(PROG_COUNTER);
                        return_code = execute_instruction(branch_delay, true);
                        if(return_code == 0){
                            return_code = -11;
                        }
                    }
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
    int return_code = 0;
    uint32_t rs,rt;
    uint32_t u_regRs, u_regRt;
    rs = (instruction >> 21) & REG_MASK;
    rt = (instruction >> 16) & REG_MASK;
    switch(instruction & FUNCT_MASK){
        case 26:
            //DIV
            LO = REG[rs]/REG[rt];
            HI = REG[rs]%REG[rt];
            break;
        case 27:
            //DIVU
            u_regRs = REG[rs];
            u_regRt = REG[rt];
            LO = u_regRs/u_regRt;
            HI = u_regRs%u_regRt;
            break;
    }
    return return_code;
}

/////////////////////////////mul_instruction//////////////////////////////////////////
int mul_instruction(const uint32_t instruction, const char type){
    int return_code = 0;
    uint32_t rs,rt;
    uint32_t u_regRs, u_regRt;
	int64_t regRs, regRt;
    uint64_t u_mul;
    int64_t mul;
    rs = (instruction >> 21) & REG_MASK;
    rt = (instruction >> 16) & REG_MASK;
    switch(instruction & FUNCT_MASK){
        case 24:
            //MULT
			regRs = REG[rs];
			regRt = REG[rt];
            mul = regRs * regRt;
            HI = (mul >> 32) & 0xFFFFFFFF;
            LO = mul & 0xFFFFFFFF;
            break;
        case 25:
            //MULTU
            u_regRs = REG[rs];
            u_regRt = REG[rt];
            u_mul = u_regRs*u_regRt;
            HI = (u_mul >> 32) & 0xFFFFFFFF;
            LO = u_mul & 0xFFFFFFFF;
            break;
    }
    return return_code;
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
    int return_code = 0;
    int16_t immediate = instruction & IMMEDIATE_MASK;
    uint32_t base = (instruction >> 21) & REG_MASK;
    uint32_t rt = (instruction >> 16) & REG_MASK;
    int32_t offset = immediate;
    int32_t address = REG[base] + offset;
    switch(instruction >> 26){
        case 40:
            return_code = MEMORY.store_memory(address, REG[rt], 'B');
            break;
        case 41:
            return_code = MEMORY.store_memory(address, REG[rt], 'H');
            break;
        case 43:
            return_code = MEMORY.store_memory(address, REG[rt], 'W');
            break;
    }
    return return_code;
}

///////////////////////////shift_instruction//////////////////////////////////////////
int shift_instruction(const uint32_t instruction, const char type){
    uint32_t rs, rt, rd, shift, rt_L, rs_V;
    switch(instruction & FUNCT_MASK){
        case 0 :
            //SLL
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            shift = (instruction >> 6) & SHIFT_MASK;
            REG[rd] = REG[rt] << shift;
            break;
        case 2 :
            //SRL
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            shift = (instruction >> 6) & SHIFT_MASK;
            rt_L = REG[rt];
            REG[rd] = rt_L >> shift;
            break;
        case 3 :
            //SRA
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            shift = (instruction >> 6) & SHIFT_MASK;
            REG[rd] = REG[rt] >> shift;
            break;
        case 4 :
            //SLLV
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            rs_V = REG[rs] & SHIFT_MASK;
            REG[rd] = REG[rt] << rs_V;
            break;
        case 6 :
            //SRLV
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            rs_V = REG[rs] & SHIFT_MASK;
            rt_L = REG[rt];
            REG[rd] = rt_L >> rs_V;
            break;
        case 7 :
            //SRAV
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            rs_V = REG[rs] & SHIFT_MASK;
            REG[rd] = REG[rt] >> rs_V;
            break;
    }
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
            //std::cout<<"\n"<<REG[rs]<<" "<<REG[rt]<<" "<<REG[rd]<<std::endl;
            if(check_overflow_sub(REG[rs], REG[rt])){
                return_code = -10;
            }
            else{
                REG[rd] = REG[rs] - REG[rt];
                //cout << rs << " " << rt << " " << rd;
            }
            break;
        case 35:
            rs = (instruction >> 21) & REG_MASK;
            rt = (instruction >> 16) & REG_MASK;
            rd = (instruction >> 11) & REG_MASK;
            REG[rd] = REG[rs] - REG[rt];
            break;

    }

    //std::cout<< "\n"<<return_code<<std::endl;
    return return_code;
}

///////////////////////mov_instruction//////////////////////////////////////////////
int mov_instruction(const uint32_t instruction, const char type){
    int return_code = 0;
    uint32_t rd = (instruction >> 11) & REG_MASK;
    uint32_t rs = (instruction >> 21) & REG_MASK;
    switch(instruction & FUNCT_MASK){
        case 16:
            //MFHI
            REG[rd] = HI;
            break;
        case 17:
            //MTHI
            HI = REG[rs];
            break;
        case 18:
            //MFLO
            REG[rd] = LO;
            break;
        case 19:
            //MTLO
            LO = REG[rs];
            break;
    }
    return return_code;
}

//////////////////////branch_instruction////////////////////////////////////////////
int branch_instruction(const uint32_t instruction, const char type){
	uint32_t opcode = instruction >> 26;
	uint32_t branch_delay;
	int16_t offset0;
	int32_t rs, rt, offset; //they are all signed becuase case 1 requires signed rs.
	int return_code = 0;
	switch (opcode) {
	case 4:
		rs = (instruction >> 21) & REG_MASK;
		rt = (instruction >> 16) & REG_MASK;
		offset0 = (instruction & IMMEDIATE_MASK); //temporary variable for signed extension
		offset = offset0 << 2; //shift left by 2 to get 18 bits
		if (REG[rs] == REG[rt]) {
			execute_branch_delay(PROG_COUNTER, offset, branch_delay, return_code);
		}
		break;

	case 1: //BGEZ
		rs = (instruction >> 21) & REG_MASK;
		rt = (instruction >> 16) & REG_MASK; //to distinguish BGEZ, BGEZAL and etc.
		switch (rt) {
		case 1:
			if (REG[rs] >= 0) {
				execute_branch_delay(PROG_COUNTER, offset, branch_delay, return_code);
			}
			break;

		case 0b10001: //BGEZAL
			if (REG[rs] >= 0) {
				if (MEMORY.check_word(PROG_COUNTER + offset + 4)=="inst") {
					PROG_COUNTER = PROG_COUNTER + 4;
					branch_delay = MEMORY.get_instruction(PROG_COUNTER);
					return_code = execute_instruction(branch_delay, true);
					REG[31] = PROG_COUNTER + 4;
					PROG_COUNTER = PROG_COUNTER + offset - 4; ///check whether -4 is correct

				}
				else {
					PROG_COUNTER = PROG_COUNTER + 4;
					branch_delay = MEMORY.get_instruction(PROG_COUNTER);
					return_code = execute_instruction(branch_delay, true);
					if (return_code == 0) {
						return_code = -11;
					}
				}
			}
			break;

		case 0:
			rs = (instruction >> 21) & REG_MASK;
			offset0 = (instruction & IMMEDIATE_MASK); //shift left by 2 to get 18 bits
			offset = offset0 << 2;
			if (REG[rs]<0) {
				execute_branch_delay(PROG_COUNTER, offset, branch_delay, return_code);
			}
			break;

		case 0b10000:
			if (REG[rs]<0) {
				if (MEMORY.check_word(PROG_COUNTER + offset + 4) == "inst") {
					PROG_COUNTER = PROG_COUNTER + 4;
					branch_delay = MEMORY.get_instruction(PROG_COUNTER);
					return_code = execute_instruction(branch_delay, true);
					REG[31] = PROG_COUNTER + 4;
					PROG_COUNTER = PROG_COUNTER + offset - 4; ///check whether -4 is correct

				}
				else {
					PROG_COUNTER = PROG_COUNTER + 4;
					branch_delay = MEMORY.get_instruction(PROG_COUNTER);
					return_code = execute_instruction(branch_delay, true);
					if (return_code != 0) {
						return_code = -11;
					}
				}
			}
			break;

		}
		break;


	case 7:
		rs = (instruction >> 21) & REG_MASK;
		offset0 = (instruction & IMMEDIATE_MASK); //shift left by 2 to get 18 bits
		offset = offset0 << 2;
		if (REG[rs]>0) {
			execute_branch_delay(PROG_COUNTER, offset, branch_delay, return_code);
		}
		break;

	case 6:
		rs = (instruction >> 21) & REG_MASK;
		offset0 = (instruction & IMMEDIATE_MASK); //shift left by 2 to get 18 bits
		offset = offset0 << 2;
		if (REG[rs] <= 0) {
			execute_branch_delay(PROG_COUNTER, offset, branch_delay, return_code);
		}
		break;

	case 5:
		rs = (instruction >> 21) & REG_MASK;
		rt = (instruction >> 16) & REG_MASK;
		offset0 = (instruction & IMMEDIATE_MASK); //shift left by 2 to get 18 bits
		offset = offset0 << 2;
		if (REG[rs] != REG[rt]) {
			execute_branch_delay(PROG_COUNTER, offset, branch_delay, return_code);
		}
		break;


	}
	return return_code;
}

/////////////////////////load_instruction///////////////////////////////////////////
int load_instruction(const uint32_t instruction, const char type){
    int return_code = 0;
    int16_t immediate = instruction & IMMEDIATE_MASK;
    uint32_t base = (instruction >> 21) & REG_MASK;
    uint32_t rt = (instruction >> 16) & REG_MASK;
    int32_t offset = immediate;
    int32_t address = REG[base] + offset;
    switch(instruction >> 26){
        case 15:
            //LUI
            offset = offset << 16;
            REG[rt] = offset;
            break;
        case 32:
            //LB
            return_code = MEMORY.load_memory(address, rt, 'B', true);
            break;
        case 33:
            //LH
            return_code = MEMORY.load_memory(address, rt, 'H', true);
            break;
        case 34:
            //LWL
            return_code = MEMORY.load_unaligned_memory(address, rt, 'L');
            break;
        case 35:
            //LW
            return_code = MEMORY.load_memory(address, rt, 'W', true);
            break;
        case 36:
            //LBU
            return_code = MEMORY.load_memory(address, rt, 'B', false);
            break;
        case 37:
            //LHU
            return_code = MEMORY.load_memory(address, rt, 'H', false);
            break;
        case 38:
            //LWR
            return_code = MEMORY.load_unaligned_memory(address, rt, 'R');
            break;
    }
    return return_code;
}

////////////////////////////set_instruction///////////////////////////////////////////
int set_instruction(const uint32_t instruction, const char type){
    int16_t sixteenbit_immediate;
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
                    sixteenbit_immediate = instruction & IMMEDIATE_MASK;
                    signed_immediate = sixteenbit_immediate;
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
                    sixteenbit_immediate = instruction & IMMEDIATE_MASK;
                    immediate = sixteenbit_immediate;
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

/////////////////////////check_overflow/////////////////////////////////////////////
bool check_overflow(int32_t add1, int32_t add2){
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

bool check_overflow_sub(int32_t sub1, int32_t sub2){//this is checking signed overflow, is that a problem? Do we need to check unsigned overflow instead?
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

////////////////////////////////////execute_instruction///////////////////////////////////
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

///////////////////////////////////Branch delay//////////////////////////////////////////
void execute_branch_delay(int32_t& PROG_COUNTER, int32_t& offset, uint32_t& branch_delay, int& return_code) {
	PROG_COUNTER = PROG_COUNTER + 4;
	if (MEMORY.check_word(PROG_COUNTER + offset) == "inst") {
		branch_delay = MEMORY.get_instruction(PROG_COUNTER);
		return_code = execute_instruction(branch_delay, true);
		PROG_COUNTER = PROG_COUNTER + offset - 4;
	}
	else {
		branch_delay = MEMORY.get_instruction(PROG_COUNTER);
		return_code = execute_instruction(branch_delay, true);
		if (return_code == 0) {
			return_code = -11;
		}
	}
}
