#include "Memory.h"
#include "GLOBALS.h"

#include "Instruction.h"

#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

#ifndef ASSEMBLYPARSER_H
#define ASSEMBLYPARSER_H

class AssemblyParser
{

private:

	const string textDumpFile = "Text Dump.txt";

	map<string, unsigned int>labels;

	void getDataLabels(const string &);

	void getTextLabels(const string &);

	bool isInstruction(const string &);

	void parseData(const string &, Memory &);

	void parseText(const string &, vector<Instruction> &);

	int wordHexaToDecimal(const string &);
	
	short halfHexaToDecimal(const string &);

	char byteHexaToDecimal(const string &);

	vector<char> toChar(const string &);

public:

	AssemblyParser(const string &, Memory &, vector<Instruction> &);

};

#endif