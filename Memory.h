#include <iostream>
using namespace std;

#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
private:
	//Address pointer for filling memory from data segment
	unsigned int currAddress;
	const string memoryDumpFile = "Memory Dump.txt";
public:
    char memory[8192];
    char loadByte(unsigned int);
    int loadWord(unsigned int);
    short loadHalf(unsigned int);
    void storeByte(unsigned int, char);
    void storeHalf(unsigned int, short);
    void storeWord(unsigned int, int);
	void MemoryArray(string);
	void memoryDump();
	//Filling memory from data segment
	Memory();
	void storeByte(char);
	void storeHalf(short);
	void storeWord(int);
};

#endif
