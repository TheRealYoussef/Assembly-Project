#include "AssemblyParser.h"
#include "Memory.h"
#include "GLOBALS.h"

#include <string>

using namespace std;

AssemblyParser::AssemblyParser(const string & file, Memory & memory)
{
	getDataLabels(file);
	parseData(file, memory);
	getTextLabels(file);
	if (!TERMINATE)
		parseText(file);
}

void AssemblyParser::getDataLabels(const string & file)
{
	unsigned int dataAddress = 0x10010000;
	string label;
	input.open(file.c_str());
	input >> label;
	if (label == ".data")
	{
		input >> label;
		while (label != ".text" && !input.eof())
		{
			if (label == ".word")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
					input.get(c);
				while (c != '\n')
				{
					if (isdigit(c) || isalpha(c) || c == '\'')
					{
						while (dataAddress % 4)
							dataAddress++;
						dataAddress += 4;
						if (c == '\'')
						{
							input.get(c);
							while (c != '\'')
								input.get(c);
						}
						while (c != ' ' && c != ',' && c != '#' && c != '\n')
							input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n')
							input.get(c);
						break;
					}
				}
			}
			else if (label == ".half")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
					input.get(c);
				while (c != '\n')
				{
					if (isdigit(c) || isalpha(c) || c == '\'')
					{
						while (dataAddress % 2)
							dataAddress++;
						dataAddress += 2;
						if (c == '\'')
						{
							input.get(c);
							while (c != '\'')
								input.get(c);
						}
						while (c != ' ' && c != ',' && c != '#' && c != '\n')
							input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n')
							input.get(c);
						break;
					}
				}
			}
			else if (label == ".byte")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
					input.get(c);
				while (c != '\n')
				{
					if (isdigit(c) || isalpha(c) || c == '\'')
					{
						dataAddress++;
						if (c == '\'')
						{
							input.get(c);
							while (c != '\'')
								input.get(c);
						}
						while (c != ' ' && c != ',' && c != '#' && c != '\n')
							input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n')
							input.get(c);
						break;
					}
				}
			}
			else if (label == ".space")
			{
				char c;
				input.get(c);
				while (!isdigit(c) && c != '#' && c != '\n' && c != '\'')
					input.get(c);
				if (isdigit(c))
				{
					string n = "";
					while (c != '#' && c != '\n' && c != ' ')
					{
						n += c;
						input.get(c);
					}
					if (n.length() > 2)
					{
						if (n[1] == 'x')
							dataAddress += (unsigned int)wordHexaToDecimal(n.substr(2));
						else
							dataAddress += (unsigned int)atoi(n.c_str());
					}
					else
						dataAddress += (unsigned int)atoi(n.c_str());
					while (c != '\n')
						input.get(c);
				}
				else if (c == '#')
				{
					while (c != '\n')
						input.get(c);
				}
				else if (c == '\'')
				{
					input.get(c);
					dataAddress += (unsigned int)c;
					while (c != '\n')
						input.get(c);
				}
			}
			else if (label == ".ascii")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n')
				{
					while (c != '"' && c != '#' && c != '\n')
						input.get(c);
					if (c == '"')
					{
						input.get(c);
						while (c != '"')
						{
							dataAddress++;
							input.get(c);
						}
						input.get(c);
					}
					else if (c == '#')
					{
						while (c != '\n')
							input.get(c);
					}
				}
			}
			else if (label == ".asciiz")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n')
				{
					while (c != '"' && c != '#' && c != '\n')
						input.get(c);
					if (c == '"')
					{
						input.get(c);
						while (c != '"')
						{
							dataAddress++;
							input.get(c);
						}
						dataAddress++;
						input.get(c);
					}
					else if (c == '#')
					{
						while (c != '\n')
							input.get(c);
					}
				}
			}
			else
				labels[label.substr(0, label.length() - 1)] = dataAddress;
			input >> label;
		}
	}
	input.close();
}

void AssemblyParser::getTextLabels(const string & str)
{
	unsigned int textAddress = 0x00400000;
	string instruction;
	ifstream input(str.c_str());
	input >> instruction;
	while (instruction != ".text")
		input >> instruction;
	input >> instruction;
	while (instruction != ".data" && !input.eof() && !terminate)
	{
		if (instruction == ".globl")
			input >> instruction;
		else if (isInstruction(instruction))
		{
			char c;
			input.get(c);
			while (c != '\n')
				input.get(c);
			textAddress += 4;
		}
		else if (instruction[instruction.length() - 1] == ':')
			labels[instruction.substr(0, instruction.length() - 1)] = textAddress;
		else
		{
			if (instruction == "li" || instruction == "la")
				textAddress += 8;
			else if (instruction == "subi" || instruction == "move")
				textAddress += 4;
			else
			{
				cout << "Parsing error!\n";
				TERMINATE = true;
			}
		}
		input >> instruction;
	}
}

