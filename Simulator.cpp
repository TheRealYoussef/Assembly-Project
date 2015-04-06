#include "Simulator.h"

Simulator::Simulator(){}

Simulator:: ~Simulator(){}

void Simulator::add(Instruction* instruction)
{
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] + cpu.registers[instruction->getRt()];
    if((cpu.registers[instruction->getRs()] < 0 && cpu.registers[instruction->getRt()] < 0 && cpu.registers[instruction->getRd()] > 0 ) || (cpu.registers[instruction->getRs()] > 0 && cpu.registers[instruction->getRt()] > 0 && cpu.registers[instruction->getRd()] < 0 ))
    {
        cout << "Overflow!!" << endl;
    }
}

void Simulator::addu(Instruction* instruction)
{
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] + cpu.registers[instruction->getRt()];
}

void Simulator::addi(Instruction* instruction)
{
    // R[rt] = R[rs] + SignExtImm
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] + cpu.registers[instruction->getSignedImm()];
}

void Simulator:: addiu(Instruction* instruction)
{
    // change sign extension R[rt] = R[rs] + SignExtImm
    
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] + cpu.registers[instruction->getSignedImm()];
                      
}

void Simulator::sub(Instruction* instruction)
{
    //R[rd] = R[rs] - R[rt]
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] - cpu.registers[instruction->getRt()];
}

void Simulator:: oring(Instruction* instruction)
{
    //R[rd] = R[rs] | R[rt]
    cpu.registers[instruction->getRd()]= cpu.registers[instruction->getRs()] | cpu.registers[instruction->getRt()];
}

void Simulator:: ori(Instruction* instruction)
{
    // missing zero extension immediate  R[rd] = R[rs] | R[rt]
    cpu.registers[instruction->getRt()]= cpu.registers[instruction->getRs()] | cpu.registers[instruction->getImm()];
}
void Simulator:: sll(Instruction* instruction)
{
    //R[rd] = R[rt] << shamt
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRt()] << cpu.registers[instruction->getShamt()];
}

void Simulator:: srl(Instruction* instruction)
{
    //R[rd] = R[rt] >> shamt
    cpu.registers[instruction->getRd()] = (unsigned int)(cpu.registers[instruction->getRt()]) >> cpu.registers[instruction->getShamt()];
}

void Simulator:: anding(Instruction* instruction)
{
    //R[rd] = R[rs] & R[rt]
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] & cpu.registers[instruction->getRt()];
    
}
void Simulator:: andi(Instruction* instruction)
{
    //R[rt] = R[rs] & ZeroExtImm
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] & cpu.registers[instruction->getImm()];
    
}

void Simulator:: xori(Instruction* instruction)
{
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] ^ cpu.registers[instruction->getImm()];
}
void Simulator:: xoring(Instruction* instruction)
{
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] ^ cpu.registers[instruction->getRt()];
}

void Simulator:: lw(Instruction* instruction)
{
    //R[rt] = M[R[rs]+SignExtImm]
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    cpu.registers[instruction->getRt()]=memory.loadWord(address);
}

void Simulator:: sw(Instruction* instruction)
{
    //M[R[rs]+SignExtImm] = R[rt]
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    memory.storeWord(address,cpu.registers[instruction->getRt()]);
    
}
void Simulator::sb(Instruction* instruction)
{
    //M[R[rs]+SignExtImm](7:0) =R[rt](7:0)
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
   memory.storeByte(address, cpu.registers[instruction->getRt()]);
    
}
void Simulator::lb(Instruction* instruction)
{
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    cpu.registers[instruction->getRt()]=memory.loadByte(address);
}
void Simulator::lh(Instruction* instruction)
{
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    cpu.registers[instruction->getRt()]=memory.loadHalf(address);
}
void Simulator::sh(Instruction* instruction)
{
    //M[R[rs]+SignExtImm](15:0) =R[rt](15:0)
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    memory.storeHalf(address, cpu.registers[instruction->getRt()]);
}
void Simulator::beq(Instruction* instruction)
{
    //if(R[rs]==R[rt]) ,PC=PC+4+BranchAddr
    if(cpu.registers[instruction->getRs()]==cpu.registers[instruction->getRt()])
        cpu.programCounter=cpu.programCounter+instruction->getSignedImm();
    
}
void Simulator::bne(Instruction* instruction)
{
     //if(R[rs]!=R[rt]) ,PC=PC+4+BranchAddr
    if(cpu.registers[instruction->getRs()]!=cpu.registers[instruction->getRt()])
        cpu.programCounter=cpu.programCounter+instruction->getSignedImm();
    
}
void Simulator::slt(Instruction* instruction)
{
    //R[rd] = (R[rs] < R[rt]) ? 1 : 0
    if(cpu.registers[instruction->getRs()] < cpu.registers[instruction->getRt()])
        cpu.registers[instruction->getRd()]=1;
    else
        cpu.registers[instruction->getRd()]=0;
}
void Simulator::slti(Instruction* instruction)
{
    ////R[rd] = (R[rs] < SignExtImm) ? 1 : 0
    if(cpu.registers[instruction->getRs()] < cpu.registers[instruction->getSignedImm()])
        cpu.registers[instruction->getRd()]=1;
    else
        cpu.registers[instruction->getRd()]=0;
    
}
void Simulator::j(Instruction* instruction)
{
    //PC=JumpAddr
    cpu.programCounter = (cpu.programCounter & 0xf0000000)|(instruction->getAddress() << 2);
}
void Simulator::jr(Instruction* instruction)
{
    //PC=R[rs]
    cpu.programCounter=cpu.registers[instruction->getRs()];
    
}

void Simulator::jal(Instruction* instruction)
{
    //R[31]=PC+8;PC=JumpAddr
    
    cpu.registers[31]=cpu.programCounter;
    cpu.programCounter+=(cpu.programCounter & 0xf0000000)|(instruction->getAddress() << 2);
    
}

void Simulator::lui(Instruction* instruction)
{
    //R[rt] = {imm, 16’b0}
    cpu.registers[instruction->getRt()]=instruction->getImm()<<16;
    
    
}
void Simulator::syscall(Instruction* instruction)
{
    int address = cpu.registers[4];
    char t;
    
   switch (cpu.registers[2])
    {
    //print integer
        case 1:
        cout<< cpu.registers[4];
            break;
    // print character
        case 11:
        cout<<char(cpu.registers[4] & 0xff);
            break;
    
    //print string
        case 4:
    while((t = memory.loadByte(address) != 0))
        {
            cout << t;
            address++;
    
        }
            break;
            //terminate execution
        case 10:
        cout<< cpu.registers[4];
            break;
}
}
