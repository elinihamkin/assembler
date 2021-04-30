/*
 * symbolTable.c
 *
 *      Author: eli
 */
#include <string.h>

#include "symbolTable.h"
#include "utilities.h"

symbolNode * createNewNode(char *symbol, int address, short attribute);

/*insert symbol to the symbol table*/
symbolNode * insertSymbol(symbolNode *symbolTable, char *symbol, int address, short attribute){
	symbolNode *newNode = createNewNode(symbol, address, attribute);
	symbolNode *node;
	if(!symbolTable){
		return newNode;
	}
	for(node = symbolTable; node->next; node = node->next)
		;
	node->next = newNode;
	newNode->previous = node;
	return symbolTable;
}

/*create symbol node*/
symbolNode * createNewNode(char *symbol, int address, short attribute){
	symbolNode *newNode = allocateMemory(sizeof(symbolNode));
	newNode->symbol = allocateMemory(sizeof(symbol));
	strcpy(newNode->symbol, symbol);
	newNode->attribute = attribute;
	newNode->address = address;
	newNode->next = NULL;
	newNode->previous = NULL;
	return newNode;
}

/*search for symbol in the table*/
symbolNode * searchSymbol(symbolNode *symbolTable, char *symbol){
	symbolNode *node;
	for(node = symbolTable; node; node = node->next){
		if(!strcmp(node->symbol, symbol)){
			return node;
		}
	}
	return NULL;
}

/*free the memory of the table*/
void freeSymbolTable(symbolNode *symboltable){
	symbolNode *tmp;
	for(tmp = symboltable; tmp; tmp = symboltable){
		symboltable = symboltable->next;
		freeMemory(tmp->symbol);
		freeMemory(tmp);
	}
}
