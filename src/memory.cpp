#include <iostream>
#include <fstream>

#include "instruction_api.hpp"

//using namespace std;

//////////////////////////////set_instructions////////////////////////////////////////////
int memory::set_instructions(std::string filename){
    std::ifstream file;
    int count = 1, index = 0, i = 0;
    int length;
    unsigned int bit;
    uint8_t b;

    file.open(filename, std::ios::binary | std::ios::in);
	if (!file.is_open()) {
		exit(-21);
	}
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
    char a[length];
    file.read(a, length);

    file.close();
    while(i < length){
        b = a[i];
        bit = b;
        INST[index] = INST[index] | (bit << (32 - count*8));
        count++;
        if (count > 4){
            index++;
            count = 1;
        }
        i++;
    }
    return index;
}

//////////////////////////////get_instruction/////////////////////////////////////////////
uint32_t memory::get_instruction(int32_t address){
	if (check_word(PROG_COUNTER)=="inst") {
		uint32_t index = (address - 0x10000000) / 4;
		uint32_t instruction = INST[index];
		return instruction;
	}
	else {
		exit(-11);
	}
}

//////////////////////////////store_memory////////////////////////////////////////////////
int memory::store_memory(int32_t address, int32_t rt, char method){
    int return_code = 0, index, byte_offset, half_offset;
    std::string check;
    switch(method){
        case 'B':
            check = MEMORY.check_byte(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                byte_offset = (address - 0x20000000)%4;
                rt = rt & BYTE_MASK;
                DATA_MEM[index] = DATA_MEM[index] & ~(BYTE_MASK << (24 - byte_offset*8));
                DATA_MEM[index] = DATA_MEM[index] | (rt << (24 - byte_offset*8));
            }
            else if(check == "putc"){
                if(address == 0x30000007){
                    char c = rt & BYTE_MASK;
                    putchar(c);
                    if(ferror(stdout)){
                        exit(-21);
                    }
                }
                else{
                    putchar(0);
                    if(ferror(stdout)){
                        exit(-21);
                    }
                }
            }
            else{
                return_code = -11;
            }
            break;
        case 'H':
            check = MEMORY.check_half(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                half_offset = (address - 0x20000000)%4;
                rt = rt & HALFWORD_MASK;
                DATA_MEM[index] = DATA_MEM[index] & ~(HALFWORD_MASK << (16 - half_offset*8));
                DATA_MEM[index] = DATA_MEM[index] | (rt << (16 - half_offset*8));
            }
            else if(check == "putc"){
                if(address == 0x30000006){
                    char c = rt & BYTE_MASK;
                    putchar(c);
                    if(ferror(stdout)){
                        exit(-21);
                    }
                }
                else{
                    putchar(0);
                    if(ferror(stdout)){
                        exit(-21);
                    }
                }
            }
            else{
                return_code = -11;
            }
            break;
        case 'W':
            check = MEMORY.check_word(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                DATA_MEM[index] = rt;
            }
            else if(check == "putc"){
                char c = rt & BYTE_MASK;
                putchar(c);
                if(ferror(stdout)){
                    exit(-21);
                }
            }
            else{
                return_code = -11;
            }
            break;
    }
    return return_code;
}

