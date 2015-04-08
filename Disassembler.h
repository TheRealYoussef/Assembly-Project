#ifndef __Mars__Disassembler__
#define __Mars__Disassembler__
#include "Instruction.h"

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
    vector<Instruction> inst;
    vector<unsigned int> instfile;
    vector<string>labels;
    unsigned int address;
    int offset;
    int brOffset=0;
    Instruction x;
    unsigned int pc=0x00400000;
    int temp_rd;
    int temp_rt;
    bool isLui;
    bool isOri;
    bool isAddi;
    bool isSub;
    bool isSubu;
    bool  printLi;
    bool  printLa;
    bool printSubi;
    bool printSubiu;
    
};
#endif /* defined(__Mars__Disassembler__) */
