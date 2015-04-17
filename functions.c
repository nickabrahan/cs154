
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

// these are the structures used in this simulator


// global variables
// register file
int regfile[32];
// instruction memory
int instmem[100];  // only support 100 static instructions
// data memory
int dataMem[1024];
// program counter
int pc =0;

int countIn=0;
int countOut=0;
int i=0;

// these are the different functions you need to write
int load(char *filename);
void fetch(InstInfo *);
void decode(InstInfo *);
void execute(InstInfo *);
void memory(InstInfo *);
void writeback(InstInfo *);

/* load
 *
 * Given the filename, which is a text file that 
 * contains the instructions stored as integers 
 *
 * You will need to load it into your global structure that 
 * stores all of the instructions.
 *
 * The return value is the maxpc - the number of instructions
 * in the file
 */
int load(char *filename)
{
  FILE *fp;
  fp = fopen(filename, "r");
  if(fp == NULL)
    {
    printf("ERROR 404: FILE NOT FOUND \n");
    return 1;
    }



  int currentInstruction;
  
  while(fscanf(fp, "%i", &currentInstruction) != EOF)
    {
      instmem[i] = currentInstruction;
      //printf("%d was the instruction that was just added. \n", currentInstruction);
      i++;
    }
  
   return i;
}

/* fetch
 *
 * This fetches the next instruction and updates the program counter.
 * "fetching" means filling in the inst field of the instruction.
 */
void fetch(InstInfo *instruction)
{ 
  int x;
  for(x = 0; x <= i; x++){

  instruction->inst = instmem[i];

  }
}

/* decode
 *
 * This decodes an instruction.  It looks at the inst field of the 
 * instruction.  Then it decodes the fields into the fields data 
 * member.  The first one is given to you.
 *
 * Then it checks the op code.  Depending on what the opcode is, it
 * fills in all of the signals for that instruction.
 */
