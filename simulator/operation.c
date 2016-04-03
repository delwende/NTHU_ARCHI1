#include <stdio.h>
#include <stdlib.h>
#include "operation.h"
#include "simulator.h"


/*due to the endian encoding, we have to reverse the order of data we read in.*/
unsigned int convert( unsigned int num )
{
	unsigned int result = 0, i;
	for(i=0; i<4; i++)
	{
		result <<= 8;
		result += (num&0xFF);
		num >>= 8;
	}
	return result;
}



void register_output(int c)
{
	int i;
	/* for the output file */
	fprintf(snap_file, "cycle %d\n", c);
	for(i=0; i<32; i++)
		fprintf(snap_file, "$%02d: 0x%08X\n",i,Register[i]);
	fprintf(snap_file, "PC: 0x%08X\n\n\n",PC);
	
}



void add()
{
	int temp = Register[rs] + Register[rt];
	//printf("add $%d,$%d,$%d\n",rd,rs,rt);
	/*reg[d] = reg[s] + reg[t];*/
	if( Register[rs]>0 && Register[rt]>0 && temp<=0 || Register[rs]<0 && Register[rt]<0 && temp>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	Register[rd] = temp;
	PC += 4;
}
void addu()
{
	int temp = Register[rs] + Register[rt];
	//printf("add $%d,$%d,$%d\n",rd,rs,rt);
	/*reg[d] = reg[s] + reg[t];*/
	/*if( Register[rs]>0 && Register[rt]>0 && temp<=0 || Register[rs]<0 && Register[rt]<0 && temp>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}*/
	Register[rd] = temp;
	PC += 4;
}

void sub()
{
	//printf("sub $%d,$%d,$%d\n",rd,rs,rt);
	int tempReg = -Register[rt], temp;
	/*reg[d] = reg[s] + tempReg; /*- reg[t];*/
	temp = Register[rs] + tempReg;
	/*if( reg[s]>0 && reg[t]<0 && reg[d]<=0 || reg[s]<0 && reg[t]>0 && reg[d]>=0 )*/
	/*if( reg[s]>0 && tempReg>0 && reg[d]<=0 || reg[s]<0 && tempReg<0 && reg[d]>=0 )*/
	if( Register[rs]>0 && tempReg>0 && temp<=0 || Register[rs]<0 && tempReg<0 && temp>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	Register[rd] = temp;
	PC += 4;
}

void and()
{
	//printf("and $%d,$%d,$%d\n",rd,rs,rt);
	Register[rd] = Register[rs] & Register[rt];
	PC += 4;
}

void or()
{
	//printf("or $%d,$%d,$%d\n",rd,rs,rt);
	Register[rd] = Register[rs] | Register[rt];
	PC += 4;
}

void xor()
{
	//printf("xor $%d,$%d,$%d\n",rd,rs,rt);
	Register[rd] = Register[rs] ^ Register[rt];
	PC += 4;
}

void nor()
{
	//printf("nor $%d,$%d,$%d\n",rd,rs,rt);
	Register[rd] = ~( Register[rs] | Register[rt] );
	PC += 4;
}

void nand()
{
	//printf("nand $%d,$%d,$%d\n",rd,rs,rt);
	Register[rd] = ~( Register[rs] & Register[rt] );
	PC += 4;
}

void slt()
{
	//printf("slt $%d,$%d,$%d\n",rd,rs,rt);
	Register[rd] = ( Register[rs] < Register[rt] ) ? 1 : 0;
	PC += 4;
}

void sll()
{
	//printf("sll $%d,$%d,%d\n",rd,rt,shamt);
	Register[rd] = Register[rt] << shamt;
	PC += 4;
}

void srl()
{
	//printf("srl $%d,$%d,%d\n",rd,rt,shamt);
	unsigned int temp = Register[rt];
	Register[rd] = temp >> shamt;
	PC += 4;
}

void sra()
{
	//printf("sra $%d,$%d,%d\n",rd,rt,shamt);
	Register[rd] = Register[rt] >> shamt;
	PC += 4;
}

void jr()
{
	//printf("jr $%d\n",rs);
	PC = Register[rs];
}

void addi()
{
	int temp = imm, tempResult;
	temp = ( (temp<<16)>>16 );
	//printf("addi $%d,$%d,%d\n",rt,rs,temp);
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	/*reg[t] = reg[s] + temp;*/
	tempResult = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && tempResult<=0 || Register[rs]<0 && temp<0 && tempResult>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	Register[rt] = tempResult;
	PC += 4;
}
void addiu()
{
	int temp = imm, tempResult;
	temp = ( (temp<<16)>>16 );
	//printf("addi $%d,$%d,%d\n",rt,rs,temp);
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	/*reg[t] = reg[s] + temp;*/
	tempResult = Register[rs] + temp;
	/*if( reg[s]>0 && temp>0 && tempResult<=0 || reg[s]<0 && temp<0 && tempResult>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}*/
	Register[rt] = tempResult;
	PC += 4;
}
int lw()
{
	char should_halt = 0;/*boolean variable*/
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("lw $%d,%d($%d)\n",rt,temp,rs);
	
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf( err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1020 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		should_halt = 1;
	}
	if( addr%4 != 0 )
	{
		fprintf(err_file , "In cycle %d: Misalignment Error\n", cycle);
		//fprintf(stdout , "In cycle %d: Misalignment Error\n", cycle);
		should_halt = 1;
	}
	if( should_halt==1 )
		return 1;
	unsigned int temp2;
	temp2 = d_memory[ addr + 0 ];
	Register[rt] &= 0x00FFFFFF;
	Register[rt] += temp2<<24;
	temp2 = d_memory[ addr + 1 ];
	Register[rt] &= 0xFF00FFFF;
	Register[rt] += temp2<<16;
	temp2 = d_memory[ addr + 2 ];
	Register[rt] &= 0xFFFF00FF;
	Register[rt] += temp2<<8;
	temp2 = d_memory[ addr + 3 ];
	Register[rt] &= 0xFFFFFF00;
	Register[rt] += temp2<<0;
	PC += 4;
	return 0;
}

int lh()
{
	char should_halt = 0;/*boolean variable*/
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("lh $%d,%d($%d)\n",rt,temp,rs);
	
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1022 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		should_halt = 1;
	}
	if( addr%2!=0 )
	{
		fprintf(err_file , "In cycle %d: Misalignment Error\n", cycle);
		//fprintf(stdout , "In cycle %d: Misalignment Error\n", cycle);
		should_halt = 1;
	}
	if( should_halt==1 )
		return 1;
	unsigned int temp2;
	temp2 = d_memory[ addr + 0 ];
	Register[rt] &= 0x00000000;/*FFFF00FF*/
	Register[rt] = temp2<<8;
	temp2 = d_memory[ addr + 1 ];
	/*reg[t] &= 0xFFFFFF00;*/
	Register[rt] += temp2<<0;
	Register[rt] = (Register[rt]<<16)>>16;
	PC += 4;
	return 0;
}

int lhu()
{
	char should_halt = 0;/*boolean variable*/
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("lhu $%d,%d($%d)\n",rt,temp,rs);
	
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1022 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		should_halt = 1;
	}
	if( addr%2!=0 )
	{
		fprintf(err_file , "In cycle %d: Misalignment Error\n", cycle);
		//fprintf(stdout , "In cycle %d: Misalignment Error\n", cycle);
		should_halt = 1;
	}
	if( should_halt==1 )
		return 1;
	unsigned int temp2;
	temp2 = d_memory[ addr + 0 ];
	Register[rt] &= 0x00000000;/*FFFF00FF*/
	Register[rt] = (temp2<<8);
	temp2 = d_memory[ addr + 1 ];
	/*reg[t] &= 0xFFFFFF00;*/
	Register[rt] += (temp2<<0);
	PC += 4;
	
	return 0;
}

