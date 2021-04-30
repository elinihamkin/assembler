/*
 * globalDefine.h
 *
 *      Author: eli
 */

#ifndef GLOBALDEFINE_H_
#define GLOBALDEFINE_H_

enum boolean {FALSE, TRUE};
enum wordRole {LABEL=5, DOT_DATA, DOT_STRING, DOT_ENTRY, DOT_EXTERN, INSTRUCTION, SYMBOL, INTEGER, STRING, IMMEDIATE=0, DIRECT, RELATIVE, REGISTER, null};
enum attributes {data, code, external};
enum ARE {A, R, E};

#define MAX_FILE_NAME 100
#define BASE_ADDRESS 100
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 31
#define SIZE_BINARY_WORD 12
#define NUM_OF_INSTRUCTIONS 16
#define NUM_OF_REGISTERS 8
#define SIZE_OPCOD 4
#define SIZE_FUNC 4
#define SIZE_ADDRESING_METHOD 2

#define AS_ENDING ".as"
#define EXT_ENDING ".ext"
#define ENT_ENDING ".ent"

#define COMMA ','
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'
#define END_STRING '\0'
#define QUOTATION_MARK '"'
#define COLON ':'
#define DOT '.'
#define NUMBER_SIGN '#'
#define SEMICOLON ';'
#define PERCENTAGE '%'

#endif /* GLOBALDEFINE_H_ */
