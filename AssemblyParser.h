#include "Memory.h"
#include "GLOBALS.h"

#include <string>
#include <fstream>
#include <map>

using namespace std;

#ifndef ASSEMBLYPARSER_H
#define ASSEMBLYPARSER_H

class AssemblyParser
{

private:

	ifstream input;

	const string assemblyFile = "assembly.txt";

	ofstream output;

	map<string, unsigned int>labels;

	void getDataLabels(const string &);

	void getTextLabels(const string &);

	bool isInstruction(const string &);

	void parseData(const string &, Memory &);

	void parseText(const string &);

	int wordHexaToDecimal(const string &);
	
	short halfHexaToDecimal(const string &);

	char byteHexaToDecimal(const string &);

public:

	AssemblyParser(const string &, Memory &);

};

#endif