int lb()
{
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("lb $%d,%d($%d)\n",rt,temp,rs);
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1023 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		return 1;
	}
	unsigned int temp2;
	temp2 = d_memory[ addr ];
/*
	reg[t] &= 0x000000FF;
	reg[t] += temp2;*/
	Register[rt] = temp2;
	Register[rt] = (Register[rt]<<24)>>24;
	PC += 4;
	return 0;
}

int lbu()
{
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("lbu $%d,%d($%d)\n",rt,temp,rs);
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1023 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		return 1;
	}
	unsigned int temp2;
	temp2 = d_memory[ addr ];
	/*reg[t] &= 0x000000FF;
	reg[t] += temp2;*/
	Register[rt] = temp2;
	PC += 4;
	return 0;
}

int sw()
{
	char should_halt = 0;/*boolean variable*/
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("sw $%d,%d($%d)\n",rt,temp,rs);
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf( err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1020 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		should_halt = 1;
	}
	if( addr%4 != 0 )
	{
		fprintf(err_file , "In cycle %d: Misalignment Error\n", cycle);
		//fprintf(stdout , "In cycle %d: Misalignment Error\n", cycle);
		should_halt = 1;
	}
	if( should_halt==1 )
		return 1;
	unsigned int temp2;
	temp2 = Register[rt] & 0xFF000000;
	d_memory[ addr + 0 ] = ( temp2>>24 );
	temp2 = Register[rt] & 0x00FF0000;
	d_memory[ addr + 1 ] = ( temp2>>16 );
	temp2 = Register[rt] & 0x0000FF00;
	d_memory[ addr + 2 ] = ( temp2>>8 );
	temp2 = Register[rt] & 0x000000FF;
	d_memory[ addr + 3 ] = ( temp2>>0 );
	PC += 4;
	return 0;
}

