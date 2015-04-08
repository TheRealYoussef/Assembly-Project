#include "Instruction.h"
#include "simulator.h"
#include <string>
#include <stack>
#include <cmath>
#include <iostream>

using namespace std;
Simulator simulator; 

Instruction::Instruction()
{
}

void Instruction::assemble(string assemblyInstruction)
{
    this->assemblyInstruction = assemblyInstruction;
    string instruction = "";
    int idx = 0;
    while (!isalpha(assemblyInstruction[idx]))
        idx++;
    while (assemblyInstruction[idx] != ' ')
    {
        instruction += tolower(assemblyInstruction[idx]);
        idx++;
    }
   binaryInstruction = "";
	if (instruction == "j" || instruction == "jal")
		setJFormat(instruction, idx);
	else if (instruction == "add" || instruction == "addu" || instruction == "sub" || instruction == "and" || instruction == "or" || instruction == "xor" || instruction == "slt" || instruction == "jr" || instruction == "sll" || instruction == "srl" || instruction == "syscall")
		setRFormat(instruction, idx);
	else if (instruction == "lui" || instruction == "beq" || instruction == "bne" || instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh" || instruction == "addi" || instruction == "addiu" || instruction == "andi" || instruction == "ori" || instruction == "xori")
		setIFormat(instruction, idx);
}

void Instruction::dissassemble(unsigned int binaryInstruction, int& trd, int& trt, bool& subi, bool& li, bool& la)
{
    opcode = binaryInstruction >> 26;
    if (opcode == 0)
    {
        func = binaryInstruction & 0x3F;
        shamt = (binaryInstruction >> 6) & 0x1f;
        rd = (binaryInstruction >> 11) & 0x1f;
        rt = (binaryInstruction >> 16) & 0x1f;
        rs = (binaryInstruction >> 21) & 0x1f;
    }
    else if (opcode != 0 && opcode != 2 && opcode != 3 && opcode != 16 && opcode != 17 && opcode != 18 && opcode != 19)
    {
        rt = (binaryInstruction >> 16) & 0x1f;
        rs = (binaryInstruction >> 21) & 0x1f;
        imm = (binaryInstruction & 0x0000FFFF);
        signedImm = (imm & 0x8000) ? (0xFFFF0000 | imm) : imm;
    }
    else if(opcode == 2 || opcode == 3)
    {
        address = (binaryInstruction & 0x3FFFFFF);
    }
     int temp_rd=trd;
    int temp_rt=trt;
    bool psubi=subi, pLa=la,pLi=li;
    setAssembleyInstruction(temp_rd ,temp_rt, psubi,pLi ,pLa);
    checkSubi = subi;
}


void Instruction::setAssembleyInstruction(int tempd, int tempt,bool subi,bool li,bool la)
{
    
    if (opcode == 0)
    {
        switch (func)
        {
            case 0x08:
                assemblyInstruction = "jr " + registerToName(rs);
                break;
            case 0x20:
                if (rt == 0) {
                    assemblyInstruction = "move " + registerToName(rd) + ", " + registerToName(rs);
                }
                else
                    assemblyInstruction = "add " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x21:
                assemblyInstruction = "addu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x22:
                //Re-test this part
                if (checkSubi == 1)
                    assemblyInstruction = "subi " + registerToName(rd) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                else
                    assemblyInstruction = "sub " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                
                break;
            case 0x23:
                assemblyInstruction = "subu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x2A:
                assemblyInstruction = "slt " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x2B:
                assemblyInstruction = "sltu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x00:
                assemblyInstruction = "sll " + registerToName(rd) + ", " + registerToName(rt) + ", " + to_string(shamt);
                break;
            case 0x02:
                assemblyInstruction = "srl " + registerToName(rd) + ", " + registerToName(rt) + ", " + to_string(shamt);
                break;
            case 0x03:
                assemblyInstruction = "sra " + registerToName(rd) + ", " + registerToName(rt) + ", " + to_string(shamt);
                break;
            case 0x24:
                assemblyInstruction = "and " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x25:
                assemblyInstruction = "or " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x26:
                assemblyInstruction = "xor " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x27:
                assemblyInstruction = "nor " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x0C:
                assemblyInstruction = "syscall";
                break;
            default:
                assemblyInstruction = "Unknown R-Format Instruction";
        }
    }
    else if (opcode != 0 && opcode != 2 && opcode != 3 && opcode != 16 && opcode != 17 && opcode != 18 && opcode != 19)
    {
        switch (opcode)
        {
            case 0x08:
                if (rs == 0 && subi == false)
                    assemblyInstruction = "li " + registerToName(rt) + ", " + to_string(signedImm);
                else//CHeck on different test cases
                    if (subi == 1) {
                        assemblyInstruction ="";
                    }
                    else
                        assemblyInstruction = "addi " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x09:
                if (rs == 0)
                    assemblyInstruction = "li " + registerToName(rt) + ", " + to_string(signedImm);
                else
                    assemblyInstruction = "addiu " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x0C:
                assemblyInstruction = "andi " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                break;
            case 0x23:
                assemblyInstruction =  "lw " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x2B:
                assemblyInstruction = "sw " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x20:
                assemblyInstruction = "lb " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x28:
                assemblyInstruction = "sb " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x21:
                assemblyInstruction = "lh " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x29:
                assemblyInstruction = "sh " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x0A:
                assemblyInstruction = "slti " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x0B:
                assemblyInstruction = "sltiu " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x0D:
                pseudo = "ori " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                if (li == false && la == false)
                    assemblyInstruction = pseudo;
                else
                    if(li == true &&la == true)
                        assemblyInstruction = "li " + registerToName(rt) + ", " + to_string((imm<<16) + liImm);
                break;
            case 0x0E:
                assemblyInstruction = "xori " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                break;
            case 0x0F:
                pseudo = "lui " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                if (li == false && la == false)
                    assemblyInstruction = pseudo;
                else
                    liImm = imm;
                assemblyInstruction ="";
                break;
            case 0x04:
                assemblyInstruction = "beq " + registerToName(rs) + ", " + registerToName(rt) + ", " /*+ to_string(signedImm)*/;
                break;
            case 0x05:
                assemblyInstruction = "bne " + registerToName(rs) + ", " + registerToName(rt) + ", "/*+ to_string(signedImm)*/;
                break;
            default:
                assemblyInstruction = "Unknown I-Format Instruction";
                break;
        }
    }
    else if (opcode == 2 || opcode == 3)
    {
        switch (opcode)
        {
            case 0x02:
                assemblyInstruction = "j " ;
                break;
            case 0x03:
                assemblyInstruction = "jal " ;
                break;
            default:
                assemblyInstruction = "Unknown J-Format Instruction" ;
                break;
        }
    }
}


string Instruction::registerToName(int reg) const
{
    string name;
    switch (reg) {
        case 0:
            name = "$Zero";
            break;
        case 1:
            name = "$at";
        case 2:case 3:
            name = "$v" + to_string(abs(reg - 2));
            break;
        case 4: case 5: case 6: case 7:
            name = "$a" + to_string(reg - 4);
            break;
        case 8:case 9:case 10:  case 11: case 12:case 13:case 14:case 15:
            name = "$t" + to_string(reg - 8);
            break;
        case 16:case 17:case 18: case 19: case 20: case 21:case 22:case 23:
            name = "$s" + to_string(reg - 16);
            break;
        case 24: case 25:
            name = "$t" + to_string(reg - 16);
            break;
        case 26:case 27:
            name = "$k" + to_string(reg - 26);
            break;
        case 28:
            name = "$gp";
            break;
        case 29:
            name = "$sp";
            break;
        case 30:
            name = "$fp";
            break;
        case 31:
            name = "$ra";
            break;
            
    }
    return name;
}

string Instruction::toBinary(int n, int size, bool sign) const
{
    n = abs(n);
    stack<int>s;
    string str = "";
    while (n != 0)
    {
        s.push(n % 2);
        n /= 2;
    }
    while (s.size() < size)
        s.push(0);
    while (!s.empty())
    {
        str += (s.top() + '0');
        s.pop();
    }
    if (!sign)
        str = twosCompliment(str, size);
    return str;
}

string Instruction::twosCompliment(string binary, int size) const
{
    string answer = "";
    for (int i = 0; i < size; i++)
        answer += '0';
    for (int i = 0; i < binary.length(); i++)
        binary[i] = (binary[i] == '0') ? '1' : '0';
    char carry = (binary[binary.length() - 1] == '1') ? '1' : '0';
    answer[answer.length() - 1] = (binary[binary.length() - 1] == '1') ? '0' : '1';
    for (int i = binary.length() - 2; i >= 0; i--)
    {
        if ((carry == '1' && binary[i] == '0') || (carry == '0' && binary[i] == '1'))
        {
            answer[i] = '1';
            carry = '0';
        }
        else if (carry == '1' && binary[i] == '1')
            carry = '1';
    }
    return answer;
}

int Instruction::convertToRegisterName(string name) const
{
    if (name == "$zero" || name == "$0")
        return 0;
    else if (name == "$at" || name == "$1")
        return 1;
    else if (name == "$v0" || name == "$2")
        return 2;
    else if (name == "$v1" || name == "$3")
        return 3;
    else if (name == "$a0" || name == "$4")
        return 4;
    else if (name == "$a1" || name == "$5")
        return 5;
    else if (name == "$a2" || name == "$6")
        return 6;
    else if (name == "$a3" || name == "$7")
        return 7;
    else if (name == "$t0" || name == "$8")
        return 8;
    else if (name == "$t1" || name == "$9")
        return 9;
    else if (name == "$t2" || name == "$10")
        return 10;
    else if (name == "$t3" || name == "$11")
        return 11;
    else if (name == "$t4" || name == "$12")
        return 12;
    else if (name == "$t5" || name == "$13")
        return 13;
    else if (name == "$t6" || name == "$14")
        return 14;
    else if (name == "$t7" || name == "$15")
        return 15;
    else if (name == "$s0" || name == "$16")
        return 16;
    else if (name == "$s1" || name == "$17")
        return 17;
    else if (name == "$s2" || name == "$18")
        return 18;
    else if (name == "$s3" || name == "$19")
        return 19;
    else if (name == "$s4" || name == "$20")
        return 20;
    else if (name == "$s5" || name == "$21")
        return 21;
    else if (name == "$s6" || name == "$22")
        return 22;
    else if (name == "$s7" || name == "$23")
        return 23;
    else if (name == "$t8" || name == "$24")
        return 24;
    else if (name == "$t9" || name == "$25")
        return 25;
    else if (name == "$k0" || name == "$26")
        return 26;
    else if (name == "$k1" || name == "$27")
        return 27;
    else if (name == "$gp" || name == "$28")
        return 28;
    else if (name == "$sp" || name == "$29")
        return 29;
    else if (name == "$fp" || name == "$30")
        return 30;
    else if (name == "$ra" || name == "$31")
        return 31;
    else
        return -1;
}

void Instruction::setJFormat(string instruction, int & idx)
{
    format = J_FORMAT;
    opcode = (instruction == "j") ? 2 : 3;
    while (!isdigit(assemblyInstruction[idx]))
        idx++;
    string temp = "";
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    address = hexaToDecimal(temp, 8);
    binaryInstruction += toBinary(opcode, 6, true);
    binaryInstruction += toBinary((address & 268435455) >> 2, 26, true);
}

void Instruction::setRFormat(string instruction, int & idx)
{
    format = R_FORMAT;
    opcode = 0;
    binaryInstruction += toBinary(opcode, 6, true);
    rs = 0;
    rt = 0;
    rd = 0;
    shamt = 0;
    if (instruction == "syscall")
        setSyscall();
    else if (instruction == "sll" || instruction == "srl")
        setShiftInstruction(instruction, idx);
    else if (instruction == "jr")
        setJrInstruction(instruction, idx);
    else
        setNormalRFormat(instruction, idx);
    binaryInstruction += toBinary(rs, 5, true);
    binaryInstruction += toBinary(rt, 5, true);
    binaryInstruction += toBinary(rd, 5, true);
    binaryInstruction += toBinary(shamt, 5, true);
    binaryInstruction += toBinary(func, 6, true);
}

void Instruction::setSyscall()
{
    func = 12;
}

void Instruction::setShiftInstruction(string instruction, int & idx)
{
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rd = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]))
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    if ((shamt = hexaToDecimal(temp, 2)) > 31)
		TERMINATE = true;
	if (instruction == "sll")
		func = 0;
	else
		func = 2;
}

