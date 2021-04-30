/*
 * lineList.h
 *
 *      Author: eli
 */

#ifndef LINELIST_H_
#define LINELIST_H_

#include "wordList.h"

typedef struct lineNode{
	wordNode *wordList;
	int lineNum;
	short lineError;
	char *error;
	struct lineNode *next;
	struct lineNode *previous;
} lineNode;

lineNode * insertLine(lineNode *lineList, lineNode *line);
lineNode * createLineNode(wordNode *wordList, int lineNum, short lineError);
void freeLineList(lineNode *lineList);

#endif /* LINELIST_H_ */
