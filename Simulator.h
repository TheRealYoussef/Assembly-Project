#ifndef __Project_Assembly_1__Simulator__
#define __Project_Assembly_1__Simulator__

#include <iostream>
#include "CPU.h"
#include "Memory.h"
#include "Instruction.h"

class Simulator
{
    Memory memory;
    CPU cpu;
public:
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
};

#endif /* defined(__Project_Assembly_1__Simulator__) */
