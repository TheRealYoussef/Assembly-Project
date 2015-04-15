#ifndef ASSEMBLYPARSER_H
#define ASSEMBLYPARSER_H

#include "Memory.h"
#include "GLOBALS.h"

#include "Instruction.h"

#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

class AssemblyParser
{

private:

	string assemblyFile, textDumpFile, memoryDumpFile;

	map<string, unsigned int>labels;

	void getDataLabels();

	void getTextLabels();

	bool isInstruction(const string &);

	void parseData(Memory &);

	void parseText(vector<Instruction> &);

	int wordHexaToDecimal(const string &);
	
	short halfHexaToDecimal(const string &);

	char byteHexaToDecimal(const string &);

	void toChar(string, vector<char> &) const;

	char specialCharacter(char) const;

public:

	AssemblyParser(const string & assemblyCodeFile, const string & textDumpFile, const string & memoryDumpFile, Memory & memory, vector<Instruction> & instructionVector);

};

#endif