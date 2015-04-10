#include "AssemblyParser.h"
#include "Memory.h"
#include "Instruction.h"
#include "GLOBALS.h"

#include <string>
#include <fstream>
#include <vector>

using namespace std;

AssemblyParser::AssemblyParser(const string & file, Memory & memory, vector<Instruction> & inst)
{
	//No need for comments, just parsing
	getDataLabels(file);
	parseData(file, memory);
	getTextLabels(file);
	if (!TERMINATE)
		parseText(file, inst);
}

void AssemblyParser::getDataLabels(const string & file)
{
	unsigned int dataAddress = 0x10010000;
	string label;
	ifstream input;
	input.open(file.c_str());
	input >> label;
	while (label != ".data" && !input.eof())
		input >> label;
	if (label == ".data")
	{
		input >> label;
		while (label != ".text" && !input.eof())
		{
			if (label[0] == '#')
			{
				char c;
				input.get(c);
				while (c != '\n')
					input.get(c);
			}
			else if (label == ".word")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
					input.get(c);
				while (c != '\n' && !input.eof())
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
						while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n' && !input.eof())
							input.get(c);
						break;
					}
				}
			}
			else if (label == ".half")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
					input.get(c);
				while (c != '\n' && !input.eof())
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
						while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n' && !input.eof())
							input.get(c);
						break;
					}
				}
			}
			else if (label == ".byte")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
					input.get(c);
				while (c != '\n' && !input.eof())
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
						while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n' && !input.eof())
							input.get(c);
						break;
					}
				}
			}
			else if (label == ".space")
			{
				char c;
				input.get(c);
				while (!isdigit(c) && c != '#' && c != '\n' && c != '\'' && !input.eof())
					input.get(c);
				if (isdigit(c))
				{
					string n = "";
					while (c != '#' && c != '\n' && c != ' ' && !input.eof())
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
					while (c != '\n' && !input.eof())
						input.get(c);
				}
				else if (c == '#')
				{
					while (c != '\n' && !input.eof())
						input.get(c);
				}
				else if (c == '\'')
				{
					input.get(c);
					dataAddress += (unsigned int)c;
					while (c != '\n' && !input.eof())
						input.get(c);
				}
			}
			else if (label == ".ascii")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n' && !input.eof())
				{
					while (c != '"' && c != '#' && c != '\n' && !input.eof())
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
						while (c != '\n' && !input.eof())
							input.get(c);
					}
				}
			}
			else if (label == ".asciiz")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n' && !input.eof())
				{
					while (c != '"' && c != '#' && c != '\n' && !input.eof())
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
						while (c != '\n' && !input.eof())
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
	while (instruction != ".text" && !input.eof())
		input >> instruction;
	input >> instruction;
	while (instruction != ".data" && !input.eof() && !TERMINATE)
	{
		if (instruction[0] == '#')
		{
			char c;
			input.get(c);
			while (c != '\n')
				input.get(c);
		}
		else if (instruction == ".globl")
			input >> instruction;
		else if (isInstruction(instruction))
		{
			char c;
			input.get(c);
			while (c != '\n' && !input.eof())
				input.get(c);
			textAddress += 4;
		}
		else if (instruction[instruction.length() - 1] == ':')
			labels[instruction.substr(0, instruction.length() - 1)] = textAddress;
		else
		{
			if (instruction == "li" || instruction == "la")
			{
				char c;
				input.get(c);
				while (c != '\n' && !input.eof())
					input.get(c);
				textAddress += 8;
			}
			else if (instruction == "subi" || instruction == "move")
			{
				char c;
				input.get(c);
				while (c != '\n' && !input.eof())
					input.get(c);
				textAddress += 4;
			}
			else
			{
				cout << "Parsing error!\n";
				TERMINATE = true;
			}
		}
		input >> instruction;
	}
	input.close();
}

