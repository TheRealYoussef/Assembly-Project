#include "Disassembler.h"
#include <iostream>
#include <fstream>
#include "Instruction.h"
#include <vector>
#include "GLOBALS.h"
using namespace std;
disassembler::disassembler(string x){
    inFileName =x;
    getData();
    display();
    
}

void disassembler::getData(){
    inFile.open(inFileName.c_str());
    
    // If the file is open (this will evaluate to false if the file could not be found)
    if(inFile.is_open())
    {
        
        while(!inFile.eof())
        {
            instWord = 0;
            // Decode the instruction read from the file
            for (int i = 0; i < 4; i++)
            {
                if (!inFile.eof())
                {
                    inFile.get(c);
                    unsigned int temp = (unsigned int)((unsigned char)c);
                    instWord = instWord | (temp << (8 * i));
                }
                else
                    break;
            }
            if (!inFile.eof())
            {
                // instfile vector that stores all the binary instructions
                instfile.push_back(instWord);
                
            }
        }
        
    }
    
    
}
void disassembler::display(){
    
    for (int i=0; i<instfile.size(); i++) {
        
        //checking on the pseudo instructions li & la
        isLui = (instfile[i] == 0x0F);
        
        isOri = (instfile[i+1] == 0x0E);
        
        isLui != isOri? printLi = false:printLi;
        
        inst.dissassemble(instfile[i]); // calling the disassebling process
        
        cout << inst.getAssemblyInstruction() << endl;
    }
}






