/*
 * firstPass.c
 *
 *      Author: eli
 */
#include <string.h>
#include <stdlib.h>

#include "lineList.h"
#include "globalDefine.h"
#include "symbolTable.h"
#include "binaryList.h"
#include "utilities.h"
#include "instructions.h"

char * convertInstructionToBinary(short opcode, short func, short firstOperandAddressingMethod, short secondOperandAddressingMethod);
void checkWordRole(lineNode *nodeLine, int *IC, int *DC, symbolNode **symbolTable, binaryNode **codeBinaryList, binaryNode **dataBinaryList);
short checkInstruction(lineNode *nodeLine, wordNode *nodeWord, char *instruction, int *IC, symbolNode **symbolTable, binaryNode **codeBinaryList);
int checkInstructionOperands(lineNode *nodeLine, instructionNode *nodeInstruction, wordNode *nodeWord, int *IC, binaryNode **codeBinaryList);
char * convertInstructionToBinary(short opcode, short func, short firstOperandAddressingMethod, short secondOperandAddressingMethod);
int getAddressingMethod(char *operand);
int checkRightMethod(int AddressingMethod, const int * legalAddressingMethod, int size);
short checkDirective(lineNode *nodeLine, wordNode *nodeWord, char *directive, int *DC, symbolNode **symbolTable, binaryNode **dataBinaryList);
short checkExtern(lineNode *nodeLine, wordNode *nodeWord, symbolNode **symbolTable);
short checkString(lineNode *nodeLine, wordNode *nodeWord, int *DC, symbolNode **symbolTable, binaryNode **dataBinaryList);
short checkData(lineNode *nodeLine, wordNode *nodeWord, int *DC, symbolNode **symbolTable, binaryNode **dataBinaryList);
short checkLabel(lineNode *nodeLine, char *label);
int checkSavedWord(char *str);
int checkIfRegister(char *str);
char * convertRegisterToBinary(char *reg);

char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*starts the first pass check for existing line*/
void startFirstPass(lineNode *lineList, int *IC, int *DC, symbolNode **symbolTable, binaryNode **codeBinaryList, binaryNode **dataBinaryList){
	lineNode *nodeLine;
	for(nodeLine = lineList; nodeLine; nodeLine = nodeLine->next){
		if(!nodeLine->wordList){
			continue;
		}
		if(nodeLine->lineError){
			continue;
		}
		checkWordRole(nodeLine, IC, DC, symbolTable, codeBinaryList, dataBinaryList);
	}
}

/*check if the first word is label or directive or instruction and then sends to the right function*/
void checkWordRole(lineNode *nodeLine, int *IC, int *DC, symbolNode **symbolTable, binaryNode **codeBinaryList, binaryNode **dataBinaryList){
	wordNode *nodeWord;
	char *word;
	for(nodeWord = nodeLine->wordList; nodeWord; nodeWord = nodeWord->next){
		word = nodeWord->word;
		if(!word){
			if(nodeWord->end == COMMA){
				nodeLine->lineError = TRUE;
				nodeLine->error = "There is unnecessary comma in the start";
				return;
			}
		}
		if(word[strlen(word)-1] == COLON){
			word[strlen(word)-1] = END_STRING;
			nodeWord->role = checkLabel(nodeLine, word);
			if(nodeWord->role == null){
				return;
			}
			if(nodeWord->end == COMMA){
				nodeLine->lineError = TRUE;
				nodeLine->error = "No need for a comma after label";
				return;
			}
			continue;
		}
		if(*word == DOT){
			nodeWord->role = checkDirective(nodeLine, nodeWord, word, DC, symbolTable, dataBinaryList);
			if(nodeWord->role != null){
				if(nodeWord->end == COMMA){
					nodeLine->lineError = TRUE;
					nodeLine->error = "No need for a comma after directive";
				}
			}
			return;
		}
		nodeWord->role = checkInstruction(nodeLine, nodeWord, word, IC, symbolTable, codeBinaryList);
		return;
	}
}

