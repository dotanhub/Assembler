
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "validations.h"
#include "defenitions.h"
#include "fileswriting.h"
#include "datastructures.h"

extern int lineNumber; /* current line in file */
extern int instructionsTable[MAX_ARRAY]; /* saves the machine code in an array assuming maximum size for the array */
extern boolean secondPass; /*if FALSE it is the first pass, if TRUE it is the second pass */
extern int symTableCounter; /* counter for the symbol table */

void import_from_file(char filePath[]); /* import the file and start to assemble line by line. Parameters: filepath - the file path(name). Returns: void */
void assemble_line(char * line); /* assembles the line - goes over the line argument by argument and assembles the line, if there are errors prints them. Parameters: line - pointer to the current line. Returns: voidn*/
void reset(); /* reset parameters between files.Parameters: void. Returns: void */

void add_to_command_table(void const * const ptr); /* adds the ready machine code to the instructions table. Parameters: ptr - a pointer to a command machine code line. Returns: void */
void add_to_data_table(void const * const ptr); /* adds the ready machine code to data table. Parameters: ptr - a pointer to a data machine code line. Returns: void */
void add_to_extern_table(char* labelName , int icAddress); /* gets label name and address of a line in the instructions table, adds a line to the extern table. Parameters: labelName, icAddress - address of a line in the instructions table. Returns: void */
boolean add_string(char * currArg);/* adds the a string to the data table. Parameters: currArg - a pointer to the current argument. Returns: TRUE if there were no errors */
boolean add_data(char * currArg); /* adds the data numbers to the data table, Parameters: currArg - a pointer to the current argument. Returns: TRUE if there were no errors */

#endif
