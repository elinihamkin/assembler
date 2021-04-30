/*
 * lineList.c
 *
 *      Author: eli
 */
#include <stdio.h>

#include "lineList.h"
#include "utilities.h"
#include "globalDefine.h"

/*inserts line to the list*/
lineNode * insertLine(lineNode *lineList, lineNode *line){
	lineNode *node;
	if(!lineList){
		return line;
	}
	for(node = lineList; node->next; node = node->next)
		;
	node->next = line;
	line->previous = node;
	return lineList;
}

/*create node of line*/
lineNode * createLineNode(wordNode *wordList, int lineNum, short lineError){
	lineNode *newNode = allocateMemory(sizeof(lineNode));
	newNode->wordList = wordList;
	newNode->lineNum = lineNum;
	newNode->lineError = lineError;
	newNode->error = NULL;
	newNode->next = NULL;
	newNode->previous = NULL;
	return newNode;
}

/*free the memory of the list*/
void freeLineList(lineNode *lineList){
	lineNode *tmp;
	for(tmp = lineList; tmp; tmp = lineList){
		lineList = lineList->next;
		freeWordList(tmp->wordList);
		freeMemory(tmp);
	}
}