void AssemblyParser::parseText(const string & file)
{
	input.open(file.c_str());
	output.open(assemblyFile.c_str());
	string str;
	input >> str;
	while (str != ".text")
		input >> str;
	input >> str;
	while (!input.eof() && str != ".data")
	{
		if (str == ".globl")
			input >> str;
		else if (isInstruction(str))
		{
			output << str;
			char c;
			input.get(c);
			while (c != '\n' && c != '#')
			{
				output << c;
				input.get(c);
			}
			while (c != '\n')
				input.get(c);
		}
		else if (str[str.length() - 1] == ':')
			continue;
		else
		{
			if (str == "li")
			{
			}
			else if (str == "la")
			{
			}
			else if (str == "move")
			{
			}
			else if (str == "subi")
			{
			}
		}
		input >> str;
	}
}

bool AssemblyParser::isInstruction(const string & instruction)
{
	return (instruction == "j" || instruction == "jal" || instruction == "add" || instruction == "addu" || instruction == "sub" || instruction == "and" || instruction == "or" || instruction == "xor" || instruction == "slt" || instruction == "jr" || instruction == "sll" || instruction == "srl" || instruction == "syscall" || instruction == "lui" || instruction == "beq" || instruction == "bne" || instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh" || instruction == "addi" || instruction == "addiu" || instruction == "andi" || instruction == "ori" || instruction == "xori");
}

void AssemblyParser::parseData(const string & file, Memory & memory)
{
	string directive;
	input.open(file.c_str());
	input >> directive;
	if (directive == ".data")
	{
		input >> directive;
		while (directive != ".text" && !input.eof())
		{	
			if (directive == ".word")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
					input.get(c);
				while (c != '\n')
				{
					if (isdigit(c) || isalpha(c) || c == '\'')
					{
						string word = "";
						if (c == '\'')
						{
							input.get(c);
							memory.storeWord((int)(unsigned char)(c));
							input.get(c);
						}
						else
						{
							while (c != ' ' && c != ',' && c != '#' && c != '\n')
							{
								word += c;
								input.get(c);
							}
							if (isalpha(word[0]))
								memory.storeWord(labels[word]);
							else
							{
								if (word.length() > 2)
								{
									if (word[1] == 'x')
										memory.storeWord(wordHexaToDecimal(word.substr(2)));
									else
										memory.storeWord(atoi(word.c_str()));
								}
								else
									memory.storeWord(atoi(word.c_str()));
							}
						}
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n')
							input.get(c);
						break;
					}
				}
			}
			else if (directive == ".half")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
					input.get(c);
				while (c != '\n')
				{
					if (isdigit(c) || isalpha(c) || c == '\'')
					{
						string half = "";
						if (c == '\'')
						{
							input.get(c);
							memory.storeHalf((short)(unsigned char)(c));
							input.get(c);
						}
						else
						{
							while (c != ' ' && c != ',' && c != '#' && c != '\n')
							{
								half += c;
								input.get(c);
							}
							if (isalpha(half[0]))
								memory.storeHalf((short)(labels[half]));
							else
							{
								if (half.length() > 2)
								{
									if (half[1] == 'x')
										memory.storeHalf(halfHexaToDecimal(half.substr(2)));
									else
										memory.storeHalf((short)(atoi(half.c_str())));
								}
								else
									memory.storeHalf((short)(atoi(half.c_str())));
							}
						}
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n')
							input.get(c);
						break;
					}
				}
			}
			else if (directive == ".byte")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
					input.get(c);
				while (c != '\n')
				{
					if (isdigit(c) || isalpha(c) || c == '\'')
					{
						string byte = "";
						if (c == '\'')
						{
							input.get(c);
							memory.storeByte(c);
							input.get(c);
							while (c != ' ' && c != ',' && c != '#' && c != '\n')
								input.get(c);
						}
						else
						{
							while (c != ' ' && c != ',' && c != '#' && c != '\n')
							{
								byte += c;
								input.get(c);
							}
							if (isalpha(byte[0]))
								memory.storeByte((char)(labels[byte]));
							else
							{
								if (byte.length() > 2)
								{
									if (byte[1] == 'x')
										memory.storeByte(byteHexaToDecimal(byte.substr(2)));
									else
										memory.storeByte((char)(atoi(byte.c_str())));
								}
								else
									memory.storeByte((char)(atoi(byte.c_str())));
							}
						}
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n')
							input.get(c);
						break;
					}
				}
			}
			else if (directive == ".space")
			{
				char c;
				input.get(c);
				while (!isdigit(c) && c != '#' && c != '\n' && c != '\'')
					input.get(c);
				if (c == '\'')
				{
					input.get(c);
					for (char i = 0; i < c; i++)
						memory.storeByte('\0');
					input.get(c);
				}
				else if (isdigit(c))
				{
					string n = "";
					while (c != '#' && c != '\n')
					{
						n += c;
						input.get(c);
					}
					if (n.length() > 2)
					{
						if (n[1] == 'x')
						{
							for (int i = 0; i < wordHexaToDecimal(n.substr(2)); i++)
								memory.storeByte('\0');
						}
						else
						{
							for (int i = 0; i < atoi(n.c_str()); i++)
								memory.storeByte('\0');
						}
					}
					else
					{
						for (int i = 0; i < atoi(n.c_str()); i++)
							memory.storeByte('\0');
					}
				}
				while (c != '\n')
					input.get(c);
			}
			else if (directive == ".ascii")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n')
				{
					while (c != '"' && c != '#' && c != '\n')
						input.get(c);
					if (c == '"')
					{
						input.get(c);
						while (c != '"')
						{
							memory.storeByte(c);
							input.get(c);
						}
						input.get(c);
					}
				}
				while (c != '\n')
					input.get(c);
			}
			else if (directive == ".asciiz")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n')
				{
					while (c != '"' && c != '#' && c != '\n')
						input.get(c);
					if (c == '"')
					{
						input.get(c);
						while (c != '"')
						{
							memory.storeByte(c);
							input.get(c);
						}
						memory.storeByte('\0');
						input.get(c);
					}
				}
				while (c != '\n')
					input.get(c);
			}
			input >> directive;
		}
	}
	input.close();
}

