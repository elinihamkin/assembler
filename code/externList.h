/*
 * externList.h
 *
 *      Author: eli
 */

#ifndef EXTERNLIST_H_
#define EXTERNLIST_H_

typedef struct externNode{
	char *ext;
	int address;
	struct externNode *next;
} externNode;

externNode * insertExtern(externNode *externList, char *ext, int address);
void freeExternList(externNode *externList);

#endif /* EXTERNLIST_H_ */
