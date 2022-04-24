#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
#include<math.h>

typedef struct{     
    unsigned char op;        // operacao
    unsigned char opr1;      // operando1
    unsigned char opr2;      // operando2
    unsigned char opr3;      // operando3
} Tinstruction;
int p;  			
Tinstruction i;
uint8_t RAM[256];
uint8_t reg[16];   // registradores
int h;
// List of available instructions
char * instructionString[] = { "LOAD", "LOAD", "STORE", "MOVE", "ADD", "ADD", "OR", "AND", "EXCLS", "ROTAT", "JUMP", "HALT"};

// Prototypes 
char * getInstructionName(int inst);
uint8_t sum_comp2(uint8_t val1, int val2);
uint8_t sum_flut(uint8_t val1, int val2);
uint8_t rot(uint8_t val1, uint8_t val2);
uint8_t trans(uint8_t val1, uint8_t val2);
Tinstruction transformInstruction(uint8_t a, uint8_t b);
int main(){
	int aa = 5;
	printf("start                   registradores\n");
	h = 0; p=0;
   	//operador&operando1       operando2&operando3
	RAM[ 0x0] = 0x20;          RAM[ 0x1] = aa;        //RAM[ 0x0]: 2005 LOAD 0 05
	RAM[ 0x2] = 0x21;          RAM[ 0x3] = 0x01;      //RAM[ 0x2]: 2101 LOAD 1 01
	RAM[ 0x4] = 0x2f;          RAM[ 0x5] = 0x01;      //RAM[ 0x4]: 2f01 LOAD f 01
	RAM[ 0x6] = 0x30;          RAM[ 0x7] = 0x25;      //RAM[ 0x6]: 3025 STORE 0 25
	RAM[ 0x8] = 0x31;          RAM[ 0x9] = 0x26;      //RAM[ 0x8]: 3126 STORE 1 26
	RAM[ 0xa] = 0x10;          RAM[ 0xb] = 0x25;      //RAM[ 0xa]: 1025 LOAD 0 25
	RAM[ 0xc] = 0x14;          RAM[ 0xd] = 0x26;      //RAM[ 0xc]: 1426 LOAD 4 26
	RAM[ 0xe] = 0x52;          RAM[ 0xf] = 0x24;      //RAM[ 0xe]: 5224 ADD 2 24
	RAM[0x10] = 0xb1;          RAM[0x11] = 0x1a;      //RAM[0x10]: b11a JUMP 1 1a
	RAM[0x12] = 0x51;          RAM[0x13] = 0x1f;      //RAM[0x12]: 511f ADD 1 1f
	RAM[0x14] = 0x31;          RAM[0x15] = 0x26;      //RAM[0x14]: 3126 STORE 1 26
	RAM[0x16] = 0x10;          RAM[0x17] = 0x26;      //RAM[0x16]: 1026 LOAD 0 26
	RAM[0x18] = 0xb1;          RAM[0x19] = 0x08;      //RAM[0x18]: b108 JUMP 1 08
	RAM[0x1a] = 0xc0;          RAM[0x1b] = 0x00;      //RAM[0x1a]: c000 HALT 0 00 
	//END 0 0

    printf("%5s %5s %5s %5s","op","opr1","opr2","opr3");
    for(h = 0; h <16; h++) {
        printf(" %8x", h);
    }
    printf("\n");
    printf("----- ----- ----- -----");
    for(h = 0; h <16; h++) {
        printf(" --------");
    }
    printf("\n");
    do {
		i = transformInstruction(RAM[p], RAM[p+1]);
		p+=2;
		switch (i.op) {
			case 1: reg[i.opr1] = RAM[trans(i.opr2, i.opr3)];				    break; // LOAD registrador <- RAM
			case 2: reg[i.opr1] = trans(i.opr2, i.opr3); 						break; // LOAD registrador <- constante
			case 3: RAM[trans(i.opr2, i.opr3)] = reg[i.opr1];			    	break; // STORE
			case 4: reg[i.opr3] = reg[i.opr2]; 									break; // MOVE
			case 5: reg[i.opr1] = sum_comp2(reg[i.opr2], reg[i.opr3]);			break; // ADD complemento de 2
			case 6: reg[i.opr1] = sum_flut(reg[i.opr2], reg[i.opr3]);			break; // ADD ponto flutuante
			case 7: reg[i.opr1] = reg[i.opr2] | reg[i.opr3]; 					break; // OR
			case 8: reg[i.opr1] = reg[i.opr2] & reg[i.opr3]; 					break; // AND
			case 9: reg[i.opr1] = reg[i.opr2] ^ reg[i.opr3]; 					break; // OR EXCLUSIVO
			case 0xA: reg[i.opr1] = rot(reg[i.opr1], i.opr3); 					break; // ROTACAO
			case 0xB: if(reg[i.opr1] == reg[0]) {p = trans(i.opr2, i.opr3);}; 	break; // JUMP
			case 0xC: p = 0; 													break; // HALT
		}
		
        printf("%5s %5x %5x %5x ", getInstructionName(i.op), i.opr1, i.opr2, i.opr3);
		for (h = 0; h < 16; h++) {
			int aux = reg[h];
            for(int j = 7; j > -1; j--) {
                if ( (aux/pow(2, j)) >= 1 ) { aux -= pow(2, j); printf("1"); }
                else { printf("0"); }
            }
            printf(" ");
			//printf(" %8d", reg[h]);
		}
		printf("\n");
		
    } while (p != 0);
    printf("----- ----- ----- -----");
    for(h = 0; h <16; h++) {
        printf(" --------");
    }
    printf("\n");
    printf("%5s %5s %5s %5s","op","opr1","opr2","opr3");
    for(h = 0; h <16; h++) {
        printf(" %8x", h);
    }
    printf("\nend                     registradores\n");
    return 0;
}