void decode(InstInfo *instruction)
{
	// fill in the signals and fields
	int val = instruction->inst;
	int op, func;
	instruction->fields.op = (val >> 26) & 0x03f;
	instruction->fields.func = (val & 0x03f);
	instruction->fields.imm = val & 0xFFFF;
	// fill in the rest of the fields here
	//printf("decode op: %d \n\n", instruction->fields.op); 
	if(instruction -> fields.op == 0b110001)
	  {

	    //ADDI
	    instruction->fields.imm = val & 0xFFFF;
	    // printf("%d \n \n", instruction->fields.imm);
	    instruction->fields.rs = (val >> 21) & 0x1F;
	    instruction->fields.rt = (val >> 16) & 0x1F;

	    instruction->signals.aluop = 2;
	    instruction->signals.mw = 0;
	    instruction->signals.mr = 0;
	    instruction->signals.mtr = 0;
	    instruction->signals.asrc = 1;
	    instruction->signals.btype = 0;
	    instruction->signals.rdst = 0;
	    instruction->signals.rw = 1;
	    sprintf(instruction->string,"addi $%d, $%d, %d",
		    instruction->fields.rt, instruction->fields.rs,
		    instruction->fields.imm);
	    instruction->destreg = instruction->fields.rt;

	    instruction->input1 = instruction->fields.rs;

	    instruction->s1data = regfile[instruction->fields.rs];
	    instruction->s2data = instruction->fields.imm;
	    //  printf("%d, %d, ARE THE VALUES OF s1D and S2D \n \n", instruction->s1data, instruction->s2data);
	  }

	

	if(instruction -> fields.op == 0b100000)
	  {
	    if(instruction -> fields.func == 0b100000) //add
	      {
		
		instruction->fields.rd = (val >> 11) & 0x1F;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
                instruction->signals.aluop = 2;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = 0;
                instruction->signals.asrc = 0;
                instruction->signals.btype = 0;
                instruction->signals.rdst = 1;
                instruction->signals.rw = 1;
                sprintf(instruction->string,"add $%d, $%d, $%d",
                        instruction->fields.rd, instruction->fields.rs,
                        instruction->fields.rt);
                instruction->destreg = instruction->fields.rd;
		
		instruction->input1 = instruction->fields.rs;
		instruction->input2 = instruction->fields.rt;
		instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = regfile[instruction->fields.rt];

	      }
	    
	    if(instruction -> fields.func == 0b100100) //and
	      {
		instruction->fields.rd = (val >> 11) & 0x1F;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
		
                instruction->signals.aluop = 0;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = 0;
                instruction->signals.asrc = 0;
                instruction->signals.btype = 0;
                instruction->signals.rdst = 1;
                instruction->signals.rw = 1;
                sprintf(instruction->string,"and $%d, $%d, $%d",
                        instruction->fields.rd, instruction->fields.rs,
                        instruction->fields.rt);
                instruction->destreg = instruction->fields.rd;

		instruction->input1 = instruction->fields.rs;
                instruction->input2 = instruction->fields.rt;
		instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = regfile[instruction->fields.rt];
	      }
	    
	    if(instruction -> fields.func == 0b101000) // sub
	      {
		instruction->fields.rd = (val >> 11) & 0x1F;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
		
                instruction->signals.aluop = 3;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = 0;
                instruction->signals.asrc = 0;
                instruction->signals.btype = 0;
                instruction->signals.rdst = 1;
                instruction->signals.rw = 1;
                sprintf(instruction->string,"sub $%d, $%d, $%d",
                        instruction->fields.rd, instruction->fields.rs,
                        instruction->fields.rt);
                instruction->destreg = instruction->fields.rd;

		instruction->input1 = instruction->fields.rs;
                instruction->input2 = instruction->fields.rt;
                instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = regfile[instruction->fields.rt];
		
	      }
	    
	    if(instruction -> fields.func == 0b110000) // sgt
	      {
		instruction->fields.rd = (val >> 11) & 0x1F;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
                instruction->signals.aluop = 7;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = 0;
                instruction->signals.asrc = 0;
                instruction->signals.btype = 0;
                instruction->signals.rdst = 1;
                instruction->signals.rw = 1;
                sprintf(instruction->string,"sgt $%d, $%d, $%d",
                        instruction->fields.rd, instruction->fields.rs,
                        instruction->fields.rt);
                instruction->destreg = instruction->fields.rd;

		instruction->input1 = instruction->fields.rs;
                instruction->input2 = instruction->fields.rt;
                instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = regfile[instruction->fields.rt];
	      }
	  }

	    if(instruction -> fields.op == 0b010001) //LW
	      {

		instruction->fields.imm = val & 0xFFFF;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
		
                instruction->signals.aluop = 2;
                instruction->signals.mw = 0;
                instruction->signals.mr = 1;
                instruction->signals.mtr = 1;
                instruction->signals.asrc = 1;
                instruction->signals.btype = 0;
                instruction->signals.rdst = 0;
                instruction->signals.rw = 1;
                sprintf(instruction->string,"lw $%d, %d($%d)",
                        instruction->fields.rt, instruction->fields.imm,
                        instruction->fields.rs);
                instruction->destreg = instruction->fields.rt;

		instruction->input1 = instruction->fields.rs;
		
		instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = instruction->fields.imm;
	      }


	    if(instruction -> fields.op == 0b010010) //SW
	      {

		instruction->fields.imm = val & 0xFFFF;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
		
                instruction->signals.aluop = 2;
                instruction->signals.mw = 1;
                instruction->signals.mr = 0;
                instruction->signals.mtr = -1;
                instruction->signals.asrc = 1;
                instruction->signals.btype = 0;
                instruction->signals.rdst = -1;
                instruction->signals.rw = 0;
                sprintf(instruction->string,"sw $%d, %d($%d)",
                        instruction->fields.rt, instruction->fields.imm,
                        instruction->fields.rs);
                instruction->destreg = -1;

		instruction->input1 = instruction->fields.rs;

		instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = instruction->fields.imm;
	      }

	    if(instruction -> fields.op == 0b001010) //BEQ
	      {
		instruction->fields.imm = val & 0xFFFF;
		instruction->fields.rt = (val >> 16) & 0x1F;
		instruction->fields.rs = (val >> 21) & 0x1F;
		
		instruction->signals.aluop = 3;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = -1;
                instruction->signals.asrc = 0;
                instruction->signals.btype = 3;
                instruction->signals.rdst = -1;
                instruction->signals.rw = 0;
                sprintf(instruction->string,"beq $%d, $%d, %d",
                        instruction->fields.rs, instruction->fields.rt,
                        instruction->fields.imm);
                instruction->destreg = -1;

		instruction->input1 = instruction->fields.rs;
		instruction->input2 = instruction->fields.rt;
		
		instruction->s1data = regfile[instruction->fields.rs];
		instruction->s2data = regfile[instruction->fields.rt];

		

	      }
	    if(instruction -> fields.op == 0b100101) //jr
	      {
		instruction->fields.rs = (val >> 21) & 0x1F;
		
		instruction->signals.aluop = 2;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = -1;
                instruction->signals.asrc = 0;
                instruction->signals.btype = 2;
                instruction->signals.rdst = -1;
                instruction->signals.rw = 0;
                sprintf(instruction->string,"jr $%d",
                        instruction->fields.rs);
                instruction->destreg = -1;
		
		instruction->input1 = instruction->fields.rs;
		instruction->s1data = regfile[instruction->fields.rs];

	      }

	    if(instruction -> fields.op == 0b001000) //jal
	      {
		instruction->fields.imm = val & 0x3FFFFFF;
		
		instruction->signals.aluop = 2;
                instruction->signals.mw = 0;
                instruction->signals.mr = 0;
                instruction->signals.mtr = -1;
                instruction->signals.asrc = 1;
                instruction->signals.btype = 1;
                instruction->signals.rdst = -1;
                instruction->signals.rw = 0;
                sprintf(instruction->string,"jal %d",
                        instruction->fields.imm);
                instruction->destreg = -1;

		instruction->s1data = instruction->fields.imm;
	      }


	// fill in s1data and input2
	  }

