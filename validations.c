/* -------------------------------------------------------------------------*/
/*                               validations.c                              */
/*       functions for validation checks of arguments and operands          */
/* -------------------------------------------------------------------------*/

#include "validations.h"

char * opCodes[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"}; /*commands table for validation checks*/

/*
recognize the current argument type.
parameters: curr - current argument
returns: argumentType - the argument type
*/
argumentType getArgumentType(char *curr)
{
    /*check if not empty*/
    if(!isspace(curr[0])) {
        /*check if comment*/
        if (curr[0] == ';')
            return COMMENT;
            /*check if label*/
        else if (is_label(curr))
            return LABEL;
            /*check if instruction and return the instruction type*/
        else if (curr[0] == '.') {
            if (strcmp(curr, ".string") == 0)
                return STRING;
            if (strcmp(curr, ".data") == 0)
                return DATA;
            if (strcmp(curr, ".entry") == 0)
                return ENTRY;
            if (strcmp(curr, ".extern") == 0)
                return EXTERN;
        }
            /*check if command*/
        else if(is_command(curr))
            return COMMAND;
        else
            return UNKNOWN;
    }
    else
    {
        return EMPTY;
    }
    return UNKNOWN;
}

/*
check if the command is valid if so, checks operands and addressing methods, in first pass parse the main machine code, in second pass parse the next word.
parameters: currArg - current argument,instructionCounter - pointer to instruction counter
returns: true if the command and operands are valid
*/
boolean check_command(char * currArg, int *instructionCounter)
{
    int operandCounter = 0;
    int i;
    int tempNumber1,tempNumber2; /* to store numbers of direct access*/
    char commandName[COMMAND_MAX_LENGTH]; /*holds the current command name*/
    char operand1[MAX_LINE_LENGTH]= "\0"; /*holds the first operand*/
    char operand2[MAX_LINE_LENGTH]= "\0"; /*holds the second operand*/
    int operand1Type= NO_OPERAND; /*first operand type (accessing method)*/
    int operand2Type= NO_OPERAND; /*first operand type (accessing method)*/
    int destOp, srcOp; /*holds the destination and source operands*/
    int dynNumLeft1, dynNumRight1, dynNumLeft2, dynNumRight2; /*holds the numbers of bits to cut in dynamic direct access*/
    int tempDyn; /*holds the number after it was cut in dynamic direct access*/
    int group; /*how many operands are in the line*/
    boolean labelFound = FALSE; /*for checking if a label exist in the symbol table*/
    boolean valid = TRUE; /* TRUE if all the operands are valid */

    strcpy(commandName,currArg);
    currArg = strtok(NULL, " , ");
    if ((!currArg) || (isspace(currArg[0]))) /*no arguments*/
    {
        operand1Type = NO_OPERAND;
        operand2Type = NO_OPERAND;
    }
    else
    {
        while ((currArg) && (!isspace(currArg[0]))) /*saves maximum of 2 operands*/
        {
            operandCounter++;
            if (operandCounter > 2)
            if (!isspace(currArg[0]))
            {
                printf("ERROR - Line number %d: too many operands after command\n", lineNumber);
                return FALSE;
            }
            if (operandCounter == 1)
            {
                strcpy(operand1, currArg);
                operand1Type = UNKNOWN_TYPE;
            }
            if (operandCounter == 2)
            {
                strcpy(operand2, currArg);
                operand2Type = UNKNOWN_TYPE;
            }
            currArg = strtok(NULL, " , \t\n");
        }
    }
    if (operand1Type > 0) /*if there is first operand checks is type*/
    {
        if (is_number(operand1, &tempNumber1, &valid))
            operand1Type = IMMEDIATE_ACCESS;
        else if (is_dyn_operand(operand1, &dynNumLeft1, &dynNumRight1, &valid))
            operand1Type = DYN_IMMEDIATE_ACCESS;
        else if (is_register(operand1))
            operand1Type = REGISTER_DIRECT_ACCESS;
        else if (is_good_label(operand1))
            operand1Type = DIRECT_ACCESS;
        else
            operand1Type = UNKNOWN_TYPE;
    }
    if (operand2Type > 0) /*if there is second operand checks is type*/
    {
        if (is_number(operand2, &tempNumber2, &valid))
            operand2Type = IMMEDIATE_ACCESS;
        else if (is_dyn_operand(operand2, &dynNumLeft2, &dynNumRight2, &valid))
            operand2Type = DYN_IMMEDIATE_ACCESS;
        else if (is_register(operand2))
            operand2Type = REGISTER_DIRECT_ACCESS;
        else if (is_good_label(operand2))
            operand2Type = DIRECT_ACCESS;
        else
            operand2Type = UNKNOWN_TYPE;
    }
    if ((operand1Type==UNKNOWN_TYPE) || (operand2Type==UNKNOWN_TYPE) || valid == FALSE) /*in case that one of the operand was not recognize or one of the operands is not valid */
    {
        printf("ERROR - Line number %d: wrong operands after %s\n", lineNumber,commandName);
        return FALSE;
    }
    if (!valid_address_method(commandName,operand1Type,operand2Type)) /*checks if the addressing methods are valid for this command */
        return FALSE;
    group=ZERO_OPERANDS;

    /* prepare the destination and source operands and determine how many operands  */
    if (operand1Type != NO_OPERAND)
    {
        group = ONE_OPERAND;
        destOp = operand1Type;
        srcOp = 0;
    }
    else
    {
        srcOp = 0;
        destOp = 0;
    }
    if (operand2Type != NO_OPERAND)
    {
        group = TWO_OPERANDS;
        srcOp = operand1Type;
        destOp = operand2Type;
    }

    if (!secondPass) /*only in the first pass adds main machine code to instructions table and if the addressing method is immediate or register access add their machine code as well, for other methods only adds line with zero */
    {
        create_main_machine_code (ERA_ABSOLUTE,destOp, srcOp, command_to_int(commandName), group,UNUSED_101);
        if ((operand1Type == REGISTER_DIRECT_ACCESS) && (operand2Type == REGISTER_DIRECT_ACCESS))
            create_source_and_destination_operands(ERA_ABSOLUTE,operand2[1]-'0',operand1[1]-'0',UNUSED_0);
        else
        {
            if (operand1Type == IMMEDIATE_ACCESS)
                create_direct_number(ERA_ABSOLUTE,tempNumber1);
            if (operand1Type == REGISTER_DIRECT_ACCESS)
                create_source_register (ERA_ABSOLUTE, UNUSED_0,operand1[1]-'0');
            if ((operand1Type == DYN_IMMEDIATE_ACCESS) || (operand1Type == DIRECT_ACCESS))
                (*instructionCounter)++;
            if (operand2Type == IMMEDIATE_ACCESS)
                create_direct_number(ERA_ABSOLUTE,tempNumber2);
            if (operand2Type == REGISTER_DIRECT_ACCESS)
                create_dest_register (ERA_ABSOLUTE, operand2[1]-'0',UNUSED_0);
            if ((operand2Type == DYN_IMMEDIATE_ACCESS) || (operand2Type == DIRECT_ACCESS))
                (*instructionCounter)++;
        }
    }
    else /* only in the second pass puts the machine code for dynamic and direct accessing*/
    {
        if ((operand1Type == REGISTER_DIRECT_ACCESS) && (operand2Type == REGISTER_DIRECT_ACCESS))
            (*instructionCounter) += 2;
        else
        {
            (*instructionCounter) += 1;
            if (operand1Type == DIRECT_ACCESS)
            {
                for (i = 0; i < symTableCounter; i++) /* checks if the label exists in the symbol table */
                {
                    if ((strcmp(symTable[i].name, operand1)) == 0)
                    {
                        if (symTable[i].isExternal == TRUE)
                        {
                            create_variable_address(ERA_EXTERNAL, symTable[i].address);
                            add_to_extern_table(symTable[i].name, (*instructionCounter)-1);
                        }
                        else
                            create_variable_address(ERA_RELOCATABLE, symTable[i].address);
                        labelFound = TRUE;
                    }
                }
                if (!labelFound)
                {
                    printf("ERROR - Line number %d: the operand \"%s\" does not exist in the symbols table\n",
                           lineNumber, operand1);
                    return FALSE;
                }
            }

            if (operand1Type == DYN_IMMEDIATE_ACCESS)
            {
                strcpy(operand1, strtok(operand1, " [ "));
                for (i = 0; i < symTableCounter; i++) /* checks if the label exists in the symbol table */
                {
                    if ((strcmp(symTable[i].name, operand1)) == 0)
                    {
                        tempDyn = instructionsTable[symTable[i].address];
                        tempDyn =(tempDyn >> dynNumLeft1) << ((sizeof(tempDyn)*8)-(dynNumRight1-dynNumLeft1+1)) >> ((sizeof(tempDyn)*8)-(dynNumRight1)) >> (dynNumLeft1-1); /* by moving the bits to the left and then to the right cuts the wanted part from the code */
                        create_direct_number(ERA_ABSOLUTE, tempDyn);
                        labelFound = TRUE;
                    }
                }
                if (!labelFound)
                {
                    printf("ERROR - Line number %d: the operand \"%s\" does not exist in the symbols table\n",
                           lineNumber, operand1);
                    return FALSE;
                }
            }
            if ((operand1Type == IMMEDIATE_ACCESS) || (operand1Type == REGISTER_DIRECT_ACCESS))
                (*instructionCounter)+=1;

            if (operand2Type == DIRECT_ACCESS)
            {
                for (i = 0; i < symTableCounter; i++)
                {
                    if ((strcmp(symTable[i].name, operand2)) == 0)
                    {
                        if (symTable[i].isExternal == TRUE)
                        {
                            create_variable_address(ERA_EXTERNAL, symTable[i].address);
                            add_to_extern_table(symTable[i].name, (*instructionCounter)-1);
                        }
                        else
                            create_variable_address(ERA_RELOCATABLE, symTable[i].address);
                        labelFound = TRUE;
                    }
                }
                if (!labelFound)
                {
                    printf("ERROR - Line number %d: the operand \"%s\" does not exist in the symbols table\n",
                           lineNumber, operand2);
                    return FALSE;
                }
            }

            if (operand2Type == DYN_IMMEDIATE_ACCESS)
            {
                strcpy(operand2, strtok(operand2, " [ "));
                for (i = 0; i < symTableCounter; i++)
                {
                    if ((strcmp(symTable[i].name, operand2)) == 0) /* checks if the label exists in the symbol table */
                    {
                        tempDyn = instructionsTable[symTable[i].address];
                        tempDyn =(tempDyn >> dynNumLeft2) << ((sizeof(tempDyn)*8)-(dynNumRight2-dynNumLeft2+1)) >> ((sizeof(tempDyn)*8)-(dynNumRight2)) >> (dynNumLeft2-1); /* by moving the bits to the left and then to the right cuts the wanted part from the code */
                        create_direct_number(ERA_ABSOLUTE, tempDyn);
                        labelFound = TRUE;
                    }
                }
                if (!labelFound)
                {
                    printf("ERROR - Line number %d: the operand \"%s\" does not exist in the symbols table\n",
                           lineNumber, operand2);
                    return FALSE;
                }
            }
            if ((operand2Type == IMMEDIATE_ACCESS) || (operand2Type == REGISTER_DIRECT_ACCESS))
                (*instructionCounter)+=1;
        }
    }
    return TRUE; /* the command and operands are valid */
}

/*
checks if the argument is a valid label with ":".
parameters: curr - current argument
returns: true if the label is valid
*/
boolean is_label(char *curr)
{
    if ((curr[strlen(curr)-1] == ':') && (is_good_label(curr)))
        return TRUE;
    return FALSE;
}

/*
checks if the argument is a valid label name
parameters: curr - current argument
returns: true if the argument has a valid label name
*/
boolean is_good_label(char *curr)
{
    int i = 0;
    char temp[LABEL_MAX_LENGTH];
    strcpy(temp,curr);
    if (temp[strlen(curr)-1] == ':') /* removes ":" */
        temp[strlen(curr)-1] = '\0';
    if (isspace(curr[strlen(curr)-1])) /* removes space */
        curr[strlen(curr)-1] = '\0';
    if (((strlen(curr)==REGISTER_LENGTH) && (is_register(temp))) || (is_command(temp)))
        printf("ERROR - Line number %d: label name can not be a register or command name\n", lineNumber);
    else if (strlen(curr) > LABEL_MAX_LENGTH)
    {
        printf("ERROR - Line number %d: label name is more than the maximum allowed\n", lineNumber);
    }
    else
    {
        if (!((curr[i] >= 'a' && curr[i] <= 'z') || (curr[i] >= 'A' && curr[i] <= 'Z')))
            printf("ERROR - line number %d: label name does not start with a letter\n",lineNumber);
        else
        {
            for (i=0; i<strlen(curr)-1; i++)
            {
                if (!((curr[i] >= 'a' && curr[i] <= 'z') || (curr[i] >= 'A' && curr[i] <= 'Z') ||
                      (curr[i] >= '0' && curr[i] <= '9')))
                {
                    printf("ERROR - line number %d: label name contains other character than only letters and numbers\n",
                           lineNumber);
                    return FALSE;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

/*
checks if the argument is a valid register name.
parameters: curr - current argument
returns: true if the argument has a valid register name
*/
boolean is_register(char *curr)
{
    if ((curr[0] == 'r' ) && (curr[1] >= '0') && (curr[1] <= '7') && ((isspace(curr[2])) || curr[2]=='\0'))
        return TRUE;
    return FALSE;
}

/*
checks if the argument is a valid command name.
parameters: curr - current argument
returns: true if the argument has a valid command name
*/
boolean is_command(char *curr)
{
    int i = 0;
    if (isspace(curr[strlen(curr)-1]))
        curr[strlen(curr)-1]=0;
    for (i=0; i<COMMANDS_AMOUNT; i++)
        if ((strcmp(curr,opCodes[i]) == 0))
            return TRUE;
    return FALSE;
}

/*
checks if the addressing method is valid according to the valid addressing method table.
parameters: command - current command, met1 - first addressing method ,met2 - second addressing method
returns: true if it is a valid addressing method
*/
boolean valid_address_method(char *command, int met1, int met2)
{
    if ((strcmp(command,"mov") == 0))
        if (((met1==0) || (met1==1) || (met1==2) || (met1==3)) && ((met2==1) || (met2==3)))
            return TRUE;
    if ((strcmp(command,"cmp") == 0))
        if (((met1==0) || (met1==1) || (met1==2) || (met1==3)) && ((met2==0) ||(met2==1) || (met2==2) ||(met2==3)))
            return TRUE;
    if ((strcmp(command,"add") == 0))
        if (((met1==0) || (met1==1) || (met1==2) || (met1==3)) && ((met2==1) || (met2==3)))
            return TRUE;
    if ((strcmp(command,"sub") == 0))
        if (((met1==0) || (met1==1) || (met1==2) || (met1==3)) && ((met2==1) || (met2==3)))
            return TRUE;
    if ((strcmp(command,"lea") == 0))
        if ((met1==1) && ((met2==1) || (met2==3)))
            return TRUE;
    if ((strcmp(command,"not") == 0))
        if ((met1==1) && ((met2==1) || (met2==3)))
            return TRUE;
    if ((strcmp(command,"clr") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"inc") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"dec") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"jmp") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"bne") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"red") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"prn") == 0))
        if (((met1==0) || (met1==1) || (met1==2) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"jsr") == 0))
        if (((met1==1) || (met1==3)) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"rts") == 0))
        if ((met1==-1) && (met2==-1))
            return TRUE;
    if ((strcmp(command,"stop") == 0))
        if ((met1==-1) && (met2==-1))
            return TRUE;
    printf("ERROR - Line number %d: the addressing method is not valid for \"%s\" command \n", lineNumber, command);
    return FALSE;
}

