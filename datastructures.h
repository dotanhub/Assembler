#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "defenitions.h"

/* symbol table line structure */
typedef struct symbolLine *symLineP;
typedef struct symbolLine
{
    char name[LABEL_MAX_LENGTH];
    int address;
    boolean isExternal;
    boolean isEntry;
    char *sentence; /* if is data or string*/
}symbolLine;
extern symLineP symTable;

/* extern table line structure */
typedef struct externLine *externLineP;
typedef struct externLine
{
    char name[LABEL_MAX_LENGTH];
    int address;
}externLine;
extern externLineP externTable;

/* -------------------------------------------------------------------------*/
/*                           MACHINE CODE STRUCTURES                        */
/* -------------------------------------------------------------------------*/

/* main machine code structure */
typedef union mainMachineCode
{
    struct
    {
        unsigned int era : 2;
        unsigned int opDest : 2;
        unsigned int opSource : 2;
        unsigned int opcode : 4;
        unsigned int group : 2;
        unsigned int unused : 3;
    } u;
    unsigned int fullReg;
}mainMachineCode;

/* number operand machine code structure */
typedef union numberOperand
{
    struct
    {
        unsigned int era : 2;
        signed int number_operand : 13;
    } u;
    unsigned int fullReg;
} numberOperand;

/* variable operand machine code structure */
typedef union variableAddress
{
    struct
    {
        unsigned int era : 2;
        unsigned int var_address : 13;
    } u;
    unsigned int fullReg;
} variableAddress;

/* destination register operand machine code structure */
typedef union destinationOperand
{
    struct
    {
        unsigned int era : 2;
        unsigned int dest_reg_num : 6;
        unsigned int unused : 7;
    } u;
    unsigned int fullReg;
} destinationOperand;

/* source register operand machine code structure */
typedef union sourceOperand
{
    struct
    {
        unsigned int era : 2;
        unsigned int unused : 6;
        unsigned int source_reg_num : 6;
    } u;
    unsigned int fullReg;
} sourceOperand;

/* source and destination registers machine code structure */
typedef union sourceAndDestinationOperands
{
    struct
    {
        unsigned int era : 2;
        unsigned int dest_reg_num : 6;
        unsigned int source_reg_num : 6;
        unsigned int unused : 1;
    } u;
    unsigned int fullReg;
} sourceAndDestinationOperands;

/* data or string machine code structure */
typedef union dataStruct
{
    struct
    {
        int data : 15;
    } d;
    struct
    {
        int ch : 15;
    } s;
    unsigned int fullReg;
} dataStruct;

#endif
