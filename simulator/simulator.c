#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "simulator.h"



int main(int argc, char* argv[]) {

	init();
	reader_dimage();
	reader_iimage();
	execute();
	/*getchar();*/
    return 0; 
}
void init(){
	memset( Register, 0, sizeof(Register) );
	memset( d_memory, 0, sizeof(d_memory) );
	memset( i_memory, 0, sizeof(i_memory) );

}
void reader_dimage (){
	FILE *d_image=fopen("dimage.bin","rb");
    
	//fin = fopen("dimage.bin", "rb");
	 if (d_image == NULL){
		fprintf(stderr, "Illegal testcase: Cannot open \"dimage.bin\"\n");
		exit(2);
	}
	
	//file_open_failure_process( fin, "dimage.bin" );
	
	/* one instruction each loop */
	printf("#1 The following is the data image in hexadecimal form.\n");
	line=0; counter=2147483640;
	while( !feof(d_image) )
	{
		++line;
		unsigned int num;
        fread(&num, sizeof(unsigned int), 1, d_image);
		num = convert(num);
		//printf("line[%03d]: %08X\n", line, num);
		if( line==1 )
			Register[29] = num; /* stack pointer */
		else if( line==2 )
			counter = num;/* how many data */
		else
		{
			if( line-3>=256 )
			{
				printf("The data image input is too much for the D-memory!!\n");
				printf("The test case may be invalid!!\n");
				break;
			}
			unsigned int temp = 0xFF000000;
			int i;
			for(i=0; i<4; i++)/* load into the memory */
			{
				d_memory[ 4*(line-3)+i ] = ( (num&temp) >> (24-8*i) );
				temp >>= 8;
			}
		}
		/*If the given input is too much, stop reading. */
		if( line-3>=counter-1 )
			break;
	}
	if( line-3<counter-1 )
	{
		printf("The real data image input is less than the amount specified!!\n");
		printf("The test case may be invalid!!\n");
	}
	printf("\n");
    fclose(d_image);
	
	/*printf("#2 The following is the initial data memory.\n");
	int i;
	for(i=0; i<256; i++)
		printf("D-mem[%03d]=%02x%02x%02x%02x\n",i,d_memory[4*i],d_memory[4*i+1],d_memory[4*i+2],d_memory[4*i+3]);
	printf("\n");*/
}
void reader_iimage(){
	FILE *i_image=fopen("iimage.bin","rb");
if (i_image == NULL){
		fprintf(stderr, "Illegal testcase: Cannot open \"iimage.bin\"\n");
		exit(2);
	}
	line=0; counter=2147483640;
	while( !feof(i_image) )
	{
		++line;
		unsigned int num;
        fread(&num, sizeof(unsigned int), 1, i_image);
		num = convert(num);
		//printf("line[%03d]: %08X\n", line, num);
		if( line==1 )
		{
			PC = num;/* program counter initialization */
			if( PC<0 || PC>1023 || PC%4!=0 ){
				printf("The program counter is not on a valid position.\n");
				exit(0);
			}
		}
		else if( line==2 )
		{
			counter = num;/* how many instructions */
			if( counter<0 ){
				printf("The negative number of instructions is obviously invalid.\n");
				exit(0);
			}
		}
		else
		{
			if( PC/4+line-3>=256 )
			{
				printf("The instruction image input is too much for the I-memory!!\n");
				printf("The test case may be invalid!!\n");
				break;
			}
			i_memory[PC/4+line-3] = num;
		}
		/*If the given input is too much, stop reading. */
		if( line-3>=counter-1 )
			break;
	}
	if( line-3<counter-1 )
	{
		printf("The real data image input is less than the amount specified!!\n");
		printf("The test case may be invalid!!\n");
	}
    putchar('\n');
    fclose(i_image);
	

}
