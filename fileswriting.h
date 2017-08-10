
#ifndef FILESWRITING_H
#define FILESWRITING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "validations.h"
#include "defenitions.h"
#include "parcer.h"
#include "assembler.h"

/* write output files, Parameters: file fName - file name ,instructionCounter ,dataCounter ,externLabelCounter, entryLabelCounter, returns: void */
void write_files(char *fName,int instructionCounter,int dataCounter,int externLabelCounter,int entryLabelCounter);
/* write the object file with ob extension, Parameters: filePath, instructionCounter, dataCounter, returns: void */
void write_object_file(char *filePath,int instructionCounter,int dataCounter);
/* write the entry file with ent extension, Parameters: filePath, returns: void */
void write_entry_file(char *filePath);
/*write the extern file with ext extension, Parameters: file path, externLabelsCounter, returns: void */
void write_extern_file(char *filePath,int externLabelCounter);
/*receives a decimal number and changes it to a spacial octal base string, Parameters: num - number, special - pointer to a char array that will hold the converted string, isAddress - TRUE if it is an address FALSE if it is an instruction line, returns: pointer to a string of the converted number*/
char * dec_to_base_8 (int num,char *special,boolean isAddress);
/* receives a octal digit and changes it to a spacial octal base string, Parameters: num - octal digit, returns: pointer to a string of the converted number */
char num_to_special (int num);
/* receives a decimal number and changes it to a spacial octal base string according to its predicted number of octal base chars (length),  Parameters: num - number, special - pointer to a char array that will hold the converted string, length - predicted number of octal base chars, returns: pointer to a string of the converted number*/
char * make_special_string(char *special,int num, int length);

#endif
