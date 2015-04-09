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
    //set vector label to the size of the instructions
    labels.resize(instfile.size());
    
    int counter =0; //used in assigning numbers to the labels
    
    for (int i=0; i < instfile.size();i++)
    {
        address =  (instfile[i]& 0x3FFFFFF) << 2; // extracting the address
        jumpIndex= (address-0x00400000)/4; //the index of the label
        branchIndex = i + 1 + getSImm(i);
        
        //if j or jal, store the labels based on it's address
        if ( (instfile[i] >> 26) == 2 || (instfile[i] >> 26) == 3 )
        {
            //check if the inserted label wasn't inserted by the branch instruction, to avoid duplication
            if (labels[jumpIndex] == "")
                labels[jumpIndex]= "label_"+ to_string(counter++) + ": ";
        }
        else
            if ( (instfile[i] >> 26) == 0x04 || (instfile[i] >> 26) == 0x05 )
                labels[branchIndex] = "label_"+ to_string(counter++) + ": ";
        
        
    }
    
    for (int i = 0 ; i < instfile.size() ; i++) {
        isLui   = (instfile[i+1] == 0x0F);
        isOri   = (instfile[i+1] >>26 == 0x0D);
        isAddi  = (instfile[i] >> 26 == 0x08);
        isSub   = (instfile[i+1] >>26 == 0);
        temp_rd = (instfile[i+1] >> 11) & 0x1f;
        temp_rt = (instfile[i+1] >> 21) & 0x1f;
        
        printLi=(isLui && isOri);
        printSubi=(isAddi && isSub);
        
        
        x.dissassemble(instfile[i],temp_rd,temp_rt,printSubi,printLi,printLa); // calling the disassebling process
        inst.push_back(x);
        
        //Printing labels
        if (labels[i] !="" )
            cout <<endl<<labels[i] << "          " << endl;
        
        if (x.getAssemblyInstruction() !="")
        {
            // printing j and jal with label
            if (x.getOpcode() == 2 || x.getOpcode() == 3)
            {
                jumpIndex = ( (x.getAddress()<<2) - 0x00400000) / 4;
                long len =labels[jumpIndex].length()-1;
                string jString =labels[jumpIndex];
                cout << "          " << x.getAssemblyInstruction() << jString.erase(abs(len - 1)) <<endl << endl; //erase is used to erase the ":"
            }
            else
                if ( x.getOpcode() == 0x04 || x.getOpcode() == 0x05 )
                {
                    //printing beq and bne with label
                    branchIndex = i + 1+ getSImm(i);
                    long len1 = labels[branchIndex].length()-1;
                    string brString = labels[branchIndex];
                    cout << "          " << x.getAssemblyInstruction() << brString.erase(len1 - 1) << endl;
                }
                else
                    cout <<"          "<< x.getAssemblyInstruction() << endl;
        }
    }
}


int disassembler:: getSImm(int i) const
{
    
    int imm = (instfile[i] & 0xFFFF);
    
    return ((imm & 0x8000) ? (0xFFFF0000 | imm) : imm);
}
