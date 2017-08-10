/* -------------------------------------------------------------------------*/
/*                                  parcer.c                                */
/*    functions that receive assembly command and change it to binary       */
/* -------------------------------------------------------------------------*/

#include "parcer.h"

/* -------------------------------------------------------------------------*/
/*         CREATE ONE LINE BINARY MACHINE CODE FUNCTIONS                    */
/* -------------------------------------------------------------------------*/

/*
receives the wanted parameters for creating the main machine code and adds it to the instruction table.
Parameters: era: the wanted E,R,A code, opDest - addressing method for the destination operand, opSource - addressing method for the source operand, opcode - the command, group - how many operands code, unused - unused bits.
Returns: void
 */
void create_main_machine_code (unsigned int era,unsigned int opDest, unsigned int opSource, unsigned int opcode, unsigned int group,unsigned int unused)
{
    mainMachineCode m;

    m.fullReg = 0;
    m.u.era = era;
    m.u.opDest = opDest;
    m.u.opSource = opSource;
    m.u.opcode = opcode;
    m.u.group = group;
    m.u.unused = unused;
    add_to_command_table(&m.u);
}

/*
receives the wanted parameters for creating a direct number machine code and adds it to the instruction table.
Parameters: era: the wanted E,R,A code, numberOp - a number
Returns: void
 */
void create_direct_number (unsigned int era,signed int numberOp)
{
    numberOperand n;

    n.fullReg = 0;
    n.u.era = era;
    n.u.number_operand = numberOp;
    add_to_command_table(&n.u);
}

/*
receives the wanted parameters for creating a variable address machine code and adds it to the instruction table.
Parameters: era: the wanted E,R,A code, address - an address in the instruction table
Returns: void
 */
void create_variable_address(unsigned int era,unsigned int address)
{
    variableAddress v;

    v.fullReg = 0;
    v.u.era = era;
    v.u.var_address = address;
    add_to_command_table(&v.u);
}

/*
receives the wanted parameters for creating a destination register machine code and adds it to the instruction table.
Parameters: era: the wanted E,R,A code, destRegNum - destination register number, unused - unused bits
Returns: void
 */
void create_dest_register (unsigned int era,unsigned int destRegNum, unsigned int unused)
{
    destinationOperand d;

    d.fullReg = 0;
    d.u.era = era;
    d.u.dest_reg_num = destRegNum;
    d.u.unused = unused;
    add_to_command_table(&d.u);
}

/*
receives the wanted parameters for creating a source register machine code and adds it to the instruction table.
Parameters: era: the wanted E,R,A code, unused - unused bits ,sourceRegNum - source register number
Returns: void
 */
void create_source_register (unsigned int era, unsigned int unused,unsigned int sourceRegNum)
{
    sourceOperand s;

    s.fullReg = 0;
    s.u.era = era;
    s.u.unused = unused;
    s.u.source_reg_num = sourceRegNum;
    add_to_command_table(&s.u);
}

/*
receives the wanted parameters for creating a source and destinations registers machine code and adds it to the instruction table.
Parameters: era: the wanted E,R,A code, destRegNum - destination register number ,sourceRegNum - source register number, unused - unused bits
Returns: void
 */
void create_source_and_destination_operands (unsigned int era,unsigned int destRegNum,unsigned int sourceRegNum, unsigned int unused)
{
    sourceAndDestinationOperands sd;

    sd.fullReg = 0;
    sd.u.era = era;
    sd.u.dest_reg_num = destRegNum;
    sd.u.source_reg_num = sourceRegNum;
    sd.u.unused = unused;
    add_to_command_table(&sd.u);
}

/*
receives the wanted number for creating a data machine code and adds it to the data table.
Parameters: data - number
Returns: void
 */
void create_data_line (int data)
{
    dataStruct d;

    d.fullReg = 0;
    d.d.data = data;
    add_to_data_table(&d.d);
}

/*
receives the wanted char for creating a string machine code and adds it to the data table.
Parameters: ch - char in ascii code(integer)
Returns: void
 */
void create_char_line (int ch)
{
    dataStruct s;

    s.fullReg = 0;
    s.s.ch = ch;
    add_to_data_table(&s.d);
}
