/*
 * instructions.h
 *
 *      Author: eli
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

typedef const struct instructionNode{
	char *name;
	short opcode;
	short func;
	short numSourceOperand;
	const int *legalSourceOperand;
	short numTargetOperand;
	const int *legalTargetOperand;
} instructionNode;

instructionNode * serchInstruction(char *instruction);

#endif /* INSTRUCTIONS_H_ */
