#include <iostream>
#include <fstream>

#include "memory.hpp"

using namespace std;

///////////////////////////////check_instruction_address//////////////////////////////////
bool memory::check_instruction_address(int32_t address){
    bool check;
    if(address == ADDR_NULL){
        check = true;
    }
    else if(address >= 0x10000000 && address < 0x11000000){
        check = true;
    }
    else{
        check = false;
    }
    return check;
}

//////////////////////////////set_instructions////////////////////////////////////////////
int memory::set_instructions(string filename){
    ifstream file;
    int count = 1, index = 0, i = 0;
    int length;
    unsigned int bit;
    uint8_t b;
    
    file.open(filename, ios::binary | ios::in);
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
}

//////////////////////////////get_instruction/////////////////////////////////////////////
uint32_t memory::get_instruction(int32_t address){
    uint32_t index = (address - 0x10000000)/4;
    uint32_t instruction = INST[index];
    return instruction;
}
