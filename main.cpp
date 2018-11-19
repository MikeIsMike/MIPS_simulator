#include <iostream>
#include <cmath>
#include <fstream>

#include "instruction_api.hpp"
using namespace std;

int main(int argc, char* argv[]){
    int return_code;
    int index;
    bool branching;
    int branched_error = 0;
    uint32_t instruction;
    index = MEMORY.set_instructions(argv[1]);
    
    ////////////////TESTING REGISTER VALUES///////////////////////////////////////////////
    REG[1] = 0x30000000;
    //////////////////////////////////////////////////////////////////////////////////////
    while(PROG_COUNTER != 0){
        instruction = MEMORY.get_instruction(PROG_COUNTER);

        return_code = execute_instruction(instruction);
        
        if(return_code){
        //IF THERE IS AN ERROR AND NO BRANCH DELAY, IMMEDIATELY THROW EXCEPTION  
            cout << "ERROR: " << return_code << endl;
            cout << "Program Counter: " << PROG_COUNTER << endl;
            for(int j = 0; j < 32; j++){
            cout << "Register " << j << " : " << REG[j] << endl; 
            }
            exit(return_code);
        }
    }
    cout << "Program Counter: " << PROG_COUNTER << endl;
    for(int j = 0; j < 32; j++){
        cout << "Register " << j << " : " << REG[j] << endl; 
    }
    uint8_t exit_code = REG[2] & BYTE_MASK; 
    exit(exit_code);
}

