#include "AssemblyParser.h"
#include "Disassembler.h"
#include "Simulator.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

//Extension is .asm or .s
bool isAssemblyCodeFile(const string &);

//Extension is .bin
bool isDumpFile(const string &);

//-d <Text Dump File> [<Memory Dump File>] [<Assembly Code File>]
//Example 1: Project.exe -d TextDump.bin MemoryDump.bin AssemblyCode.asm
//Example 2: Project.exe -d TextDump.bin MemoryDump.bin AssemblyCode.s
//Example 3: Project.exe -d TextDump.bin MemoryDump.bin
//Example 4: Project.exe -d TextDump.bin AssemblyCode.asm
//Example 5: Project.exe -d TextDump.bin AssemblyCode.s
//Example 6: Project.exe -d TextDump.bin
//-a <Assembly Code File> [<Text Dump File>] [<Memory Dump File>]
//Example 1: Project.exe -a AssemblyCode.asm TextDump.bin MemoryDump.bin
//Example 2: Project.exe -a AssemblyCode.s TextDump.bin MemoryDump.bin
//Example 3: Project.exe -a AssemblyCode.asm TextDump.bin
//Example 4: Project.exe -a AssemblyCode.s TextDump.bin
//Example 5: Project.exe -a AssemblyCode.asm
//Example 6: Project.exe -a AssemblyCode.s
//Notes: 
//1- -d means disassembler, -a means assembler
//2- <> means necessary argument, [<>] means optional argument
//3- An argument should not contain any spaces
//4- The arguments are space-seperated
//5- Dump files should have the extension .bin
//6- Assembly code files should have the extension .asm or .s
//7- Either one or both of the optional arguments in the disassembler can be used
//7- If only one optional argument is used in the assembler, it is assumed to be the text dump file
int main(int argc, char *argv[])
{
	string programType = "";
	int programTypeIdx = 1, idx = 0;
	while (argv[programTypeIdx][idx] != '\0')
	{
		programType += argv[1][idx];
		idx++;
	}
	if (programType == "-d")
	{

	}
	else if (programType == "-a")
	{
		string assemblyCodeFile = "", textDumpFile = "", memoryDumpFile = "";
		int assemblyCodeFileIdx = 2, textDumpFileIdx = 3, memoryDumpFileIdx = 4;
		if (argc >= 3 && argc <= 5)
		{
			idx = 0;
			while (argv[assemblyCodeFileIdx][idx] != '\0')
			{
				assemblyCodeFile += argv[assemblyCodeFileIdx][idx];
				idx++;
			}
			if (isAssemblyCodeFile(assemblyCodeFile))
			{
				if (argc > 3)
				{
					idx = 0;
					while (argv[textDumpFileIdx][idx] != '\0')
					{
						textDumpFile += argv[textDumpFileIdx][idx];
						idx++;
					}
					if (isDumpFile(textDumpFile))
					{
						if (argc == 5)
						{
							idx = 0;
							while (argv[memoryDumpFileIdx][idx] != '\0')
							{
								memoryDumpFile += argv[memoryDumpFileIdx][idx];
								idx++;
							}
							if (!isDumpFile(memoryDumpFile))
							{
								cerr << "Invalid memory dump file path.\n";
								TERMINATE = true;
							}
						}
						else
							memoryDumpFile = "MemoryDump.bin";
					}
					else
					{
						cerr << "Invalid text dump file path.\n";
						TERMINATE = true;
					}
				}
				else
				{
					textDumpFile = "TextDump.bin";
					memoryDumpFile = "MemoryDump.bin";
				}
			}
			else
				cerr << "Invalid assembly code file path.\n";
		}
		else
			cerr << "Invalid command.\n";
		if (!TERMINATE)
		{
			Simulator sim;
			AssemblyParser ap(assemblyCodeFile, textDumpFile, memoryDumpFile, sim.memory, sim.program);
			if (!TERMINATE)
				sim.simulate();
		}
	}
	else
		cerr << "Invalid command.\n";
	return 0;
}

bool isAssemblyCodeFile(const string & assemblyCodeFile)
{
	int length = assemblyCodeFile.length();
	return ((assemblyCodeFile[length - 4] == '.' && assemblyCodeFile[length - 3] == 'a' && assemblyCodeFile[length - 2] == 's' && assemblyCodeFile[length - 1] == 'm') || (assemblyCodeFile[length - 2] == '.' && assemblyCodeFile[length - 1] == 's'));
}

bool isDumpFile(const string & dumpFile)
{
	int length = dumpFile.length();
	return (dumpFile[length - 4] == '.' && dumpFile[length - 3] == 'b' && dumpFile[length - 2] == 'i' && dumpFile[length - 1] == 'n');
}