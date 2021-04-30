/*
 * instructions.c
 *
 *      Author: eli
 */
#include <string.h>

#include "instructions.h"
#include "globalDefine.h"

const int op1[] = {IMMEDIATE, DIRECT, REGISTER};
const int op2[] = {DIRECT, REGISTER};
const int op3[] = {DIRECT, RELATIVE};
const int op4[] = {DIRECT};

instructionNode move = {"mov", 0, 0, 3, op1, 2, op2};
instructionNode cmp = {"cmp", 1, 0, 3, op1, 2, op1};
instructionNode add = {"add", 2, 10, 3, op1, 2, op2};
instructionNode sub = {"sub", 2, 11, 3, op1, 2 ,op2};
instructionNode lea = {"lea", 4, 0, 1, op4, 2, op2};
instructionNode clr = {"clr", 5, 10, 0, NULL, 2, op2};
instructionNode not = {"not", 5, 11, 0, NULL, 2, op2};
instructionNode inc = {"inc", 5, 12, 0, NULL, 2, op2};
instructionNode dec = {"dec", 5, 13, 0, NULL, 2, op2};
instructionNode jmp = {"jmp", 9, 10, 0, NULL, 2, op3};
instructionNode bne = {"bne", 9, 11, 0, NULL, 2, op3};
instructionNode jsr = {"jsr", 9, 12, 0, NULL, 2, op3};
instructionNode red = {"red", 12, 0, 0, NULL, 2, op2};
instructionNode prn = {"prn", 13, 0, 0, NULL, 3, op1};
instructionNode rts = {"rts", 14, 0, 0, NULL, 0, NULL};
instructionNode stop = {"stop", 15, 0, 0, NULL, 0, NULL};

instructionNode *instructionArray[] = {&move, &cmp, &add, &sub, &lea, &clr, &not, &inc, &dec, &jmp, &bne, &jsr, &red, &prn, &rts, &stop};

/*return the instruction node that looked for*/
instructionNode * serchInstruction(char *instruction){
	instructionNode *node;
	int i;
	for(i=0; i<NUM_OF_INSTRUCTIONS; ++i){
		node = instructionArray[i];
		if(!strcmp(node->name, instruction)){
			return node;
		}
	}
	return NULL;
}
