

#ifndef DEFENITIONS_H
#define DEFENITIONS_H

#define MAX_FILE_NAME_L 100 /* maximum file name length */
#define MAX_LINE_LENGTH 80 /* maximum line length of the input file */
#define LABEL_MAX_LENGTH 30 /* maximum label name length */
#define COMMAND_MAX_LENGTH 4 /* maximum file name length */
#define MAX_ARRAY 1000 /* maximum length for instructions and data tables */
#define COMMANDS_AMOUNT 16
#define REGISTER_LENGTH 3 /* register string length */
#define ADD_SPECIAL_LENGTH_1 1 /* the length of a special 8 address with 1 char */
#define ADD_SPECIAL_LENGTH_2 2 /* the length of a special 8 address with 2 chars */
#define ADD_SPECIAL_LENGTH_3 3 /* the length of a special 8 address with 3 chars */
#define ADD_SPECIAL_LENGTH_4 4 /* the length of a special 8 address with 4 chars */
#define INS_SPECIAL_LENGTH 5 /* the length of a special 8 instruction code with 5 chars */
#define MAX_SPECIAL_LENGTH 6 /* maximum allowed special 8 string */
#define EIGHT_POWER_ONE 8 /* 8 to the first power - uses one char in an octal number*/
#define EIGHT_POWER_TWO 64 /* 8 to the second power  - uses two chars in an octal number*/
#define EIGHT_POWER_THREE 512 /* 8 to the 3rd power  - uses three char in an octal number*/

#define FIRST_INSTRUCTION_ADDRESS 100 /* the first address in the memory for the first instruction */
#define AS_EXTENSION ".as" /* .as extension for input file */

/* general definitions */
typedef enum {FALSE, TRUE} boolean; /* create boolean type */
typedef enum {COMMENT,LABEL,DATA,STRING,EXTERN,ENTRY,COMMAND,UNKNOWN,EMPTY}argumentType; /* arguments types for line recognition */
enum EXIT {EXIT_SUCCEED, EXIT_FAIL}; /* exit codes */

/* definitions for op codes structures */
enum ERA {ERA_ABSOLUTE, ERA_EXTERNAL, ERA_RELOCATABLE};
enum GROUP {ZERO_OPERANDS, ONE_OPERAND, TWO_OPERANDS};
enum ACCESS {NO_OPERAND = -1, IMMEDIATE_ACCESS, DIRECT_ACCESS, DYN_IMMEDIATE_ACCESS, REGISTER_DIRECT_ACCESS,UNKNOWN_TYPE = 4}; /* operand recognition */
enum UNUSED {UNUSED_0=0,UNUSED_101=5}; /* for unused bits in binary data structures */

#endif
