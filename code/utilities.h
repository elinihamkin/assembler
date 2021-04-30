/*
 * utilities.h
 *
 *      Author: eli
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "lineList.h"

void * allocateMemory(int size);
void freeMemory(void *toFree);
void printError(char *error, int lineNum);
void printWarning(char *worning, int lineNum);
void print(char *str);
char * convretBinaryToHexadecimal(char *binary);
char * convertCharToBinary(char ch);
char * convertIntToBinary(int num);
int convertStrToInt(lineNode *nodeLine, char *str);
int isletter(char ch);
int checkLettersAndNumbers(char *str);

#endif /* UTILITIES_H_ */
