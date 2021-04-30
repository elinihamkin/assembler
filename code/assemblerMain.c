/*
 * assemblerMain.c
 *
 *      Author: eli
 */
#include <stdio.h>
#include <string.h>

#include "symbolTable.h"
#include "lineList.h"
#include "globalDefine.h"
#include "binaryList.h"
#include "firstPass.h"
#include "externList.h"
#include "secondPass.h"
#include "fileToList.h"
#include "errorPass.h"
#include "utilities.h"
#include "assemblerFileOutput.h"

FILE * getFile(char *fileName);
binaryNode * combineBinaryList(binaryNode *codeBinaryList, binaryNode *dataBinaryList);
void updateSymbolTable(symbolNode *symbolTable, int ICF);
void freeAndExitAll(FILE *file, lineNode *lineList, symbolNode *symbolTable, binaryNode *binaryList, externNode *externList);

/*main assembler controls on all the functions and data structures for specific file name*/
void startAssembler(char *fileName){
	FILE *file = NULL;
	lineNode *lineList = NULL;
	symbolNode *symbolTable = NULL;
	binaryNode *codeBinaryList = NULL, *dataBinaryList = NULL, *binaryList = NULL;
	externNode *externList = NULL;
	int IC = BASE_ADDRESS, DC = 1;
	print("Start to work on:");
	print(fileName);
	file = getFile(fileName);
	if(!file){
		print("Error: The file did not open");
		return;
	}
	lineList = convertFileToList(file);
	startFirstPass(lineList, &IC, &DC, &symbolTable, &codeBinaryList, &dataBinaryList);
	binaryList = combineBinaryList(codeBinaryList, dataBinaryList);
	if(checkForErrors(lineList)){
		print("Work on the file ended with errors\n");
		freeAndExitAll(file, lineList, symbolTable, binaryList, externList);
		return;
	}
	updateSymbolTable(symbolTable, IC);
	startSecondPass(lineList, symbolTable, &externList);
	if(checkForErrors(lineList)){
		print("Work on the file ended with errors\n");
		freeAndExitAll(file, lineList, symbolTable, binaryList, externList);
		return;
	}
	createOutputFiles(fileName, binaryList, symbolTable, externList, IC-BASE_ADDRESS, DC);
	print("Work on the file has been successfully completed");
	freeAndExitAll(file, lineList, symbolTable, binaryList, externList);
}

/*free the memory of all the data structures of the program*/
void freeAndExitAll(FILE *file, lineNode *lineList, symbolNode *symbolTable, binaryNode *binaryList, externNode *externList){
	fclose(file);
	freeLineList(lineList);
	freeSymbolTable(symbolTable);
	freeBinaryList(binaryList);
	freeExternList(externList);
}

/*get the code file for the assembler by name*/
FILE * getFile(char *fileName){
	FILE *file;
	char fullFileName[MAX_FILE_NAME + 3];
	strcpy(fullFileName, fileName);
	strcat(fullFileName, AS_ENDING);
	file = fopen(fullFileName, "r");
	return file;
}

/*update the address of symbols after the first pass*/
void updateSymbolTable(symbolNode *symbolTable, int ICF){
	symbolNode *node;
	for(node = symbolTable; node; node = node->next){
		if(node->attribute == data){
			node->address = node->address + ICF;
		}
	}
}

/*combines the code binary list and the data binary list and updates the address to the data*/
binaryNode * combineBinaryList(binaryNode *codeBinaryList, binaryNode *dataBinaryList){
	binaryNode *binaryList = codeBinaryList;
	int lastCodeAddress;
	for(; codeBinaryList && codeBinaryList->next; codeBinaryList = codeBinaryList->next)
		;
	if(!codeBinaryList){
		lastCodeAddress = 0;
		binaryList = dataBinaryList;
	}
	else{
		lastCodeAddress = codeBinaryList->address;
		codeBinaryList->next = dataBinaryList;
		dataBinaryList->previous = codeBinaryList;
	}
	for(; dataBinaryList; dataBinaryList = dataBinaryList->next){
		dataBinaryList->address = dataBinaryList->address + lastCodeAddress;
	}
	return binaryList;
}
