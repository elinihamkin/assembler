/*
 * errorPass.c
 *
 *      Author: eli
 */
#include "utilities.h"
#include "lineList.h"
#include "globalDefine.h"

/*checks if there is error or warning in one of the lines and print them*/
int checkForErrors(lineNode *lineList){
	int fileError = FALSE;
	lineNode *tmp;
	for(tmp = lineList; tmp; tmp = tmp->next){
		if(tmp->lineError){
			fileError = TRUE;
			printError(tmp->error, tmp->lineNum);
			continue;
		}
		if(tmp->error){
			printWarning(tmp->error, tmp->lineNum);
		}
	}
	return fileError;
}