/*check if the word is instruction and if it is check the operands*/
short checkInstruction(lineNode *nodeLine, wordNode *nodeWord, char *instruction, int *IC, symbolNode **symbolTable, binaryNode **codeBinaryList){
	symbolNode *nodeSym = NULL;
	instructionNode *nodeInstruction = serchInstruction(instruction);
	wordNode *tmp;
	int numImmediate;
	char *binary, *word;
	binaryNode *tmpBinaryNode;
	if(!nodeInstruction){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Instruction not exist";
		return null;
	}
	if(nodeWord->end == COMMA){
		nodeLine->lineError = TRUE;
		nodeLine->error = "No need for a comma after instruction";
		return null;
	}
	if(nodeWord->previous){
		tmp = nodeWord->previous;
		if(tmp->role == LABEL){
			nodeSym = searchSymbol(*symbolTable, tmp->word);
			if(nodeSym){
				if(nodeSym->attribute != external){
					nodeLine->lineError = TRUE;
					nodeLine->error = "There is symbol with the same name";
					return null;
				}
			}
			else{
				*symbolTable = insertSymbol(*symbolTable, tmp->word, *IC, code);
			}
		}
	}
	if(!checkInstructionOperands(nodeLine, nodeInstruction, nodeWord->next, IC, codeBinaryList)){
		return null;
	}
	for(nodeWord = nodeWord->next; nodeWord; nodeWord = nodeWord->next){
		if(nodeWord->role == IMMEDIATE){
			word = nodeWord->word;
			numImmediate = convertStrToInt(nodeLine, ++word);
			binary = convertIntToBinary(numImmediate);
			*codeBinaryList = insertBinary(*codeBinaryList, binary, *IC, A);
			++*IC;
		}
		else if(nodeWord->role == REGISTER){
			binary = convertRegisterToBinary(nodeWord->word);
			*codeBinaryList = insertBinary(*codeBinaryList, binary, *IC, A);
			++*IC;
		}
		else if(nodeWord->role == RELATIVE || nodeWord->role == DIRECT){
			*codeBinaryList = insertBinary(*codeBinaryList, NULL, *IC, A);
			++*IC;
			for(tmpBinaryNode = *codeBinaryList; tmpBinaryNode->next; tmpBinaryNode = tmpBinaryNode->next)
				;
			nodeWord->binary = tmpBinaryNode;
		}
	}
	return INSTRUCTION;
}

/*check the operand of the instruction*/
int checkInstructionOperands(lineNode *nodeLine, instructionNode *nodeInstruction, wordNode *nodeWord, int *IC, binaryNode **codeBinaryList){
	char *binary;
	int firstOperandAddressingMethod, secondOperandAddressingMethod;
	if(!nodeWord){
		if(!nodeInstruction->legalSourceOperand && !nodeInstruction->legalTargetOperand){
			binary = convertInstructionToBinary(nodeInstruction->opcode, nodeInstruction->func, 0, 0);
			*codeBinaryList = insertBinary(*codeBinaryList, binary, *IC, A);
			++*IC;
			return TRUE;
		}
		nodeLine->lineError = TRUE;
		nodeLine->error = "Missing operand";
		return FALSE;
	}
	firstOperandAddressingMethod = getAddressingMethod(nodeWord->word);
	if(firstOperandAddressingMethod < 0){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Cannot understand the first operand";
		return FALSE;
	}
	if(!nodeWord->next){
		if(nodeInstruction->legalSourceOperand){
			nodeLine->lineError = TRUE;
			nodeLine->error = "Missing operand";
			return FALSE;
		}
		if(nodeWord->end == COMMA){
			nodeLine->lineError = TRUE;
			nodeLine->error = "No need for a comma after operand";
			return FALSE;
		}
		if(!checkRightMethod(firstOperandAddressingMethod, nodeInstruction->legalTargetOperand, nodeInstruction->numTargetOperand)){
			nodeLine->lineError = TRUE;
			nodeLine->error = "Illegal target operand";
			return FALSE;
		}
		nodeWord->role = firstOperandAddressingMethod;
		binary = convertInstructionToBinary(nodeInstruction->opcode, nodeInstruction->func, 0, firstOperandAddressingMethod);
		*codeBinaryList = insertBinary(*codeBinaryList, binary, *IC, A);
		++*IC;
		return TRUE;
	}
	if(!checkRightMethod(firstOperandAddressingMethod, nodeInstruction->legalSourceOperand, nodeInstruction->numSourceOperand)){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Illegal source operand";
		return FALSE;
	}
	nodeWord->role = firstOperandAddressingMethod;
	if(nodeWord->end != COMMA){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There isn't comma between operands";
		return FALSE;
	}
	nodeWord = nodeWord->next;
	if(nodeWord->next){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is unnecessary data after the second operand";
		return FALSE;
	}
	if(nodeWord->end == COMMA){
		nodeLine->lineError = TRUE;
		nodeLine->error = "No need for a comma after second operand";
		return FALSE;
	}
	secondOperandAddressingMethod = getAddressingMethod(nodeWord->word);
	if(secondOperandAddressingMethod < 0){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Cannot understand the target operand";
		return FALSE;
	}
	if(!checkRightMethod(secondOperandAddressingMethod, nodeInstruction->legalTargetOperand, nodeInstruction->numTargetOperand)){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Illegal target operand";
		return FALSE;
	}
	nodeWord->role = secondOperandAddressingMethod;
	binary = convertInstructionToBinary(nodeInstruction->opcode, nodeInstruction->func, firstOperandAddressingMethod, secondOperandAddressingMethod);
	*codeBinaryList = insertBinary(*codeBinaryList, binary, *IC, A);
	++*IC;
	return TRUE;
}

