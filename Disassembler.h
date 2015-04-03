#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Instruction.h"
#include <vector>

#ifndef __Mars__Disassembler__
#define __Mars__Disassembler__

class disassembler{
    
public:
    disassembler(string);
    void getData();
    void display();
private:
    char c;
    unsigned int instWord;
    ifstream inFile;
    string inFileName;
    Instruction inst;
    bool isLui;
    bool isOri;
    
};
#endif /* defined(__Mars__Disassembler__) */
