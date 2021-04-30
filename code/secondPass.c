/*
 * secondPass.c
 *
 *      Author: eli
 */
#include "globalDefine.h"
#include "symbolTable.h"
#include "utilities.h"
#include "externList.h"

void checkEntry(lineNode *lineList, symbolNode *symbolTable, wordNode *word);
void checkDirect(lineNode *lineList, symbolNode *symbolTable, wordNode *word, externNode **externList);
void checkRelative(lineNode *lineList, symbolNode *symbolTable, wordNode *word, externNode **externList);

/*start the second pass and find the specific word that need to check */
void startSecondPass(lineNode *lineList, symbolNode *symbolTable, externNode **externList){
	lineNode *nodeLine;
	wordNode *word;
	for(nodeLine = lineList; nodeLine; nodeLine = nodeLine->next){
		for(word = nodeLine->wordList; word; word = word->next){
			if(word->role == DOT_ENTRY){
				checkEntry(lineList, symbolTable, word->next);
			}
			if(word->role == DIRECT){
				checkDirect(lineList, symbolTable, word, externList);
			}
			if(word->role == RELATIVE){
				checkRelative(lineList, symbolTable, word, externList);
			}

		}
	}
}

/*check relative addressing method and update the binary code*/
void checkRelative(lineNode *lineList, symbolNode *symbolTable, wordNode *word, externNode **externList){
	char *wordtmp = word->word;
	symbolNode *symbol = searchSymbol(symbolTable, ++wordtmp);
	char *binary;
	if(!symbol){
		lineList->lineError = TRUE;
		lineList->error = "Symbol not exist 2";
		return;
	}
	binary = convertIntToBinary(symbol->address - (word->binary->address + 1));
	word->binary->binary = binary;
	if(symbol->attribute == external){
		word->binary->ARE = E;
		*externList = insertExtern(*externList, symbol->symbol, word->binary->address);
	}
	else{
		word->binary->ARE = R;
	}
}

/*check direct addressing method and update the binary code*/
void checkDirect(lineNode *lineList, symbolNode *symbolTable, wordNode *word, externNode **externList){
	symbolNode *symbol = searchSymbol(symbolTable, word->word);
	char *binary;
	if(!symbol){
		lineList->lineError = TRUE;
		lineList->error = "Symbol not exist";
		return;
	}
	binary = convertIntToBinary(symbol->address);
	word->binary->binary = binary;
	if(symbol->attribute == external){
		word->binary->ARE = E;
		*externList = insertExtern(*externList, symbol->symbol, word->binary->address);
	}
	else{
		word->binary->ARE = R;
	}
}

/*check entry method and update the symbol table*/
void checkEntry(lineNode *lineList, symbolNode *symbolTable, wordNode *word){
	symbolNode *symbol;
	if(!word){
		lineList->lineError = TRUE;
		lineList->error = "There is no symbol after entry";
		return;
	}
	if(word->end == COMMA){
		lineList->lineError = TRUE;
		lineList->error = "There is comma after the symbol";
		return;
	}
	if(word->next){
		lineList->lineError = TRUE;
		lineList->error = "There is more data after symbol";
		return;
	}
	symbol = searchSymbol(symbolTable, word->word);
	if(!symbol){
		lineList->lineError = TRUE;
		lineList->error = "Symbol not exist";
		return;
	}
	if(symbol->attribute == external){
		lineList->lineError = TRUE;
		lineList->error = "Symbol is external";
		return;
	}
	symbol->attribute = DOT_ENTRY;
}
