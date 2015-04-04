#include "Memory.h"

#include <iostream>

using namespace std;

char Memory::loadByte(unsigned int addr)
{
    addr -= 0x10010000;
    return memory[addr];
}

int Memory::loadWord(unsigned int addr)
{
    addr -= 0x10010000;
    return (memory[addr + 3] << 24) | (memory[addr + 2] << 16) | (memory[addr + 1] << 8) | memory[addr];
}

short Memory::loadHalf(unsigned int addr)
{
    addr -= 0x10010000;
    return (memory[addr + 1] << 8) | memory[addr];
}

void Memory::storeByte(unsigned int addr, char val)
{
    addr -= 0x10010000;
    memory[addr] = val;
}

void Memory::storeHalf(unsigned int addr, short val)
{
    addr -= 0x10010000;
    memory[addr] = val & 0xff;
    memory[addr + 1] = val >> 8;
}

void Memory::storeWord(unsigned int addr, int val)
{
    addr -= 0x10010000;
    memory[addr] = val & 0xff;
    memory[addr + 1] = (val >> 8) & 0xff;
    memory[addr + 2] = (val >> 16) & 0xff;
    memory[addr + 3] = (val >> 24);
}