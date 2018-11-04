#include <iostream>
#include <cmath>
#include <fstream>

#include "instruction_api.hpp"

using namespace std;

int main(int argc, char* argv[]){
    ifstream file;
    int count = 1, index = 0, i;
    int length;
    int k;
    int return_code;
    bool branching;
    int branched_error = 0;
    uint32_t opcode, instruction;
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
        INST[index] = INST[index] + (bit << (32 - count*8));
        count++;
        if (count > 4){
            index++;
            count = 1;
        }
        i++;
    }
    
    ////////////////TESTING REGISTER VALUES///////////////////////////////////////////////
    REG[1] = 1;
    REG[2] = 2;
    REG[3] = 3;
    REG[4] = 4;
    REG[8] = -8;
    REG[10] = 0x10000010;
    //////////////////////////////////////////////////////////////////////////////////////
    while(PROG_COUNTER < index && k < 3){
        if(BRANCH_DELAY){
            instruction = BRANCH_DELAY;
            branching = true;
        }
        else{
            instruction = INST[PROG_COUNTER];
        }
        //FIND OPCODE
        opcode = instruction >> 26;
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
        
        if(branching){
        //BRANCH_DELAY INSTRUCTION HAS BEEN EXECUTED, RESET TO PROGRAM COUNTER
            BRANCH_DELAY = 0;
            branching = false;
        }
        
        if(branched_error){
        //THROW EXCEPTION AFTER BRANCH_DELAY INSTRUCTION IS EXECUTED
            cout << "ERROR: " << branched_error << endl;
            cout << "Program Counter: " << PROG_COUNTER << endl;
            for(int j = 0; j < 32; j++){
            cout << "Register " << j << " : " << REG[j] << endl; 
            }
            return branched_error;
        }
        
        if(return_code && BRANCH_DELAY){
        // IF THERE IS AN ERROR AND BRANCH DELAY, THEN MUST DELAY EXCEPTION    
            branched_error = return_code;
        }
        
        else if(return_code && BRANCH_DELAY == 0){
        //IF THERE IS AN ERROR AND NO BRANCH DELAY, IMMEDIATELY THROW EXCEPTION  
            cout << "ERROR: " << return_code << endl;
            cout << "Program Counter: " << PROG_COUNTER << endl;
            for(int j = 0; j < 32; j++){
            cout << "Register " << j << " : " << REG[j] << endl; 
            }
            return return_code;
        }
        
    }
    cout << "Program Counter: " << PROG_COUNTER << endl;
    for(int j = 0; j < 32; j++){
        cout << "Register " << j << " : " << REG[j] << endl; 
    }
    return 0;
}

