#include "Memory.h"

#include <iostream>

#include <cstring>

using namespace std;

Memory::Memory()
{
	currAddress = 268500992;
	memset(memory, 0, sizeof memory);
}

char Memory::loadByte(unsigned int addr)
{
    addr -= 0x10010000;
    return memory[addr];
}

int Memory::loadWord(unsigned int addr)
{
    return ((unsigned char)(memory[addr + 3]) << 24) | ((unsigned char)(memory[addr + 2]) << 16) | ((unsigned char)(memory[addr + 1]) << 8) | (unsigned char)(memory[addr]);
}

short Memory::loadHalf(unsigned int addr)
{
    return ((unsigned char)(memory[addr + 1]) << 8) | (unsigned char)(memory[addr]);
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

void Memory::storeByte(char val)
{
	memory[currAddress] = val;
	currAddress++;
}

void Memory::storeHalf(short val)
{
	memory[currAddress] = val & 0xff;
	memory[currAddress + 1] = val >> 8;
	currAddress += 2;
}

void Memory::storeWord(int val)
{
	memory[currAddress] = val & 0xff;
	memory[currAddress + 1] = (val >> 8) & 0xff;
	memory[currAddress + 2] = (val >> 16) & 0xff;
	memory[currAddress + 3] = (val >> 24);
	currAddress += 4;
}
