/*
 * symbolTable.h
 *
 *      Author: eli
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

typedef struct symbolNode{
	char *symbol;
	int address;
	short attribute;
	struct symbolNode *next;
	struct symbolNode *previous;
} symbolNode;

symbolNode * insertSymbol(symbolNode *symbolTable, char *symbol, int address, short attribute);
symbolNode * searchSymbol(symbolNode *symbolTable, char *symbol);
void freeSymbolTable(symbolNode *symboltable);

#endif /* SYMBOLTABLE_H_ */
