
#ifndef __Project_Assembly_1__CPU__
#define __Project_Assembly_1__CPU__

#include <iostream>
using namespace std;

class CPU
{
public:
    CPU()
    {
        initialize();
    }
    int registers[32];
    unsigned int programCounter;
    void initialize()
    {
        for (int i=0; i<32; i++)
            registers[i]=0;
        registers[28]= 268468224;
        registers[29]= 0x10010000+8192-4;
        programCounter= 0x00400000;
    }
    
};

#endif /* defined(__Project_Assembly_1__CPU__) */
