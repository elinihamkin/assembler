/*
 * utilities.c
 *
 *      Author: eli
 */

#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"
#include "globalDefine.h"

char binaryToHex(char *binary);

/*allocate memory for given size*/
void * allocateMemory(int size){
	void *memory = malloc(size);
	if(!memory){
		print("FATAL ERROR: memory allocation error");
		exit(EXIT_FAILURE);
	}
	return memory;
}

/*free the memory of given pointer*/
void freeMemory(void *toFree){
	free(toFree);
}

/*print full error message*/
void printError(char *error, int lineNum){
	printf("ERROR: line %d: %s\n", lineNum, error);
}

/*print full warning message*/
void printWarning(char *worning, int lineNum){
	printf("WORNING: line %d: %s\n", lineNum, worning);
}

/*print string*/
void print(char *str){
	printf("%s\n", str);
}

/*convert binary word to hexadecimal word*/
char * convretBinaryToHexadecimal(char *binary){
	char *hex = allocateMemory(SIZE_BINARY_WORD/4 + 1);
	char tmp[(SIZE_BINARY_WORD/3)+1];
	int i,j;
	for(i=0; i<SIZE_BINARY_WORD/4; ++i){
		for(j=0; j<SIZE_BINARY_WORD/3; ++j){
			tmp[j] = binary[i*4+j];
		}
		tmp[4] = END_STRING;
		hex[i] = binaryToHex(tmp);
	}
	hex[3] = END_STRING;
	return hex;
}

/*convert binary word to hexadecimal word*/
char binaryToHex(char *binary){
	char hex[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	int num = (int)strtol(binary, NULL, 2);
	return hex[num];
}

/*convert char to binary word */
char * convertCharToBinary(char ch){
	char *binary = allocateMemory(SIZE_BINARY_WORD + 1);
	char mask;
	int i;
	for(i=SIZE_BINARY_WORD -1, mask = ch; i>=0; --i, mask>>=1){
		binary[i] = (mask & 1) + '0';
	}
	binary[SIZE_BINARY_WORD] = END_STRING;
	return binary;
}

/*convert number decimal number to binary*/
char * convertIntToBinary(int num){
	char *binary = allocateMemory(SIZE_BINARY_WORD + 1);
	int mask;
	int i;
	for(i=SIZE_BINARY_WORD-1, mask = num; i>=0; --i, mask>>=1){
		binary[i] = (mask & 1) + '0';
	}
	binary[SIZE_BINARY_WORD] = END_STRING;
	return binary;
}

/*convert string to decimal number*/
int convertStrToInt(lineNode *nodeLine, char *str){
	char *tmp = str;
	if(*tmp == '+' || *tmp == '-'){
		++tmp;
	}
	for(; *tmp; ++tmp){
		if(*tmp >= '0' && *tmp <= '9'){
			continue;
		}
		else{
			nodeLine->lineError = TRUE;
			nodeLine->error = "There is data that isn't number";
			return FALSE;
		}
	}
	return atoi(str);
}

/*check if word is only from letters and numbers*/
int checkLettersAndNumbers(char *str){
	for(; str; ++str){
		if((*str >= 'a' && *str <='z') || (*str >= 'A' && *str <= 'Z') || (*str >= '0' && *str <= '9')){
			continue;
		}
		return FALSE;
	}
	return TRUE;
}

/*check if char is letter*/
int isletter(char ch){
	if((ch >= 'a' && ch <='z') || (ch >= 'A' && ch <= 'Z')){
		return TRUE;
	}
	return FALSE;
}

