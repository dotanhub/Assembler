
#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "defenitions.h"
#include "parcer.h"

/* recognize the current argument type. parameters: curr - current argument, returns: argumentType - the argument type */
argumentType getArgumentType(char *curr);
/* check if the command is valid if so, checks operands and addressing methods, in first pass parse the main machine code, in second pass parse the next word. parameters: currArg - current argument,instructionCounter - pointer to instruction counter. returns: true if the command and operands are valid */
boolean check_command(char * currArg, int *instructionCounter);

/* checks if the argument is a valid label with ":". parameters: curr - current argument , returns: true if the label is valid */
boolean is_label(char *curr);
/* checks if the argument is a valid label name . parameters: curr - current argument , returns: true if the argument has a valid label name*/
boolean is_good_label(char *curr);
/* checks if the argument is a valid register name. parameters: curr - current argument , returns: true if the argument has a valid register name*/
boolean is_register(char *curr);
/* checks if the argument is a valid command name. parameters: curr - current argument , returns: true if the argument has a valid command name*/
boolean is_command(char *curr);

boolean valid_address_method(char *command, int met1, int met2); /* checks if the addressing method is valid according to the valid addressing method table. parameters: command - current command, met1 - first addressing method ,met2 - second addressing method , returns: true if it is a valid addressing method*/

boolean is_number(char * operand, int *tempNumber, boolean *valid); /* checks if the operand is a valid immediate access operand ("#" + number). parameters: operand - current operand, tempNumber - pointer to the operand number, valid - pointer to a boolean if it is a valid number , returns: true if the operand is a valid immediate access operand */

boolean is_dyn_operand(char *operand, int *dynNumLeft,  int *dynNumRight, boolean *valid); /* checks if the operand is a valid dynamic direct access operand (LABEL[num-num)). parameters: operand - current operand,pointers dynNumLeft,dynNumRight - pointers for holding the numbers of bits to cut, valid - pointer to a boolean if it is a valid number  , returns: true if the operand is a valid dynamic direct access operand */

int command_to_int(char *command); /* turns a string command to its parallel integer. parameters: command - command name , returns: the number of the command */

#endif