/*
checks if the operand is a valid immediate access operand ("#" + number).
parameters: operand - current operand, tempNumber - pointer to the operand number, valid - pointer to a boolean if it is a valid number
returns: true if the operand is a valid immediate access operand
*/
boolean is_number(char * operand, int *tempNumber, boolean *valid)
{
    char *tempOperand;
    char ch;
    int tmp;

    if (isspace(operand[strlen(operand)-1])) /* removes space */
        operand[strlen(operand)-1] = '\0';
    if (operand[0] == '#')  /*checks if it is immediate addressing*/
    {
        tempOperand = operand + 1;
        if (isspace(tempOperand[strlen(tempOperand)-1])) /* removes space */
            tempOperand[strlen(tempOperand)-1]= '\0';
        tmp = sscanf(tempOperand, "%d%c", tempNumber, &ch); /* scan a number, if there was other char next to the number */
        if (tmp == 1)
            return TRUE;
        else
        {
            printf("ERROR - Line number %d: wrong syntax after #\n", lineNumber);
            *valid = FALSE; /* it is an immediate access operand but it is not valid */
            return TRUE;
        }
    }
    return FALSE;
}

/*
checks if the operand is a dynamic direct access operand (LABEL[num-num)).
parameters: operand - current operand,pointers dynNumLeft,dynNumRight - pointers for holding the numbers of bits to cut, valid - pointer to a boolean if it is a valid number
returns: true if the operand is a valid dynamic direct access operand
*/
boolean is_dyn_operand(char *operand, int *dynNumLeft,  int *dynNumRight, boolean *valid)
{
    char *tempOperand;
    int tmp;
    char ch1,ch2;

    tempOperand = strtok(operand, " [\t\n");
    if (tempOperand)
    {
        if (is_good_label(tempOperand))
        {
            tempOperand = strtok(NULL, " ]\t\n");
            if (!tempOperand)
                return FALSE;
            tmp = sscanf(tempOperand, "%d%c%d%c", dynNumLeft, &ch1, dynNumRight, &ch2); /* scans the numbers and chars between "[]" and saves them */
            if ((tmp == 3) && (ch1 == '-'))  /* if there are only two numbers and "-" between them */
            {
                if ((*dynNumLeft < 0) || (*dynNumLeft > 14) || (*dynNumRight < 0) || (*dynNumRight > 14) ||
                    ((*dynNumRight - *dynNumLeft) > 12)) /* check if it is a valid use of the numbers */
                {
                    printf("ERROR - Line number %d: non-valid numbers within:\"[]\" in dynamic addressing\n",
                           lineNumber);
                    *valid = FALSE; /* it is an dynamic address operand but it is not valid */
                    return TRUE;
                }
                tempOperand = strtok(NULL, " \t\n");
                if (tempOperand)
                {
                    if (!isspace(tempOperand[0])) /* check if there are other chars next to "[]" */
                    {
                        printf("ERROR - Line number %d: wrong syntax after \"[]\" in dynamic addressing\n",
                               lineNumber);
                        *valid = FALSE; /* it is an dynamic address operand but it is not valid */
                        return TRUE;
                    }
                    return TRUE; /*finished the checks, valid dynamic addressing - with spaces afterwards*/
                }
                else
                    return TRUE; /*finished the checks, valid dynamic addressing*/
            }
            else
            {
                printf("ERROR - Line number %d: wrong syntax within:\"[]\" in dynamic addressing\n",
                       lineNumber);
                *valid = FALSE; /* it is an dynamic address operand but it is not valid */
                return TRUE;
            }
        }
        else
        {
            printf("ERROR - Line number %d: non-valid label in dynamic addressing\n", lineNumber);
            *valid = FALSE; /* it is an dynamic address operand but it is not valid */
            return TRUE;
        }
    }
    else
        return FALSE;
}

/*
turns a string command to its parallel integer.
parameters: command - command name
returns: the number of the command
 */
int command_to_int(char *command)
{
    int i;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {
        if (strcmp(opCodes[i], command) == 0)
            return i;
    }
    return -1;
}