/*convert the instruction line to binary*/
char * convertInstructionToBinary(short opcode, short func, short firstOperandAddressingMethod, short secondOperandAddressingMethod){
	char *binaryOpcode = convertIntToBinary(opcode);
	char *binaryFunc = convertIntToBinary(func);
	char *binaryFirstMethod = convertIntToBinary(firstOperandAddressingMethod);
	char *binarySecondMethod = convertIntToBinary(secondOperandAddressingMethod);
	char *binary = allocateMemory(SIZE_BINARY_WORD + 1);
	int i, j;
	for(i=0, j = SIZE_BINARY_WORD - SIZE_OPCOD; j<SIZE_BINARY_WORD; ++i, ++j){
		binary[i] = binaryOpcode[j];
	}
	for(j = SIZE_BINARY_WORD-SIZE_FUNC; j<SIZE_BINARY_WORD; ++i, ++j){
		binary[i] = binaryFunc[j];
	}
	for(j = SIZE_BINARY_WORD - SIZE_ADDRESING_METHOD; j<SIZE_BINARY_WORD; ++i, ++j){
		binary[i] = binaryFirstMethod[j];
	}
	for(j = SIZE_BINARY_WORD - SIZE_ADDRESING_METHOD; j<SIZE_BINARY_WORD; ++i, ++j){
		binary[i] = binarySecondMethod[j];
	}
	freeMemory(binaryOpcode);
	freeMemory(binaryFunc);
	freeMemory(binaryFirstMethod);
	freeMemory(binarySecondMethod);
	binary[SIZE_BINARY_WORD] = END_STRING;
	return binary;
}

/*find which addressing method the operand*/
int getAddressingMethod(char *operand){
	if(checkIfRegister(operand)){
		return REGISTER;
	}
	if(*operand == NUMBER_SIGN){
		return IMMEDIATE;
	}
	if(*operand == PERCENTAGE){
		return RELATIVE;
	}
	return DIRECT;
}

/*checks if the addressing method of the operand is right for the instraction*/
int checkRightMethod(int AddressingMethod, const int * legalAddressingMethod, int size){
	int i;
	for(i=0; i<size; ++i){
		if(AddressingMethod == legalAddressingMethod[i]){
			return TRUE;
		}
	}
	return FALSE;
}

/*check if the word is directive*/
short checkDirective(lineNode *nodeLine, wordNode *nodeWord, char *directive, int *DC, symbolNode **symbolTable, binaryNode **dataBinaryList){
	wordNode *tmp;
	if(!strcmp(directive, ".data")){
		return checkData(nodeLine, nodeWord, DC, symbolTable, dataBinaryList);
	}
	if(!strcmp(directive, ".string")){
		return checkString(nodeLine, nodeWord, DC, symbolTable, dataBinaryList);
	}
	if(!strcmp(directive, ".entry")){
		if(nodeWord->previous){
			tmp = nodeWord->previous;
			if(tmp->role == LABEL){
				nodeLine->error = "Label not required";
			}
		}
		return DOT_ENTRY;
	}
	if(!strcmp(directive, ".extern")){
		if(nodeWord->previous){
			tmp = nodeWord->previous;
			if(tmp->role == LABEL){
				nodeLine->error = "Label not required";
			}
		}
		return checkExtern(nodeLine, nodeWord, symbolTable);
	}
	nodeLine->lineError = TRUE;
	nodeLine->error = "Unknown directive";
	return null;
}

/*check the exstern*/
short checkExtern(lineNode *nodeLine, wordNode *nodeWord, symbolNode **symbolTable){
	symbolNode *nodeSym = NULL;
	if(!nodeWord->next){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is no symbol after extern";
		return null;
	}
	nodeWord = nodeWord->next;
	if(nodeWord->end == COMMA){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is unnecessary comma after the symbol";
		return null;
	}
	if(nodeWord->next){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is unnecessary data after the symbol";
		return null;
	}
	nodeSym = searchSymbol(*symbolTable, nodeWord->word);
	if(nodeSym){
		if(nodeSym->attribute != external)
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is local symbol with the same name";
		return null;
	}
	*symbolTable = insertSymbol(*symbolTable, nodeWord->word, 0, external);
	return DOT_EXTERN;
}

