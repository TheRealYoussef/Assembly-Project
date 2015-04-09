#include "Disassembler.h"
#include <iostream>
using namespace std;

disassembler::disassembler(string x){
    inFileName =x;
    getData();
    display();
    
}

void disassembler::getData(){
    inFile.open(inFileName.c_str());
    
    // If the file is open (this will evaluate to false if the file could not be found)
    if(inFile.is_open())
    {
        
        while(!inFile.eof())
        {
            instWord = 0;
            // Decode the instruction read from the file
            for (int i = 0; i < 4; i++)
            {
                if (!inFile.eof())
                {
                    inFile.get(c);
                    unsigned int temp = (unsigned int)((unsigned char)c);
                    instWord = instWord | (temp << (8 * i));
                }
                else
                    break;
            }
            if (!inFile.eof())
            {
                // instfile vector that stores all the binary instructions
                instfile.push_back(instWord);
                
                
            }
        }
        
    }
    
    
}
void disassembler::display(){
    labels.resize(instfile.size());
    
    for (int i=0; i<instfile.size();i++) {
        address = (instfile[i]& 0x3FFFFFF) <<2;
        offset= (address-0x00400000)/4;
        
        if ((instfile[i]>>26 == 2) || (instfile[i]>>26 == 3))
            labels[offset]= "label_"+ to_string(char(offset)) +": ";
        
    }
    
    for (int i=0; i<instfile.size(); i++) {
        isLui = (instfile[i]>>26 == 0x0F);
        isOri = (instfile[i+1]>>26 == 0x0D);
        isAddi = (instfile[i]>> 26 == 0x08);
        
        isSub = (instfile[i+1]>>26 == 0);
        temp_rd = (instfile[i+1]>> 11) & 0x1f;
        temp_rt = (instfile[i+1]>> 21) & 0x1f;
        
        printLi=(isLui && isOri);
        
        //  printLi ==1? brOffset+=2:brOffset;
        
        printSubi=(isAddi && isSub);
        
        // printSubi ==1? brOffset+=2:brOffset;
        
        
        
        x.dissassemble(instfile[i],temp_rd,temp_rt,printSubi,printLi,printLa); // calling the disassebling process
        inst.push_back(x);
        
        
        pc+=4;
        
        if (labels[i] !="" )
            cout <<endl<<labels[i] << endl << endl;
        
        if (x.getAssemblyInstruction() !=""){ //j and jal
            if (x.getOpcode() == 2 || x.getOpcode() == 3){
                offset= ((x.getAddress()<<2)-0x00400000)/4;
                long len1 =labels[offset].length()-1;
                string temps1 =labels[offset];
                cout <<endl<< x.getAssemblyInstruction() << temps1.erase(abs(len1-1)) <<endl << endl;
            }
            else
                if (x.getOpcode() == 0x04 || x.getOpcode() == 0x05) {//beq and bne
                    //   offset= (pc-0x00400000);
                    long len=labels[x.getSignedImm()+1].length()-1;
                    string temps = labels[x.getSignedImm()+1];
                    cout << x.getAssemblyInstruction() <<temps.erase(abs(len-1)) <<endl;
                }
                else
                    cout << x.getAssemblyInstruction() << endl;
        }
    }
}





