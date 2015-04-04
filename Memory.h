#include <iostream>
using namespace std;

#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
public:
    char memory[4096];
    char loadByte(unsigned int);
    int loadWord(unsigned int);
    short loadHalf(unsigned int);
    void storeByte(unsigned int, char);
    void storeHalf(unsigned int, short);
    void storeWord(unsigned int, int);
};

#endif
