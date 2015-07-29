#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl) {
        case 0:
            *ALUresult = A + B;
            break;
        case 1:
            *ALUresult = A - B;
            break;
        case 2:
            *ALUresult = ((int)A < (int)B) ? 1 : 0;
            break;
        case 3:
            *ALUresult = (A < B) ? 1 : 0;
            break;
        case 4:
            *ALUresult = A & B;
            break;
        case 5:
            *ALUresult = A | B;
            break;
        case 6:
            *ALUresult = B << 16;
            break;
        case 7:
            *ALUresult = ~A;
            break;
    }
    *Zero = (*ALUresult == 0) ? 1 : 0;
}



/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //fetch the instruction addressed by PC from Mem and write it to instruction
    //if PC is a multiple of 4 and is within memory
    if(PC % 4 == 0 && PC <= 0xFFFF)
    {
        *instruction = Mem[PC >> 2];
        return 0;
    }

    //halt condition occured - counter was not a multiple of 4 or was out of range
    else 
        return 1;
}



/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // May need to cast the hex values to unsigned, but I don't think that should make any difference
    *op = (instruction & 0xfc000000) >> 26;
    *r1 = (instruction & 0x3e00000) >> 21;
    *r2 = (instruction & 0x1f0000) >> 16;
    *r3 = (instruction & 0xf800) >> 11;
    *funct = instruction & 0x3f;
    *offset = instruction & 0xffff;
    *jsec = instruction & 0x3ffffff;
}



/* instruction decode */
/* 15 Points */
//
int instruction_decode(unsigned op,struct_controls *controls)
{
    switch(op) {
        case 0: // r-types add, sub, and, or, slt, sltu
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            return 0;
        case 2: // j
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 4: // beq
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            return 0;
        case 8: // addi
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        case 10: // slti
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        case 11: // sltiu
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        case 15: // lui
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        case 35: // lw
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        case 43: // sw
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            return 0;
        default: // Bad Instruction
            return 1;
    }
}



/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    //assign value of register r1 to data1 and r2 to data2
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}



/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //signbit is the most signifigant bit
    unsigned SignBit = offset >> 15;

    //if negative, first 16 bits will be made 1s
    if (SignBit == 1)
        *extended_value = 0xFFFF0000 | offset;
    
    //if postive, first 16 bits will be made 0s
    else 
        *extended_value = 0x0000FFFF & offset;
}



/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    if (ALUSrc == 1) 
        data2 = extended_value;
    if (ALUOp == 7) {
        // add, sub, and, or, slt, sltu, left shitf
        switch(funct) {
            case 4: // sllv (not sure if this is necessary for the 16 bit left shift)
                ALUOp = 6;
                break;
            case 32: //add
                ALUOp = 0;
                break;
            case 34: //sub
                ALUOp = 1;
                break;
            case 36: //and
                ALUOp = 4;
                break;
            case 37: //or
                ALUOp = 5;
                break;
            case 39: //not
                ALUOp = 7;
                break;
            case 42: //slt
                ALUOp = 2;
                break;
            case 43: //sltu
                ALUOp = 3;
                break;
            default: //not a proper function
                return 1;
        }
    }
    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    //read the content of the memory location addressed by ALUresult to memdata
    if(MemRead == 1)
    {
        //check for word alignment
        if(ALUresult % 4 != 0)
            return 1;
        else
            *memdata = Mem[ALUresult >> 2];
    }

    //write the value of data2 to the memory location addressed by ALUresult
    if(MemWrite == 1)
    {
        //check for word alignment
        if(ALUresult % 4 != 0)
            return 1;
        else
            Mem[ALUresult >> 2] = data2;
    }

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if (RegWrite == 1) {

        switch(MemtoReg) {
            case 0:
               //Value from ALUresult
               if (RegDst == 0) 
                    Reg[r2] = ALUresult;
               else 
                    Reg[r3] = ALUresult;
               break;

            case 1:
               //Value from memdata
                if (RegDst == 0) 
                    Reg[r2] = memdata;
               else 
                    Reg[r3] = memdata;
               break;
        }
    }
}


/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //increament by 4 everytime
    *PC += 4;

    //branch
    if(Branch == 1 && Zero == 1)
        *PC += (extended_value << 2);

    //jump
    if(Jump == 1)
        *PC = (*PC & 0xF0000000) | ((jsec << 2) & 0x0FFFFFFF);
}


