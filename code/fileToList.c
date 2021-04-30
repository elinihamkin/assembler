/*
 * fileToList.c
 *
 *      Author: eli
 */
#include <stdio.h>
#include <string.h>

#include "lineList.h"
#include "globalDefine.h"

#define LINE_OFFSET 2

/*file to list of lines that every line is list of words*/
lineNode * convertFileToList(FILE *file){
	lineNode *lineList = NULL , *tmpLineNode;
	wordNode *wordList = NULL;
	char line[MAX_LINE_LENGTH + LINE_OFFSET];
	int lineNum;
	for(lineNum = 1; fgets(line, MAX_LINE_LENGTH + LINE_OFFSET, file); ++lineNum){
		wordList = insertWords(line);
		if(!wordList){
			continue;
		}
		tmpLineNode = createLineNode(wordList, lineNum, FALSE);
		if(strlen(line) == MAX_LINE_LENGTH + LINE_OFFSET){
			tmpLineNode->lineError = TRUE;
			tmpLineNode->error = "line longer then allowed";
		}
		lineList = insertLine(lineList, tmpLineNode);
	}
	return lineList;
}