void Instruction::setJrInstruction(string instruction, int & idx)
{
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    rs = convertToRegisterName(temp);
    func = 8;
}

void Instruction::setNormalRFormat(string instruction, int & idx)
{
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rd = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    if (instruction == "add")
        func = 32;
    else if (instruction == "addu")
        func = 33;
    else if (instruction == "sub")
        func = 34;
    else if (instruction == "and")
        func = 36;
    else if (instruction == "or")
        func = 37;
    else if (instruction == "xor")
        func = 38;
    else if (instruction == "slt")
        func = 42;
}

void Instruction::setIFormat(string instruction, int & idx)
{
    format = I_FORMAT;
    rs = 0;
    rt = 0;
    imm = 0;
    if (instruction == "lui")
        setLui(idx);
    else if (instruction == "beq" || instruction == "bne")
        setBranchInstruction(instruction, idx);
    else if (instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh")
        setMemoryInstruction(instruction, idx);
    else
        setNormalIFormat(instruction, idx);
    binaryInstruction += toBinary(opcode, 6, true);
    binaryInstruction += toBinary(rs, 5, true);
    binaryInstruction += toBinary(rt, 5, true);
    binaryInstruction += (imm < 0) ? toBinary(imm, 16, false) : toBinary(imm, 16, true);
}

void Instruction::setLui(int & idx)
{
    opcode = 15;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    signedImm = hexaToDecimal(temp, 4);
	imm = signedImm & 0x0000ffff;
}

void Instruction::setBranchInstruction(string instruction, int & idx)
{
    opcode = (instruction == "beq") ? 4 : 5;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
	signedImm = hexaToDecimal(temp, 4);
	imm = signedImm & 0x0000ffff;
}

void Instruction::setMemoryInstruction(string instruction, int & idx)
{
    opcode = (instruction == "lw") ? 35 : (instruction == "sw") ? 43 : (instruction == "lh") ? 33 : (instruction == "sh") ? 41 : (instruction == "lb") ? 32 : 40;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != '(' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
	signedImm = hexaToDecimal(temp, 4);
	imm = signedImm & 0x0000ffff;
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ')' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
}

void Instruction::setNormalIFormat(string instruction, int & idx)
{
    opcode = (instruction == "addi") ? 8 : (instruction == "addiu") ? 9 : (instruction == "andi") ? 12 : (instruction == "ori") ? 13 : (instruction == "xori") ? 14 : 10;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\n' && idx < assemblyInstruction.length())
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
	signedImm = atoi(temp.c_str());
	imm = signedImm & 0x0000ffff;
}

Instruction::InstructionFormat Instruction::getFormat() const
{
    return format;
}

int Instruction::getOpcode() const
{
    return opcode;
}

int Instruction::getRs() const
{
    return rs;
}

int Instruction::getRt() const
{
    return rt;
}

int Instruction::getRd() const
{
    return rd;
}

int Instruction::getShamt() const
{
    return shamt;
}

int Instruction::getFunc() const
{
    return func;
}

int Instruction::getImm() const
{
    return imm;
}

int Instruction::getSignedImm() const
{
    return signedImm;
}

int Instruction::getAddress() const
{
    return address;
}

string Instruction::getAssemblyInstruction() const
{
    return assemblyInstruction;
}

string Instruction::getBinaryInstruction() const
{
    return binaryInstruction;
}

void Instruction::run()
{
    if (opcode == 0)
    {
        switch (func)
        {
            case 0x08:
                simulator.jr(this);
                break;
            case 0x20:
                simulator.add(this);
                break;
            case 0x21:
                simulator.addu(this);
                break;
            case 0x22:
                simulator.sub(this);
                break;
            case 0x2A:
                simulator.slt(this);
                break;
            case 0x00:
                simulator.sll(this);
                break;
            case 0x02:
                simulator.srl(this);
                break;
            case 0x24:
                simulator.anding(this);
                break;
            case 0x25:
                simulator.oring(this);
                break;
            case 0x26:
                simulator.xoring(this);
                break;
            case 0x0C:
                simulator.syscall(this);
                break;
            default:
                cout<<"error";
        }
    }
    else if (opcode != 0 && opcode != 2 && opcode != 3)
    {
        switch (opcode)
        {
            case 0x08:
                simulator.addi(this);
                break;
            case 0x09:
                simulator.addiu(this);
                break;
            case 0x0C:
                simulator.andi(this);
                break;
            case 0x23:
                simulator.lw(this);
                break;
            case 0x2B:
                simulator.sw(this);
                break;
            case 0x20:
                simulator.lb(this);
                break;
            case 0x28:
                simulator.sb(this);
                break;
            case 0x21:
                simulator.lh(this);
                break;
            case 0x29:
                simulator.sh(this);
                break;
            case 0x0A:
                simulator.slti(this);
                break;
            case 0x0D:
                simulator.ori(this);
                break;
            case 0x0E:
                simulator.xori(this);
                break;
            case 0x0F:
                simulator.lui(this);
                break;
            case 0x04:
                simulator.beq(this);
                break;
            case 0x05:
                simulator.bne(this);
                break;
            default:
                  cout<<"error";
                break;
        }
    }
    else if (opcode == 2 || opcode == 3)
    {
        switch (opcode) 
        {
            case 0x02:
                simulator.j(this);
                break;
            case 0x03:
                simulator.jal(this);
                break;
            default:
                cout<<"error";
                break;
        }
    }
                
}

int Instruction::hexaToDecimal(const string & hexa, int n)
{
	if (hexa.length() > 2)
	{
		if (hexa[1] == 'x')
		{
			int ans = 0;
			int i = hexa.length() - 1;
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
		else
			return atoi(hexa.c_str());
	}
	else
		return atoi(hexa.c_str());
}