int AssemblyParser::wordHexaToDecimal(const string & hexa)
{
	int ans = 0;
	int i = hexa.length() - 1;
	int n = 8;
	while (i >= 0 && n > 0)
	{
		if (hexa[i] >= '0' && hexa[i] <= '9')
			ans += ((hexa[i] - '0') << (4 * (hexa.length() - 1 - i)));
		else if (hexa[i] >= 'a' && hexa[i] <= 'f')
			ans += ((hexa[i] - 'a' + 10) << (4 * (hexa.length() - 1 - i)));
		else if (hexa[i] >= 'A' && hexa[i] <= 'F')
			ans += ((hexa[i] - 'A' + 10) << (4 * (hexa.length() - 1 - i)));
		i--;
		n--;
	}
	return ans;
}

short AssemblyParser::halfHexaToDecimal(const string & hexa)
{
	short ans = 0;
	int i = hexa.length() - 1;
	int n = 4;
	while (i >= 0 && n > 0)
	{
		if (hexa[i] >= '0' && hexa[i] <= '9')
			ans += ((hexa[i] - '0') << (4 * (hexa.length() - 1 - i)));
		else if (hexa[i] >= 'a' && hexa[i] <= 'f')
			ans += ((hexa[i] - 'a' + 10) << (4 * (hexa.length() - 1 - i)));
		else if (hexa[i] >= 'A' && hexa[i] <= 'F')
			ans += ((hexa[i] - 'A' + 10) << (4 * (hexa.length() - 1 - i)));
		i--;
		n--;
	}
	return ans;
}

char AssemblyParser::byteHexaToDecimal(const string & hexa)
{
	char ans = 0;
	int i = hexa.length() - 1;
	int n = 2;
	while (i >= 0 && n > 0)
	{
		if (hexa[i] >= '0' && hexa[i] <= '9')
			ans += ((hexa[i] - '0') << (4 * (hexa.length() - 1 - i)));
		else if (hexa[i] >= 'a' && hexa[i] <= 'f')
			ans += ((hexa[i] - 'a' + 10) << (4 * (hexa.length() - 1 - i)));
		else if (hexa[i] >= 'A' && hexa[i] <= 'F')
			ans += ((hexa[i] - 'A' + 10) << (4 * (hexa.length() - 1 - i)));
		i--;
		n--;
	}
	return ans;
}