void AssemblyParser::parseText(const string & file, vector<Instruction> & inst)
{
	unsigned int textAddress = 0x00400000;
	ifstream input;
	input.open(file.c_str());
	ofstream output(textDumpFile.c_str());
	string str;
	input >> str;
	while (str != ".text" && !input.eof())
		input >> str;
	input >> str;
	char c;
	vector<char>characters(4);
	while ((!input.eof() || str == "syscall") && str != ".data")
	{
		if (str[0] == '#')
		{
			char c;
			input.get(c);
			while (c != '\n')
				input.get(c);
		}
		else if (str == ".globl")
			input >> str;
		else if (isInstruction(str))
		{
			string instruction = "";
			instruction += str;
			if (str != "beq" && str != "bne" && str != "j" && str != "jal")
			{
				input.get(c);
				while (c != '\n' && c != '#' && !input.eof())
				{
					instruction += c;
					input.get(c);
				}
				while (c != '\n' && !input.eof())
					input.get(c);
			}
			else if (str == "j" || str == "jal")
			{
				input.get(c);
				while (!isdigit(c) && !isalpha(c))
				{
					instruction += c;
					input.get(c);
				}
				if (isalpha(c))
				{
					string label = "";
					while (c != '\n' && c != '#' && c != ' ' && !input.eof())
					{
						label += c;
						input.get(c);
					}
					while (c != '\n' && !input.eof())
						input.get(c);
					instruction += to_string(labels[label]);
				}
				else
				{
					while (c != '\n' && c != '#' && !input.eof())
					{
						instruction += c;
						input.get(c);
					}
					while (c != '\n' && !input.eof())
						input.get(c);
				}
			}
			else 
			{
				input.get(c);
				while (c != ',')
				{
					instruction += c;
					input.get(c);
				}
				instruction += c;
				input.get(c);
				while (c != ',')
				{
					instruction += c;
					input.get(c);
				}
				instruction += c;
				while (!isalpha(c) && !isdigit(c))
					input.get(c);
				if (isdigit(c))
				{
					string n = "";
					while (c != ' ' && c != '#' && c != '\n' && !input.eof())
					{
						n += c;
						input.get(c);
					}
					int num;
					if (n.length() > 2)
					{
						if (n[1] == 'x')
							num = wordHexaToDecimal(n.substr(2));
						else
							num = atoi(n.c_str());
					}
					else
						num = atoi(n.c_str());
					num = (num - (textAddress + 4)) / 4;
					instruction += to_string(num);
				}
				else
				{
					string label = "";
					while (c != ' ' && c != '#' && c != '\n' && !input.eof())
					{
						label += c;
						input.get(c);
					}
					int num = (labels[label] - (textAddress + 4)) / 4;
					instruction += to_string(num);
				}
			}
			textAddress += 4;
			Instruction instr;
			instr.assemble(instruction);
			inst.push_back(instr);
			characters = toChar(instr.getBinaryInstruction());
			for (int i = 3; i >= 0; i--)
				output << characters[i];
		}
		else if (str[str.length() - 1] == ':')
		{
			input >> str;
			continue;
		}
		else
		{
			if (str == "li" || str == "la")
			{
				string instruction = "lui $1, ";
				string reg = "", immediate = "";
				int imm;
				input.get(c);
				while (c != '$')
					input.get(c);
				while (c != ' ' && c != ',')
				{
					reg += c;
					input.get(c);
				}
				while (!isdigit(c) && !isalpha(c))
					input.get(c);
				if (isdigit(c))
				{
					while (c != ' ' && c != '#' && c != '\n' && !input.eof())
					{
						immediate += c;
						input.get(c);
					}
					if (immediate.length() > 2)
					{
						if (immediate[1] == 'x')
							imm = wordHexaToDecimal(immediate.substr(2));
						else
							imm = atoi(immediate.c_str());
					}
					else
						imm = atoi(immediate.c_str());
					int imm2;
					imm2 = (imm & 0x80000000) ? ((imm >> 16) | 0xffff0000) : ((imm >> 16) & 0x0000ffff);
					instruction += to_string(imm2);
					Instruction instr;
					instr.assemble(instruction);
					inst.push_back(instr);
					characters = toChar(instr.getBinaryInstruction());
					for (int i = 3; i >= 0; i--)
						output << characters[i];
					string instruction2 = "ori ";
					instruction2 += reg;
					instruction2 += ", $1, ";
					instruction2 += to_string(imm & 0x0000ffff);
					Instruction instr2;
					instr2.assemble(instruction2);
					inst.push_back(instr2);
					characters = toChar(instr2.getBinaryInstruction());
					for (int i = 3; i >= 0; i--)
						output << characters[i];
					textAddress += 8;
				}
				else
				{
					string label = "";
					while (c != ' ' && c != '#' && c != '\n' && !input.eof())
					{
						label += c;
						input.get(c);
					}
					imm = labels[label];
					int imm2;
					imm2 = (imm & 0x80000000) ? ((imm >> 16) | 0xffff0000) : ((imm >> 16) & 0x0000ffff);
					instruction += to_string(imm2);
					Instruction instr;
					instr.assemble(instruction);
					inst.push_back(instr);
					characters = toChar(instr.getBinaryInstruction());
					for (int i = 3; i >= 0; i--)
						output << characters[i];
					string instruction2 = "ori ";
					instruction2 += reg;
					instruction2 += ", $1, ";
					instruction2 += to_string(imm & 0x0000ffff);
					Instruction instr2;
					instr2.assemble(instruction2);
					inst.push_back(instr2);
					characters = toChar(instr2.getBinaryInstruction());
					for (int i = 3; i >= 0; i--)
						output << characters[i];
					textAddress += 8;
				}
			}
			else if (str == "move")
			{
				string reg1 = "", reg2 = "";
				input.get(c);
				while (c != '$')
					input.get(c);
				while (c != ',' && c != ' ')
				{
					reg1 += c;
					input.get(c);
				}
				while (c != '$')
					input.get(c);
				while (c != ' ' && c != '#' && c != '\n' && !input.eof())
				{
					reg2 += c;
					input.get(c);
				}
				string instruction = "addu ";
				instruction += reg1;
				instruction += ", $0, ";
				instruction += reg2;
				Instruction instr;
				instr.assemble(instruction);
				inst.push_back(instr);
				characters = toChar(instr.getBinaryInstruction());
				for (int i = 3; i >= 0; i--)
					output << characters[i];
				textAddress += 4;
			}
			else if (str == "subi")
			{
				string reg1 = "", reg2 = "", imm = "";
				input.get(c);
				while (c != '$')
					input.get(c);
				while (c != ' ' && c != ',')
				{
					reg1 += c;
					input.get(c);
				}
				while (c != '$')
					input.get(c);
				while (c != ' ' && c != ',')
				{
					reg2 += c;
					input.get(c);
				}
				while (!isdigit(c))
					input.get(c);
				while (c != ' ' && c != '#' && c != '\n' && !input.eof())
				{
					imm += c;
					input.get(c);
				}
				string instruction1 = "addi $1, $0, ";
				instruction1 += imm;
				Instruction instr1;
				instr1.assemble(instruction1);
				inst.push_back(instr1);
				characters = toChar(instr1.getBinaryInstruction());
				for (int i = 3; i >= 0; i--)
					output << characters[i];
				string instruction2 = "sub ";
				instruction2 += reg1;
				instruction2 += ", ";
				instruction2 += reg2;
				instruction2 += ", $1";
				Instruction instr2;
				instr2.assemble(instruction2);
				inst.push_back(instr2);
				characters = toChar(instr2.getBinaryInstruction());
				for (int i = 3; i >= 0; i--)
					output << characters[i];
				textAddress += 4;
			}
			while (c != '\n' && !input.eof())
				input.get(c);
		}
		if (input.eof())
			break;
		input >> str;
	}
	output.close();
	input.close();
}

