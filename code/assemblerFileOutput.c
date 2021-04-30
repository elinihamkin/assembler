/*
 * assemblerFileOutput.c
 *
 *      Author: eli
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "binaryList.h"
#include "symbolTable.h"
#include "globalDefine.h"
#include "utilities.h"
#include "externList.h"

void createObFile(char *fileName, binaryNode *binaryList, int ICF, int DCF);
void createExtFile(char *fileName, externNode *externList);
void createEntFile(char *fileName, symbolNode *symbolTable);
void removeFile(char *fileName, char *extension);
FILE * openFile(char *fileName, char *extension);

/*main function that ccalls the functions that creates the output files*/
void createOutputFiles(char *fileName, binaryNode *binaryList, symbolNode *symbolTable, externNode *externLiest, int ICF, int DCF){
	createObFile(fileName, binaryList, ICF, DCF);
	createEntFile(fileName, symbolTable);
	createExtFile(fileName, externLiest);
}

/*create object file*/
void createObFile(char *fileName, binaryNode *binaryList, int ICF, int DCF){
	FILE *fileob;
	char *hex;
	if(ICF == 0 && DCF == 0){
		return;
	}
	fileob = openFile(fileName, ".ob");
	if(!fileob){
		print("FATAL ERROR: The file .ob could not be opend");
		exit(1);
	}
	fprintf(fileob, "%d %d\n", ICF, DCF);
	for(; binaryList; binaryList = binaryList->next){
		hex = convretBinaryToHexadecimal(binaryList->binary);
		fprintf(fileob, "%04d %s", binaryList->address, hex);
		if(binaryList->ARE == A){
			fprintf(fileob, " A\n");
		}
		else if(binaryList->ARE == R){
			fprintf(fileob, " R\n");
		}
		else{
			fprintf(fileob, " E\n");
		}
		freeMemory(hex);
	}
	fclose(fileob);

}

/*create externals file*/
void createExtFile(char *fileName, externNode *externList){
	FILE *fileext = openFile(fileName, ".ext");
	if(!fileext){
		print("FATAL ERROR: The file .ext could not be opend");
		exit(1);
	}
	if(!externList){
		removeFile(fileName, ".ext");
	}
	for(; externList; externList = externList->next){
		fprintf(fileext, "%s %04d\n", externList->ext, externList->address);
	}
	fclose(fileext);
}

/*create entries file*/
void createEntFile(char *fileName, symbolNode *symbolTable){
	FILE *fileent = openFile(fileName, ".ent");
	int fileEmty = FALSE;
	if(!fileent){
		print("FATAL ERROR: The file .ent could not be opend");
		exit(1);
	}
	for(; symbolTable; symbolTable = symbolTable->next){
		if(symbolTable->attribute == DOT_ENTRY){
			fprintf(fileent, "%s %04d\n", symbolTable->symbol, symbolTable->address);
			fileEmty = TRUE;
		}
	}
	if(!fileEmty){
		removeFile(fileName, ".ent");
	}
	fclose(fileent);
}

/*removes file if its not needed*/
void removeFile(char *fileName, char *extension){
	char *fullName = allocateMemory(MAX_FILE_NAME + strlen(extension));
	strcpy(fullName, fileName);
	strcat(fullName, extension);
	remove(fullName);
	freeMemory(fullName);
}

/*opens file for writing bay name and extension*/
FILE * openFile(char *fileName, char *extension){
	FILE *file;
	char *fullName = allocateMemory(MAX_FILE_NAME + strlen(extension));
	strcpy(fullName, fileName);
	strcat(fullName, extension);
	file = fopen(fullName, "w");
	freeMemory(fullName);
	return file;
}