/////////////////////////////////load_memory//////////////////////////////////////////////
int memory::load_memory(int32_t address, uint32_t rt, char method, bool sign){
    int return_code = 0, index, byte_offset, half_offset;
    std::string check;
    int8_t byte;
    uint8_t u_byte;
    int16_t halfword;
    uint16_t u_halfword;
    int32_t data;
    switch(method){
        case 'B':
            check = MEMORY.check_byte(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                byte_offset = (address - 0x20000000)%4;
                if(sign){
                    byte = (DATA_MEM[index] >> (24-8*byte_offset)) & BYTE_MASK;
                    data = byte;
                }
                else{
                    u_byte = (DATA_MEM[index] >> (24-8*byte_offset)) & BYTE_MASK;
                    data = u_byte;
                }
                REG[rt] = data;
            }
            else if(check == "inst"){
                index = (address - 0x10000000)/4;
                byte_offset = (address - 0x10000000)%4;
                if(sign){
                    byte = (INST[index] >> (24-8*byte_offset)) & BYTE_MASK;
                    data = byte;
                }
                else{
                    u_byte = (INST[index] >> (24-8*byte_offset)) & BYTE_MASK;
                    data = u_byte;
                }
                REG[rt] = data;
            }
            else if(check == "getc"){
                if(address == 0x30000003){
                    if(sign){
                        byte = getchar();
                        if(ferror(stdin)){
                            exit(-21);
                        }
                        data = byte;
                        REG[rt] = data;
                    }
                    else{
                        u_byte = getchar();
                        if(ferror(stdin)){
                            exit(-21);
                        }
                        data = u_byte;
                        REG[rt] = data;
                    }
                }
                else{
                    if(sign){
                        byte = getchar();
                        if(ferror(stdin)){
                            exit(-21);
                        }
                        if(byte == -1){
                            data = byte;
                            REG[rt] = data;
                        }
                        else{
                            REG[rt] = 0;
                        }
                    }
                    else{
                        u_byte = getchar();
                        if(ferror(stdin)){
                            exit(-21);
                        }
                        if(u_byte == 0xFF){
                            data = u_byte;
                            REG[rt] = data;
                        }
                        else{
                            REG[rt] = 0;
                        }
                    }
                }

            }
            else{
                return_code = -11;
            }
            break;
        case 'H':
            check = MEMORY.check_half(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                half_offset = (address - 0x20000000)%4;
                if(sign){
                    halfword = (DATA_MEM[index] >> (16-8*half_offset)) & HALFWORD_MASK;
                    data = halfword;
                }
                else{
                    u_halfword = (DATA_MEM[index] >> (16-8*half_offset)) & HALFWORD_MASK;
                    data = u_halfword;
                }
                REG[rt] = data;
            }
            else if(check == "inst"){
                index = (address - 0x10000000)/4;
                half_offset = (address - 0x10000000)%4;
                if(sign){
                    halfword = (INST[index] >> (16-8*half_offset)) & HALFWORD_MASK;
                    data = halfword;
                }
                else{
                    u_halfword = (INST[index] >> (16-8*half_offset)) & HALFWORD_MASK;
                    data = u_halfword;
                }
                REG[rt] = data;
            }
            else if(check == "getc"){
                if(address == 0x30000002){
                    u_byte = getchar();
                    if(ferror(stdin)){
                        exit(-21);
                    }
                    data = u_byte;
                    if(u_byte == 0xFF){
                        if(sign){
                            REG[rt] = 0xFFFFFFFF;
                        }
                        else{
                            REG[rt] = 0x0000FFFF;
                        }
                    }
                    else{
                        REG[rt] = data;
                    }
                }
                else{
                    if(sign){
                        halfword = getchar();
                        if(ferror(stdin)){
                            exit(-21);
                        }
                        if(halfword == -1){
                            data = halfword;
                            REG[rt] = data;
                        }
                        else{
                            REG[rt] = 0;
                        }
                    }
                    else{
                        u_halfword = getchar();
                        if(ferror(stdin)){
                            exit(-21);
                        }
                        if(u_halfword == 0xFFFF){
                            data = u_halfword;
                            REG[rt] = data;
                        }
                        else{
                            REG[rt] = 0;
                        }
                    }
                }
            }
            else{
                return_code = -11;
            }
            break;
        case 'W':
            check = MEMORY.check_word(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                REG[rt] = DATA_MEM[index];
            }
            else if(check == "inst"){
                index = (address - 0x10000000)/4;
                REG[rt] = INST[index];
            }
            else if(check == "getc"){
                u_byte = getchar();
                if(ferror(stdin)){
                    exit(-21);
                }
                if(u_byte == 0xFF){
                    REG[rt] = 0xFFFFFFFF;
                }
                else{
                    REG[rt] = u_byte;
                }
            }
            else{
                return_code = -11;
            }
            break;
    }
    return return_code;
}

