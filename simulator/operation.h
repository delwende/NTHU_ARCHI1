#ifndef OPERATION_H
#define OPERATION_H

unsigned int convert( unsigned int num );

void register_output(int c);
void instruction_output();
void add();
void addu();
void sub();
void and();
void or();
void xor();
void nor();
void nand();
void slt();
void sll();
void srl();
void sra();
void jr();
void addi();
void addiu();
int lw();
int lh();
int lhu();
int lb();
int lbu();
int sw();
int sh();
int sb();
void lui();
void andi();
void ori();
void nori();
void slti();
void beq();
void bne();
void bgtz();
void j();
void jal();
unsigned int convert( unsigned int num );

void register_output(int c);
void instruction_output();
#endif // OPERATION_H