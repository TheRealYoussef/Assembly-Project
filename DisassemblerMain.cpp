
#include <iostream>
#include "Disassembler.h"
#include "Simulator.h"
int main(int argc, const char * argv[]) {
    
    vector<Instruction> sim;
       Simulator simulator;
       simulator.memory.MemoryArray("Memory Dump text");
        Disassembler disassembler(" Assembly Code dump");
        disassembler.display();
        disassembler.simulatorData(sim); // copy instruction vector
        simulator.program = sim;
                       
    simulator.simulate();
    simulator.displayRegister("/Users/bahermursi/Desktop/proj231/proj231/simulate");
    
    
    return 0;
}
