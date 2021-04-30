/*
 * externList.c
 *
 *      Author: eli
 */
#include <string.h>

#include "externList.h"
#include "utilities.h"

/*insert extern symbol and the address to the list*/
externNode * insertExtern(externNode *externList, char *ext, int address){
	externNode *newNode = allocateMemory(sizeof(externNode));
	externNode *tmp;
	newNode->ext = allocateMemory(strlen(ext)+1);
	strcpy(newNode->ext, ext);
	newNode->address = address;
	newNode->next = NULL;
	if(!externList){
		return newNode;
	}
	for(tmp = externList; tmp->next; tmp = tmp->next)
		;
	tmp->next = newNode;
	return externList;
}

/*free the memory of the list*/
void freeExternList(externNode *externList){
	externNode *tmp;
	for(tmp = externList; tmp; tmp = externList){
		externList = externList->next;
		freeMemory(tmp->ext);
		freeMemory(tmp);
	}
}