uint8_t sum_comp2(uint8_t val1, int val2){
    uint8_t comp2_val1 = (255 - val1) + 0b00000001;
    uint8_t comp2_val2 = (255 - val2) + 0b00000001;
    uint8_t result = comp2_val1 + comp2_val2;
    result = (255 - result) + 0b00000001;
    return result;
}

uint8_t sum_flut(uint8_t val1, int val2){
    uint8_t sinal1, sinal2, expv1, expv2, mant1, mant2, result, soma;
    sinal1 = val1>>7;
    sinal2 = val2>>7;
    expv1 = val1<<1; expv1 = expv1>>5;
    expv2 = val2<<1; expv2 = expv2>>5;
    mant1 = val1<<4; mant1 = mant1>>4;
    mant2 = val2<<4; mant2 = mant2>>4;
    
    if(expv1 != expv2){
        int aux;
        if(expv1 > expv2){
            aux = expv1-expv2;
            mant2>>aux;
        }
        else{
            aux = expv2-expv1;
            mant1>>aux;
        }
    }
    
    if(sinal1 == 0){
        if(sinal2 == 0){
            soma = mant1 + mant2;
            result = sinal1<<7;
            result += expv1<<4;
            result += soma;
            return result;
        }
        else{
            soma = abs(mant1 - mant2);
            if(mant2 > mant1) { result = sinal2<<7; }
            else { result = sinal1<<7;}
            result += expv1<<4;
            result += soma;
            return result;
        }
    }
    else{
        if(sinal2 == 0){
            soma = abs(mant1 - mant2);
            if(mant1 > mant2) { result = sinal1<<7; }
            else { result = sinal2<<7;}
            result += expv1<<4;
            result += soma;
            return result;
        }
        else{
            soma = abs(mant1 + mant2);
            result = sinal1<<7;
            result += expv1<<4;
            result += soma;
            return result;
        }
    }
}

uint8_t rot(uint8_t val1, uint8_t val2){
    uint8_t a = val1>>val2 ;
    uint8_t b = val1<<(8-val2);
    uint8_t c = a+b;
    return c;
}
uint8_t trans(uint8_t val1, uint8_t val2){
    uint8_t a = val1*16;
    uint8_t b = a + val2;
    return b;
}
Tinstruction transformInstruction(uint8_t a, uint8_t b){
    Tinstruction result;
    result.op = a>>4;
    result.opr1 = a<<4; result.opr1 = result.opr1>>4;
    result.opr2 = b>>4;
    result.opr3 = b<<4; result.opr3 = result.opr3>>4;
    return result;
}
char * getInstructionName(int inst){
	return instructionString[inst - 1];
}

