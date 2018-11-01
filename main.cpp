#include <iostream>
#include <cmath>
#include <fstream>

#include "instruction_api.hpp"

using namespace std;
uint32_t inst[4194304];
uint32_t reg[32];
uint32_t data_mem[16777216];
const uint32_t addr_null = 0;

int main(int argc, char* argv[]){
    ifstream file;
    int count = 1, index = 0, i;
    int length;
    int return_code;
    uint32_t opcode;
    unsigned int bit;
    uint8_t b;
    file.open(argv[1], ios::binary | ios::in);
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
    char a[length];
    file.read(a, length);

    file.close();
    while(i < length){
        b = a[i];
        bit = b;
        inst[index] = inst[index] + (bit << (32 - count*8));
        count++;
        if (count > 4){
            index++;
            count = 1;
        }
        i++;
    }

    while(reg[0] < index){
        opcode = inst[reg[0]] >> 26;
            if(opcode == 0){
                return_code = sort_R(inst[reg[0]], 'R');
            }
            else if(32 <= opcode <= 38){
                return_code = load_instruction(inst[reg[0]], 'I');
            }
            else if(40 <= opcode <= 43){
                return_code = store_instruction(inst[reg[0]], 'I');
            }
            else if(opcode == 1){
                return_code = branch_instruction(inst[reg[0]], 'I');
            }
            else if (opcode == 2 || opcode == 3){
                return_code = jump_instruction(inst[reg[0]], 'J');
            }
            else if (4 <= opcode <= 15){
                return_code = sort_I(inst[reg[0]], 'I');
            }
        if(return_code){
            return return_code;
        }    
    }
    
    return 0;
}

