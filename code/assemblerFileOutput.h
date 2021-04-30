/*
 * assemblerFileOutput.h
 *
 *      Author: eli
 */

#ifndef ASSEMBLERFILEOUTPUT_H_
#define ASSEMBLERFILEOUTPUT_H_

void createOutputFiles(char *fileName, binaryNode *binaryList, symbolNode *symbolTable, externNode *externLiest, int ICF, int DCF);

#endif /* ASSEMBLERFILEOUTPUT_H_ */
