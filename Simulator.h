#include "CPU.h"
#include "Memory.h"
#include "Instruction.h"
#include "GLOBALS.H"

#include <iostream>
#include <vector>
#include <fstream>

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator
{
public:
    CPU cpu;
    Memory memory;
    string registerName[32];
    ofstream outfile;
    void initializeName();
    vector <Instruction> program;
    Simulator();
    ~Simulator();
    void add(Instruction*);
    void addi(Instruction*);
    void addiu(Instruction*);
    void addu(Instruction*);
    void sub(Instruction*);
    void andi(Instruction*);
    void oring(Instruction*);
    void sll(Instruction*);
    void srl(Instruction*);
    void anding(Instruction*);
    void ori(Instruction*);
    void xori(Instruction*);
    void xoring(Instruction*);
    void lw(Instruction*);
    void sw(Instruction*);
    void sb(Instruction*);
    void lb(Instruction*);
    void lh(Instruction*);
    void sh(Instruction*);
    void beq(Instruction*);
    void bne(Instruction*);
    void slt(Instruction*);
    void slti(Instruction*);
    void j(Instruction*);
    void jr(Instruction*);
    void jal(Instruction*);
    void lui(Instruction*);
    void syscall(Instruction*);
	void run(Instruction *);
    void simulate();
	void displayRegister(string);
};

#endif
