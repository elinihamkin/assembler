/*
 * main.c
 *
 *      Author: eli
 */
#include <string.h>
#include <stdlib.h>

#include "assemblerMain.h"
#include "utilities.h"

void fileForAssembler(int numOfFiles, char **filesNames);

int main(int argc, char *argv[]){
	fileForAssembler(argc-1, argv+1);
	return 0;
}

/*call the assembler for specific file name*/
void fileForAssembler(int numOfFiles, char **filesNames){
	int i;
	if(!numOfFiles){
		print("WARNING: There is no given file name\n");
		exit(0);
	}
	for(i=0; i<numOfFiles; ++i, ++filesNames){
		startAssembler(*filesNames);
	}
}