/* execute
 *
 * This fills in the aluout value into the instruction and destdata
 */

void execute(InstInfo *instruction)
{

  //Number stored in rs/rt refer to the regfile array
  //printf("execute: op: %d  \n \n", instruction->fields.op); 

 if(instruction -> fields.op == 0b110001)
    {
      dataMem[countIn] = instruction->s1data + instruction->s2data;
      //printf("%d, IS THE VALUE OF DATAMEM \n", dataMem[countIn]);
      instruction->aluout = dataMem[countIn];
      countIn++;
    }

  if(instruction -> fields.op == 0b100000)
    {
      if(instruction -> fields.func == 0b100000) //add
	{
	  dataMem[countIn] = instruction->s1data + instruction->s2data;
	  countIn++;
	}
      
      if(instruction -> fields.func == 0b100100) //and
	{
	  dataMem[countIn] = instruction->s1data & instruction->s2data;
	  countIn++;
	}
      
      if(instruction -> fields.func == 0b101000) // sub
	{
	  dataMem[countIn] = instruction->s1data - instruction->s2data;
	  countIn++;
	    }
      
      if(instruction -> fields.func == 0b110000) // sgt
	{
	  if(instruction->s1data > instruction->s2data)
	    dataMem[countIn] = 1;
	  else
	    dataMem[countIn] = 0;
	  countIn++;
	}
      instruction->aluout = dataMem[countIn];
    }
  pc++;
}

/* memory
 *
 * If this is a load or a store, perform the memory operation
 */
void memory(InstInfo *instruction)
{
  if(instruction -> fields.op == 0b010001) //LW
    {
      dataMem[countIn] = dataMem[instruction->s1data + instruction->s2data];
      countIn++;
    }


  if(instruction -> fields.op == 0b010001) //SW
    {
      dataMem[instruction->s1data + instruction->s2data] = dataMem[countIn];
      countIn++;
    }
}

/* writeback
 *
 * If a register file is supposed to be written, write to it now
 */
void writeback(InstInfo *instruction)
{
  if(instruction -> fields.op == 0b110001 || instruction -> fields.op == 0b100000)
    {
      regfile[instruction->destreg] = dataMem[countOut];
      countOut++;
    }
}

/* setPCWithInfo
 *
 * branch instruction will overwrite PC
*/
void setPCWithInfo( InstInfo *instruction)
{
  if(instruction -> fields.op == 0b100101) //jr
    {
      pc = (pc + instruction->s1data); 
    }

  if(instruction -> fields.op == 0b001000) //jal
    {
      regfile[31] = pc + 1;
      pc = (pc + instruction->s1data);
    }
}
