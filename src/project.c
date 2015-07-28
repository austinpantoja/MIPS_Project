#include "spimcore.h"


/** Change Log
 * 7/28 - Completed instruction_partition() by assigning the corresponding value for all of the parameters using bitmasking. -Austin
 * 7/28 - Completed ALU() function. -Austin
 */


/* ALU */
/* 10 Points */
/* Since I have used it below and you may not be familiar, the ternary operator (?:) in C (and Java)
 *      value = (condition) ? [if true] : [if false]
 *      This is basically and if-else statement used to set a particular value
 *
 * I am pretty sure this is right (at least mostly), but I am not sure how to test right now. This is where I think this could be wrong.
 *  > For cases 0 and 1 (add/sub) I am assuming the sign (positive/negative) logic is handled somewhere else
 *      I'm shakey on the material since the last test, if this is a problem we can cast A and B to signed for the operation
 *  > Currently, the value pointed to by Zero is set every time the function is called (last line of the function)
 *      I don't really like giving a value to *Zero and not *ALUresult (cases where ALUControl is <0, >7, or =6)
 *      Since ALUresult is a pointer there'll always be some value, and it could have some meaningful initial value from somewhere else(?)
 *  > ALUControl is a binary number on the table that describes ALU() in ProjectGuide.doc
 *      Since the value is given as a char, I am comparing with it's ASCII value directly
 *      If this doesn't work we could try casting ALUControl in the switch, or altering the values in the cases (using 0x, 0b, or char vals)
 */
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
            *ALUresult = ((signed)A < (signed)B) ? 1 : 0;
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
            B <<= 16;
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
    /*fetch the instruction addressed by PC from Mem and write it to instruction
    if PC is a multiple of 4 and is within memory*/
    if(PC % 4 == 0 && PC <= 0xFFFF)
    {
        *instruction = Mem[PC >> 2];
        return 0;
    }

    /*if a halt condition occurs:
    counter was not a multiple of 4 or was out of range*/
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
        // r-types add, sub, and, or, slt, sltu
        case 0: 
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        // jump
        case 2:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = ;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;
        // beq
        case 4:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = 1;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;
        // addi
        case 8:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = ;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;
        //sltui
        case 11:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = ;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;
        //lui
        case 15:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = ;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;
        //lw
        case 34:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = 0;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;
        //sw
        case 43:
            controls->RegDst = ;
            controls->Jump = ;
            controls->Branch = ;
            controls->MemRead = ;
            controls->MemtoReg = ;
            controls->ALUOp = 0;
            controls->MemWrite = ;
            controls->ALUSrc = ;
            controls->RegWrite = ;
            break;

    }
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    //get value of register r1 and assign it to data1
    *data1 = Reg[r1];
    //get value of register r1 and assign it to data1
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}


