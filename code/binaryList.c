/*
 * binaryList.c
 *
 *      Author: eli
 */
#include <stdio.h>

#include "binaryList.h"
#include "utilities.h"

/*add to binary list new node of binary data*/
binaryNode * insertBinary(binaryNode *binaryList, char *binary, int address, int ARE){
	binaryNode *node;
	binaryNode *newNode = allocateMemory(sizeof(binaryNode));
	newNode->binary = binary;
	newNode->address = address;
	newNode->ARE = ARE;
	newNode->next = NULL;
	newNode->previous = NULL;
	if(!binaryList){
		return newNode;
	}
	for(node = binaryList; node->next; node = node->next)
		;
	node->next = newNode;
	newNode->previous = node;
	return binaryList;
}

/*free the memory of the binary list*/
void freeBinaryList(binaryNode *binaryList){
	binaryNode *tmp;
	for(tmp = binaryList; tmp; tmp = binaryList){
		binaryList = binaryList->next;
		freeMemory(tmp->binary);
		freeMemory(tmp);
	}
}
