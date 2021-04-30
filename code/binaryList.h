/*
 * binaryList.h
 *
 *      Author: eli
 */

#ifndef BINARYLIST_H_
#define BINARYLIST_H_

typedef struct binaryNode{
	char *binary;
	int address;
	char ARE;
	struct binaryNode *next;
	struct binaryNode *previous;
} binaryNode;

binaryNode * insertBinary(binaryNode *binaryList, char *binary, int address, int ARE);
void freeBinaryList(binaryNode *binaryList);

#endif /* BINARYLIST_H_ */