int sh()
{
	char should_halt = 0;/*boolean variable*/
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("sh $%d,%d($%d)\n",rt,temp,rs);
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1022 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		should_halt = 1;
	}
	if( addr%2!=0 )
	{
		fprintf(err_file , "In cycle %d: Misalignment Error\n", cycle);
		//fprintf(stdout , "In cycle %d: Misalignment Error\n", cycle);
		should_halt = 1;
	}
	if( should_halt==1 )
		return 1;
	unsigned int temp2;
	temp2 = Register[rt] & 0x0000FF00;
	d_memory[ addr + 0 ] = ( temp2>>8 );
	temp2 = Register[rt] & 0x000000FF;
	d_memory[ addr + 1 ] = ( temp2>>0 );
	PC += 4;
	return 0;
}

int sb()
{
	int temp = imm, addr;
	temp = ( (temp<<16)>>16 );
	//printf("sb $%d,%d($%d)\n",rt,temp,rs);
	addr = Register[rs] + temp;
	if( Register[rs]>0 && temp>0 && addr<=0 || Register[rs]<0 && temp<0 && addr>=0 )
	{
		fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
	}
	if( addr<0 || addr>1023 )
	{
		fprintf( err_file , "In cycle %d: Address Overflow\n", cycle);
		//fprintf( stdout , "In cycle %d: Address Overflow\n", cycle);
		return 1;
	}
	unsigned int temp2;
	temp2 = Register[rt] & 0x000000FF;
	d_memory[ addr ] = temp2;
	PC += 4;
	return 0;
}

void lui()
{
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	else
		Register[rt] = imm<<16;
	PC += 4;
	printf("lui $%d,%d\n",rt,imm);
}

void andi()
{
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	else
		Register[rt] = Register[rs] & imm;
	PC += 4;
	//printf("andi $%d,$%d,%d\n",rt,rs,imm);
}

void ori()
{
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	else
		Register[rt] = Register[rs] | imm;
	PC += 4;
	//printf("ori $%d,$%d,%d\n",rt,rs,imm);
}

void nori()
{
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	else
		Register[rt] = ~ (Register[rs] | imm);
	PC += 4;
	printf("nori $%d,$%d,%d\n",rt,rs,imm);
}

void slti()
{
	int temp = imm;
	if( rt==0 )
	{
		fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
		//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
	}
	else
	{
		temp = ( (temp<<16)>>16 );
		Register[rt] = ( Register[rs] < temp ) ? 1 : 0;
		//printf("slti $%d,$%d,%d\n",rt,rs,temp);
	}
	PC += 4;
}

void beq()
{
	int temp = imm;
	temp = ( (temp<<16)>>16 );
	//printf("beq $%d,$%d,%d\n",rs,rt,temp);
	temp <<= 2;
	PC += 4;
	if( Register[rs]==Register[rt] )
	{
		int tempPC = PC + temp;
		if( PC>0 && temp>0 && tempPC<=0 || PC<0 && temp<0 && tempPC>=0 )
		{
			fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
			//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
		}
		PC = tempPC;
	}
}

void bne()
{
	int temp = imm;
	temp = ( (temp<<16)>>16 );
	//printf("bne $%d,$%d,%d\n",rs,rt,temp);
	temp <<= 2;
	PC += 4;
	if( Register[rs]!=Register[rt] )
	{
		int tempPC = PC + temp;
		if( PC>0 && temp>0 && tempPC<=0 || PC<0 && temp<0 && tempPC>=0 )
		{
			//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
			fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		}
		PC = tempPC;
	}
}
void bgtz()
{
	int temp = imm;
	temp = ( (temp<<16)>>16 );
	//printf("bne $%d,$%d,%d\n",rs,rt,temp);
	temp <<= 2;
	PC += 4;
	if( Register[rs]>0 )
	{
		int tempPC = PC + temp;
		if( PC>0 && temp>0 && tempPC<=0 || PC<0 && temp<0 && tempPC>=0 )
		{
			//fprintf(stdout , "In cycle %d: Number Overflow\n", cycle);
			fprintf(err_file , "In cycle %d: Number Overflow\n", cycle);
		}
		PC = tempPC;
	}
}
void j()
{
	//printf("j %d\n",imm);
	PC = ( (PC+4)&0xF0000000 ) | ( imm<<2 );
}

void jal()
{
	//printf("jal %d\n",imm);
	Register[31] = PC + 4;
	PC = ( (PC+4)&0xF0000000 ) | ( imm<<2 );
}
