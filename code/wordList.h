/*
 * wordList.h
 *
 *      Author: eli
 */

#ifndef WORDLIST_H_
#define WORDLIST_H_

#include "binaryList.h"

typedef struct wordNode{
	char *word;
	char end;
	short role;
	binaryNode *binary;
	struct wordNode *next;
	struct wordNode *previous;
} wordNode;

wordNode * insertWords(char *line);
void freeWordList(wordNode *head);

#endif /* WORDLIST_H_ */
