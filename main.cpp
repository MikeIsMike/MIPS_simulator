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
    REG[1] = pow(2,32)-10;
    REG[2] = 8;
    //REG[3] = 3;
    REG[4] = 4;
    REG[8] = -8;
    REG[10] = 0x10000010;
    //////////////////////////////////////////////////////////////////////////////////////
    while(PROG_COUNTER < (index + 0x10000000)){

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
    exit(0);
}
