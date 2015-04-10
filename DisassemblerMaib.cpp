//
//  main.cpp
//  proj231
//
//  Created by Baher Mursi on 4/9/15.
//  Copyright (c) 2015 Baher Mursi. All rights reserved.
//

#include <iostream>
#include "Disassembler.h"
#include "Simulator.h"

int main(int argc, const char * argv[]) {
    
    vector<Instruction> sim;
    Simulator simulator;
    simulator.memory.MemoryArray("/Users/bahermursi/Desktop/Memory Dump.txt");
    Disassembler disassembler("/Users/bahermursi/Desktop/texttest.txt");
    disassembler.display();
    disassembler.simulatorData(sim);
    simulator.program = sim;
    simulator.simulate();
    cout << endl;
    
    
    return 0;
}
