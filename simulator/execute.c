#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"
#include "simulator.h"


void execute(){
	snap_file = fopen("snapshot.rpt", "w");
	err_file = fopen("error_dump.rpt", "w");
	
	
	
	cycle = 0;
    while(1)
	{
		/* PC validity check and instruction fetching setting */
		if( PC<0 || PC>1023 || PC%4!=0 ){
			
	fprintf(stderr, "Illegal testcase: instruction fetch over 1K space\n");
	
	exit(0);	}
		cmdidx = PC/4;
		
		/* register data display */
		register_output(cycle);
		cycle++;
		
		if( cycle>500000 )
		{
			fprintf( err_file, "Too many cycles\n");
			//fprintf( stdout, "Too many cycles\n");
		}
		/* register index setting */
		rs = ( i_memory[cmdidx]<<6 ) >> 27;
		rt = ( i_memory[cmdidx]<<11 ) >> 27;
		rd = ( i_memory[cmdidx]<<16 ) >> 27;
		shamt = ( i_memory[cmdidx]<<21 ) >> 27;
		imm = i_memory[cmdidx] & 0xFFFF;
		
		/* instruction display */
		//instruction_output();
		
		int status=0;
		switch( i_memory[cmdidx]>>26 )
		{
			/*halt*/
			case 0x3F: printf("halt\n"); goto BOTTOM; break;
			/*I-type*/
			case 0x08: addi(); break;
			case 0x09: addiu(); break;
			case 0x23: status = lw(); break;
			case 0x21: status = lh(); break;
			case 0x25: status = lhu(); break;
			case 0x20: status = lb(); break;
			case 0x24: status = lbu(); break;
			case 0x2B: status = sw(); break;
			case 0x29: status = sh(); break;
			case 0x28: status = sb(); break;
			case 0x0F: lui(); break;
			case 0x0C: andi(); break;
			case 0x0D: ori(); break;
			case 0x0E: nori(); break;
			case 0x0A: slti(); break;
			case 0x07: bgtz(); break;
			case 0x04: beq(); break;
			case 0x05: bne(); break;
			/*J-type*/
			case 0x02: j(); break;
			case 0x03: jal(); break;
			/*R-type*/
			case 0x00:
				funct = i_memory[cmdidx]&63;
				if( (rd==0) && (funct!=0x08) && ( (i_memory[cmdidx]&0xFC1FFFFF) !=0) )/* write 0 error occurs */
				{
					fprintf( err_file, "In cycle %d: Write $0 Error\n", cycle);
					//fprintf( stdout, "In cycle %d: Write $0 Error\n", cycle);
					/*pc += 4;*/
				}
				switch( funct )
				{
					case 0x20: add(); break;
					case 0x21: addu(); break;
					case 0x22: sub(); break;
					case 0x24: and(); break;
					case 0x25: or(); break;
					case 0x26: xor(); break;
					case 0x27: nor(); break;
					case 0x28: nand(); break;
					case 0x2A: slt(); break;
					case 0x00: sll(); break;
					case 0x02: srl(); break;
					case 0x03: sra(); break;
					case 0x08: jr(); break;
					default: //printf("exit 0x%02X\n",funct);
						printf("This is NOT an instruction!!\n"); 
							break;
				}
				break;
			default: //printf("exit 0x%02X\n",i_memory[cmdidx]>>26 );
			printf("This is NOT an instruction!!\n"); 
			break;
		}
		Register[0] = 0;/* avoid write 0 error */
		if( status==1 )/*error handling */
			goto BOTTOM;
		/*getchar();*/
	}
	BOTTOM:
	fclose( snap_file );
	fclose( err_file );
}