bool AssemblyParser::isInstruction(const string & instruction)
{
	return (instruction == "j" || instruction == "jal" || instruction == "add" || instruction == "addu" || instruction == "sub" || instruction == "and" || instruction == "or" || instruction == "xor" || instruction == "slt" || instruction == "jr" || instruction == "sll" || instruction == "srl" || instruction == "syscall" || instruction == "lui" || instruction == "beq" || instruction == "bne" || instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh" || instruction == "addi" || instruction == "addiu" || instruction == "andi" || instruction == "ori" || instruction == "xori");
}

void AssemblyParser::parseData(const string & file, Memory & memory)
{
	string directive;
	ifstream input;
	input.open(file.c_str());
	while (directive != ".data" && !input.eof())
		input >> directive;
	if (directive == ".data")
	{
		input >> directive;
		while (directive != ".text" && !input.eof())
		{	
			if (directive[0] == '#')
			{
				char c;
				input.get(c);
				while (c != '\n')
					input.get(c);
			}
			else if (directive == ".word")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof() && c != '-')
					input.get(c);
				while (c != '\n' && !input.eof())
				{
					if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
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
							while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
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
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof() && c != '-')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n' && !input.eof())
							input.get(c);
						break;
					}
				}
			}
			else if (directive == ".half")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof() && c != '-')
					input.get(c);
				while (c != '\n' && !input.eof())
				{
					if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
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
							while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
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
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof() && c != '-')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n' && !input.eof())
							input.get(c);
						break;
					}
				}
			}
			else if (directive == ".byte")
			{
				char c;
				input.get(c);
				while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof() && c != '-')
					input.get(c);
				while (c != '\n' && !input.eof())
				{
					if (isdigit(c) || isalpha(c) || c == '\'' || c != '-')
					{
						string byte = "";
						if (c == '\'')
						{
							input.get(c);
							memory.storeByte(c);
							input.get(c);
							while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
								input.get(c);
						}
						else
						{
							while (c != ' ' && c != ',' && c != '#' && c != '\n' && !input.eof())
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
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof() && c != '-')
							input.get(c);
					}
					else if (c == '#' || c == '\n')
					{
						while (c != '\n' && !input.eof())
							input.get(c);
						break;
					}
				}
			}
			else if (directive == ".space")
			{
				char c;
				input.get(c);
				while (!isdigit(c) && c != '#' && c != '\n' && c != '\'' && !input.eof())
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
					while (c != '#' && c != '\n' && !input.eof())
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
				while (c != '\n' && !input.eof())
					input.get(c);
			}
			else if (directive == ".ascii")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n' && !input.eof())
				{
					while (c != '"' && c != '#' && c != '\n' && !input.eof())
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
				while (c != '\n' && !input.eof())
					input.get(c);
			}
			else if (directive == ".asciiz")
			{
				char c;
				input.get(c);
				while (c != '#' && c != '\n' && !input.eof())
				{
					while (c != '"' && c != '#' && c != '\n' && !input.eof())
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
				while (c != '\n' && !input.eof())
					input.get(c);
			}
			input >> directive;
		}
	}
	input.close();
	memory.memoryDump();
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

vector<char> AssemblyParser::toChar(const string & s)
{
	string chars[4];
	vector<char>characters(4);
	for (int i = 0; i < 4; i++)
		chars[i] = s.substr(i * 8, 8);
	for (int i = 0; i < 4; i++)
	{
		unsigned int ans = 0;
		for (int j = 0; j < 8; j++)
			ans = (ans << 1) + (chars[i][j] - '0');
		characters[i] = char(ans);
	}
	return characters;
}