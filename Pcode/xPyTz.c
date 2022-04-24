/*
    Author: Gustavo B. Fragoso
    2021-10-26: Updated by Allana Campos passing as input fixed values for the multiplication of two values and the adition of another value
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX_INSTRUCTIONS 50 // Define your own
#define STACKSIZE 1024

typedef struct{
    int f;
    int l;
    int a;
} Tinstruction;

int p;  			// Points to an i in the program area
int b;				// Points to the b address in the s for the current invocation of a given procedure
int t; 				// Points to the current t of the s
int totalInstrctns = 0;
Tinstruction i;
int s[STACKSIZE] = {-1};		// Vector with only integers, used as a datastore
int h;
// Array of instructions (Input)
Tinstruction code[MAX_INSTRUCTIONS]; 

// List of available instructions
enum { LIT, OPR, LOD, STO, CAL, INT, JMP, JPC } instructionCode;
char * instructionString[] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"};

// List of available arithmetic operations
enum { RTN, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ } operationCode;
char * operationString[] = { "RTN", "NEG", "ADD", "SUB", "MUL", "DIV", "MOD", "ODD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};

// Prototypes 
void executeInstruction();
void stackOperation(int a);
int base(int l);
int getInstructionCode(char *c);
char * getInstructionName(int inst);
void pcode(int aa, int ab, int ac);
int main(){
    FILE *times;
    times = fopen("seconds-60.txt", "w");
    int aa = 2, ab = 4, ac = 8;
    time_t t_ini, t_fim;
    float trec = 0;
    while(aa < 129){
        t_ini = time(NULL); 
        pcode(aa, ab, ac);
        t_fim = time(NULL);
        trec = difftime(t_fim, t_ini);
        fprintf(times, "%d\t%d\t%d\t%f\n", aa, ab, ac, trec );
        aa*=2;
        ab*=2;
        ac*=2;
    }
    fflush(times);
    fclose(times);
    return 0;
}

void pcode (int aa, int ab, int ac){
	printf("start pl/0           stack\n");
    // Parametros
    h = 0; 
    t = -1; b = 0; p = 0; 
	code[ 0].f = 5;     code[ 0].l = 0;     code[ 0].a = 6;     //INT 0 6
    code[ 1].f = 0;     code[ 1].l = 0;     code[ 1].a = ab;     //LIT 0 2
	code[ 2].f = 3;     code[ 2].l = 0;     code[ 2].a = 3;     //STO 0 3
    code[ 3].f = 0;     code[ 3].l = 0;     code[ 3].a = ac;     //LIT 0 3 
	code[ 4].f = 3;     code[ 4].l = 0;     code[ 4].a = 4;     //STO 0 4
	code[ 5].f = 2;     code[ 5].l = 0;     code[ 5].a = 3;     //LOD 0 3
	code[ 6].f = 2;     code[ 6].l = 0;     code[ 6].a = 4;     //LOD 0 4
	code[ 7].f = 1;     code[ 7].l = 0;     code[ 7].a = 4;     //OPR 0 4 mul
	code[ 8].f = 3;     code[ 8].l = 0;     code[ 8].a = 5;     //STO 0 5
	code[ 9].f = 0;     code[ 9].l = 0;     code[ 9].a = aa;     //LIT 0 1
	code[10].f = 3;     code[10].l = 0;     code[10].a = 4;     //STO 0 4
	code[11].f = 2;     code[11].l = 0;     code[11].a = 4;     //LOD 0 4
	code[12].f = 2;     code[12].l = 0;     code[12].a = 5;     //LOD 0 5
	code[13].f = 1;     code[13].l = 0;     code[13].a = 2;     //OPR 0 2 add
	code[14].f = 3;     code[14].l = 0;     code[14].a = 5;     //STO 0 5
	code[15].f = 1;     code[15].l = 0;     code[15].a = 0;     //OPR 0 0 rtn
	//END 0 0

    printf("%3s %3s %3s %3s %3s ","f","l","a","t","p");
    for(h = 0; h <22; h++) {
        printf(" %3d" , h);
    }
    printf("\n");
    printf("--- --- --- --- --- ");
    for(h = 0; h <22; h++) {
        printf(" ---");
    }
    printf("\n");
    do {
		i = code[p];
		p++;
		switch (i.f) {
			// LIT, OPR, LOD, STO, CAL, INT, JMP, JPC
			// 0	1	 2	  3	   4	5	 6	  7	   
			case LIT:  t++; s[t] = i.a; break;
			case OPR:
				switch (i.a) { 
					// RTN, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
					// 0	1	 2	  3	   4	5	 6	  7    8	9	 10	  11   12	13
					case RTN: t = b - 1; p = s[t+3]; b = s[t+2]; break;
					case NEG: s[t]= -s[t]; break;
					case ADD: t--; s[t] += s[t+1]; break;
					case SUB: t--; s[t] -= s[t+1]; break;
					case MUL: t--; s[t] *= s[t+1]; break;
					case DIV: t--; s[t] /= s[t+1]; break;
					case MOD: t--; s[t] %= s[t+1]; break;
					case ODD: s[t] = (s[t]%2 == 1); break;
					case EQL: t--; s[t] = (s[t] == s[t+1]); break;
					case NEQ: t--; s[t] = (s[t] != s[t+1]); break;
					case LSS: t--; s[t] = (s[t] < s[t+1]); break;
					case LEQ: t--; s[t] = (s[t] <= s[t+1]); break;
					case GTR: t--; s[t] = (s[t] > s[t+1]); break;
					case GEQ: t--; s[t] = (s[t] >= s[t+1]); break;
					}
				break;
			case LOD: t++; s[t] = s[ base(i.l) + i.a ]; break;
			case STO: s[ base(i.l) + i.a ] = s[t]; t--; break;
			case CAL: s[t+1] = base( i.l ); s[t+2] = b; s[t+3] = p; b = t+1; p = i.a; break;
			case INT: t += i.a; break;
			case JMP: p = i.a-1; break; //CHECAR
			case JPC: if ( s[t] == 0 ) { p = i.a-1; } t--; break; //CHECAR
		}
			
		// PrintStack
		printf("%3s %3d %3d %3d %3d ", getInstructionName(i.f), i.l, i.a, s[t], p);

		for (h = 0; h < t+1; h++){
			printf(" %3d", s[h]);
		}
		printf("\n");
		
    } while (p != 0);
    printf("--- --- --- --- --- ");
    for(h = 0; h <22; h++) {
        printf(" ---");
    }
    printf("\n");
    printf("%3s %3s %3s %3s %3s ","f","l","a","t","p");
    for(h = 0; h <22; h++) {
        printf(" %3d" , h);
    }
}


int base(int l){
    int newBase;
	newBase = b;
	while (l>0){
		newBase = s[newBase];
		l--;
	}
	return newBase;
}

char * getInstructionName(int inst){
	return instructionString[inst];
}