/////////////////////////////////////load_unaligned_memory////////////////////////////////
int memory::load_unaligned_memory(int32_t address, uint32_t rt, char method){
    int return_code = 0, offset, index;
    uint8_t u_byte;
    std::string check;
    int32_t ms_bytes, ls_bytes, temp;
    uint32_t unsigned_shift;
    switch(method){
        case 'L':
            check = MEMORY.check_byte(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                offset = (address - 0x20000000)%4;
                ms_bytes = DATA_MEM[index] << 8*offset;
                REG[rt] = REG[rt] & ~(0xFFFFFFFF << 8*offset);
                REG[rt] = REG[rt] | ms_bytes;
            }
            else if(check == "inst"){
                index = (address - 0x10000000)/4;
                offset = (address - 0x10000000)%4;
                ms_bytes = INST[index] << 8*offset;
                REG[rt] = REG[rt] & ~(0xFFFFFFFF << 8*offset);
                REG[rt] = REG[rt] | ms_bytes;
            }
            else if(check == "getc"){
                if(address == 0x30000000){
                    u_byte = getchar();
                    if(ferror(stdin)){
                        exit(-21);
                    }
                    REG[rt] = u_byte;
                }
                else{
                    u_byte = getchar();
                    if(ferror(stdin)){
                        exit(-21);
                    }
                    temp = u_byte;
                    index = (address - 0x30000000)/4;
                    offset = (address - 0x30000000)%4;
                    ms_bytes = temp << 8*offset;
                    REG[rt] = REG[rt] & ~(0xFFFFFFFF << 8*offset);
                    REG[rt] = REG[rt] | ms_bytes;
                }
            }
            else{
                return_code = -11;
            }
            break;
        case 'R':
            check = MEMORY.check_byte(address);
            if(check == "data"){
                index = (address - 0x20000000)/4;
                offset = (address - 0x20000000)%4;
                unsigned_shift = DATA_MEM[index];
                ls_bytes = unsigned_shift >> (24-8*offset);
                REG[rt] = REG[rt] & ~(0xFFFFFFFF >> (24 - 8*offset));
                REG[rt] = REG[rt] | ls_bytes;
            }
            else if(check == "inst"){
                index = (address - 0x10000000)/4;
                offset = (address - 0x10000000)%4;
                unsigned_shift = INST[index];
                ls_bytes = unsigned_shift >> (24-8*offset);
                REG[rt] = REG[rt] & ~(0xFFFFFFFF >> (24 - 8*offset));
                REG[rt] = REG[rt] | ls_bytes;
            }
            else if(check == "getc"){
                if(address == 0x30000003){
                    u_byte = getchar();
                    if(ferror(stdin)){
                        exit(-21);
                    }
                    REG[rt] = u_byte;
                }
                else{
                    u_byte = getchar();
                    if(ferror(stdin)){
                        exit(-21);
                    }
                    temp = u_byte;
                    index = (address - 0x30000000)/4;
                    offset = (address - 0x30000000)%4;
                    unsigned_shift = temp;
                    ls_bytes = unsigned_shift >> (24-8*offset);
                    REG[rt] = REG[rt] & ~(0xFFFFFFFF >> (24 - 8*offset));
                    REG[rt] = REG[rt] | ls_bytes;
                }
            }
            else{
                return_code = -11;
            }
            break;
    }
    return return_code;
}
///////////////////////////////check_byte/////////////////////////////////////////////////
std::string memory::check_byte(int32_t address){
    std::string access;
    if(address >= 0x20000000 && address < 0x24000000){
        access = "data";
    }
    else if(address >= 0x30000004 && address < 0x30000008){
        access = "putc";
    }
    else if(address >= 0x30000000 && address < 0x30000004){
        access = "getc";
    }
    else if(address >= 0x10000000 && address < 0x11000000){
        access = "inst";
    }
    else{
        access = "error";
    }
    return access;
}
///////////////////////////////check_half/////////////////////////////////////////////////
std::string memory::check_half(int32_t address){
    std::string access;
    if((address & 0b1) == 0){
        if(address >= 0x20000000 && address < 0x24000000){
            access = "data";
        }
        else if(address >= 0x30000004 && address < 0x30000008){
            access = "putc";
        }
        else if(address >= 0x30000000 && address < 0x30000004){
            access = "getc";
        }
        else if(address >= 0x10000000 && address < 0x11000000){
            access = "inst";
        }
        else{
            access = "error";
        }
    }
    else{
        access = "error";
    }
    return access;
}
///////////////////////////////check_word/////////////////////////////////////////////////
std::string memory::check_word(int32_t address){
    std::string access;
    if((address & 0b11) == 0){
        if(address >= 0x20000000 && address < 0x24000000){
            access = "data";
        }
        else if(address >= 0x30000004 && address < 0x30000008){
            access = "putc";
        }
        else if(address >= 0x30000000 && address < 0x30000004){
            access = "getc";
        }
        else if(address >= 0x10000000 && address < 0x11000000){
            access = "inst";
        }
        else if(address == 0x00000000){
            access = "null";
        }
        else{
            access = "error";
        }
    }
    else{
        access = "error";
    }
    return access;
}