/*check the string*/
short checkString(lineNode *nodeLine, wordNode *nodeWord, int *DC, symbolNode **symbolTable, binaryNode **dataBinaryList){
	symbolNode *nodeSym = NULL;
	wordNode *tmp;
	char *str, *binary;
	if(nodeWord->previous){
		tmp = nodeWord->previous;
		if(tmp->role == LABEL){
			nodeSym = searchSymbol(*symbolTable, tmp->word);
			if(nodeSym){
				if(nodeSym->attribute != external)
				nodeLine->lineError = TRUE;
				nodeLine->error = "There is symbol with the same name";
				return null;
			}
			*symbolTable = insertSymbol(*symbolTable, tmp->word, *DC, data);
		}
	}
	if(!nodeWord->next){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is no string";
		return null;
	}
	nodeWord = nodeWord->next;
	str = nodeWord->word;
	if(nodeWord->end != QUOTATION_MARK || *str != QUOTATION_MARK){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is problem with the quotation";
		return null;
	}
	if(!nodeWord->next){
		nodeLine->lineError = TRUE;
		nodeWord = nodeWord->next;
		if(!nodeWord->word){
			if(!(nodeWord->end == COMMA)){
				nodeLine->error = "There is unnecessary comma after the string";
			}
		}
		nodeLine->error = "There is unnecessary data after the string";
		return null;
	}
	for(++str; *str; ++str){
		binary = convertCharToBinary(*str);
		*dataBinaryList = insertBinary(*dataBinaryList, binary, *DC, A);
		++*DC;
	}
	binary = convertCharToBinary(END_STRING);
	*dataBinaryList = insertBinary(*dataBinaryList, binary, *DC, A);
	++*DC;
	return DOT_STRING;
}

/*check the data*/
short checkData(lineNode *nodeLine, wordNode *nodeWord, int *DC, symbolNode **symbolTable, binaryNode **dataBinaryList){
	symbolNode *nodeSym = NULL;
	wordNode *tmp;
	int num;
	char *binary;
	if(nodeWord->previous){
		tmp = nodeWord->previous;
		if(tmp->role == LABEL){
			nodeSym = searchSymbol(*symbolTable,  tmp->word);
			if(nodeSym){
				if(nodeSym->attribute != external)
				nodeLine->lineError = TRUE;
				nodeLine->error = "There is symbol with the same name";
				return null;
			}
			*symbolTable = insertSymbol(*symbolTable, tmp->word, *DC, data);
		}
	}
	if(!nodeWord->next){
		nodeLine->lineError = TRUE;
		nodeLine->error = "There is no data";
		return null;
	}
	for(nodeWord = nodeWord->next; nodeWord; nodeWord = nodeWord->next){
		if(!nodeWord->next){
			if(nodeWord->end == COMMA){
				nodeLine->lineError = TRUE;
				nodeLine->error = "There is an unnecessary comma after the last data";
				return null;
			}
		}
		else{
			if(nodeWord->end != COMMA){
				nodeLine->lineError = TRUE;
				nodeLine->error = "There is a missing comma after data";
				return null;
			}
		}
		if(!nodeWord->word){
			nodeLine->lineError = TRUE;
			nodeLine->error = "There is an redundant comma after data";
			return null;
		}
		num = convertStrToInt(nodeLine, nodeWord->word);
		if(nodeLine->lineError){
			return null;
		}
		binary = convertIntToBinary(num);
		*dataBinaryList = insertBinary(*dataBinaryList, binary, *DC, A);
		++*DC;
	}
	return DOT_DATA;
}

/*check the label*/
short checkLabel(lineNode *nodeLine, char *label){
	if(strlen(label) == MAX_LABEL_LENGTH - 1){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Label is to long";
		return null;
	}
	if(!isletter(*label)){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Label need to start with letter";
		return null;
	}
	if(checkLettersAndNumbers(label)){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Label can only contain letters and numbers";
		return null;
	}
	if(checkSavedWord(label)){
		nodeLine->lineError = TRUE;
		nodeLine->error = "Label can not be saved word";
		return null;
	}
	return LABEL;
}

/*check if the word is saved word*/
int checkSavedWord(char *str){
	instructionNode *instruction = serchInstruction(str);
	if(checkIfRegister(str) || instruction){
		return TRUE;
	}
	return FALSE;
}

/*check if word is register*/
int checkIfRegister(char *str){
	int i;
	for(i=0; i<NUM_OF_REGISTERS; ++i){
		if(!strcmp(registers[i], str)){
			return TRUE;
		}
	}
	return FALSE;
}

/*converts register to binary*/
char * convertRegisterToBinary(char *reg){
	char *binary;
	int num;
	++reg;
	num = atoi(reg) +1;
	binary = convertIntToBinary(0);
	binary[SIZE_BINARY_WORD - num] = '1';
	return binary;
}
