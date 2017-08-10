#ifndef PARCER_H
#define PARCER_H

#include <stdio.h>
#include "defenitions.h"
#include "datastructures.h"
#include "assembler.h"

/* receives the wanted parameters for creating the main machine code and adds it to the instruction table. */
void create_main_machine_code (unsigned int era,unsigned int opDest, unsigned int opSource, unsigned int opcode, unsigned int group,unsigned int unused);
/* receives the wanted parameters for creating a direct number machine code and adds it to the instruction table. */
void create_direct_number (unsigned int era,signed int numberOp);
/* receives the wanted parameters for creating a variable address machine code and adds it to the instruction table. */
void create_variable_address(unsigned int era,unsigned int address);
/* receives the wanted parameters for creating a destination register machine code and adds it to the instruction table. */
void create_dest_register (unsigned int era,unsigned int destRegNum, unsigned int unused);
/* receives the wanted parameters for creating a source register machine code and adds it to the instruction table. */
void create_source_register (unsigned int era, unsigned int unused,unsigned int sourceRegNum);
/* receives the wanted parameters for creating a source and destinations registers machine code and adds it to the instruction table. */
void create_source_and_destination_operands (unsigned int era,unsigned int destRegNum,unsigned int sourceRegNum, unsigned int unused);
/* receives the wanted number for creating a data machine code and adds it to the data table. */
void create_data_line (int data);
/* receives the wanted char for creating a string machine code and adds it to the data table. */
void create_char_line (int ch);

#endif
