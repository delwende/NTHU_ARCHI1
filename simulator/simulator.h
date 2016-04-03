#ifndef SIMULATOR_H
#define SIMULATOR_H

 FILE *fin, *snap_file, *err_file;
 int Register[32];/* There are 32 registers. */
 unsigned int i_memory[256];/*1K bytes are 256 words. */
 unsigned char d_memory[1024];/*1K bytes are 1024 bytes. */
 int line; /* total line counter */
 int counter;/* real total number of data or instructions */
 int PC; /* program counter */
 int cycle; /* record the current cycle */

/* variables for instruction data */
int cmdidx;
unsigned int rs, rt, rd, shamt, imm, funct;
	void init();
	void reader_dimage();
	void reader_iimage();
	void execute();
#endif