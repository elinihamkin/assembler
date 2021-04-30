/*
 * wordList.c
 *
 *      Author: eli
 */
#include <string.h>

#include "wordList.h"
#include "utilities.h"
#include "globalDefine.h"

void freeNode(wordNode *node);
char * getNextStr(char **line);

/*convert line to list of words*/
wordNode * insertWords(char *line){
	wordNode *head = NULL, *tail = NULL, *tmp;
	while(*line){
		tmp = allocateMemory(sizeof(wordNode));
		tmp->word = getNextStr(&line);
		tmp->end = *line;
		tmp->role = null;
		tmp->binary = NULL;
		tmp->next = NULL;
		tmp->previous = NULL;
		if(*line){
			++line;
		}
		tmp->next = NULL;
		if(!*tmp->word && (tmp->end == SPACE || tmp->end == TAB || !tmp->end)){
			freeNode(tmp);
			continue;
		}
		if(*tmp->word == SEMICOLON){
			freeNode(tmp);
			return NULL;
		}
		if(!head){
			head = tmp;
			tail = tmp;
		}
		else{
			if(tail->end != COMMA && !*tmp->word && tmp->end == COMMA){
				tail->end = COMMA;
				freeNode(tmp);
				continue;
			}
			tail->next = tmp;
			tmp->previous = tail;
			tail = tail->next;
		}
	}
	if(tail->end != COMMA &&  tail->end != QUOTATION_MARK){
		tail->end = END_STRING;
	}
	return head;
}

/*get the next word from the line*/
char * getNextStr(char **line){
	char tmpStr[MAX_LINE_LENGTH];
	char *ptmp = tmpStr;
	char *str;
	for(; **line!=SPACE && **line != TAB && **line != COMMA && **line != NEW_LINE && **line != END_STRING; ++*line, ++ptmp){
		/*get string from the line*/
		if(**line == QUOTATION_MARK){
			*ptmp = **line;
			for(++*line, ++ptmp; **line != QUOTATION_MARK && **line != END_STRING ; ++*line, ++ptmp){
				*ptmp = **line;
			}
			*ptmp = END_STRING;
			str = allocateMemory(strlen(tmpStr)+1);
			strcpy(str, tmpStr);
			return str;
		}
		*ptmp = **line;
	}
	*ptmp = END_STRING;
	str = allocateMemory(strlen(tmpStr)+1);
	strcpy(str, tmpStr);
	return str;
}

/*free the memory of the eord list*/
void freeWordList(wordNode *head){
	wordNode *tmp;
	while(head){
		tmp = head;
		head = head->next;
		freeNode(tmp);
	}
}

/*free the memory of node*/
void freeNode(wordNode *node){
	freeMemory(node->word);
	freeMemory(node);
}
