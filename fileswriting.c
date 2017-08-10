/* -------------------------------------------------------------------------*/
/*                               fileswriting.c                             */
/*                    functions to write the export files                   */
/* -------------------------------------------------------------------------*/

#include "fileswriting.h"

/*
write output files
Parameters: file fName - file name ,instructionCounter ,dataCounter ,externLabelCounter, entryLabelCounter
returns: void
*/
void write_files(char *fName,int instructionCounter,int dataCounter,int externLabelCounter,int entryLabelCounter)
{
    char filePath[MAX_FILE_NAME_L];
    strcpy(filePath, fName);

    if(externLabelCounter > 0)/*write extern file if externs exist*/
    {
        strcat(filePath, ".ext");
        write_extern_file(filePath, externLabelCounter);
    }
    if(entryLabelCounter > 0)/*write entry file if enteries exist*/
    {
        strcpy(filePath, fName);
        strcat(filePath, ".ent");
        write_entry_file(filePath);
    }
    strcpy(filePath, fName);
    strcat(filePath, ".ob");
    write_object_file(filePath,instructionCounter,dataCounter);
}

/*
write the object file with ob extension
Parameters: filePath, instructionCounter, dataCounter
returns: void
*/
void write_object_file(char *filePath,int instructionCounter,int dataCounter)
{
    int i;
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char address[MAX_SPECIAL_LENGTH];
    char machineCode[MAX_SPECIAL_LENGTH];
    char icSpecial[MAX_SPECIAL_LENGTH] = "0";
    char dcSpecial[MAX_SPECIAL_LENGTH] = "0";

    file = fopen(filePath, "w");
    if(file)
    {
        dec_to_base_8(instructionCounter-dataCounter,icSpecial,TRUE);
        dec_to_base_8(dataCounter,dcSpecial,TRUE);
        sprintf(line, "\t%s %s\n", icSpecial, dcSpecial);
        fputs(line, file);

        for(i=0; i < instructionCounter; i++)
        {
            memset(machineCode,0,MAX_SPECIAL_LENGTH);
            memset(address,0,MAX_SPECIAL_LENGTH);
            strcpy(address,dec_to_base_8(i+FIRST_INSTRUCTION_ADDRESS,address,TRUE));
            strcpy(machineCode,dec_to_base_8(instructionsTable[i],machineCode,FALSE));
            sprintf(line, "%s\t%s\n", address, machineCode);
            fputs(line, file);
        }

        fclose(file);
    }
}

/*
write the entry file with ent extension
Parameters: filePath
returns: void
*/
void write_entry_file(char *filePath)
{
    int i;
    char specialEntry[MAX_SPECIAL_LENGTH];
    char line[MAX_LINE_LENGTH];
    FILE *file;

    file = fopen(filePath, "w");
    if(file)
    {
        for(i=0; i < symTableCounter; i++)
        {
            if (symTable[i].isEntry == TRUE)
            {
                memset(specialEntry,0,MAX_SPECIAL_LENGTH);
                sprintf(line, "%s\t%s\n", symTable[i].name, dec_to_base_8(symTable[i].address+FIRST_INSTRUCTION_ADDRESS, specialEntry,TRUE));
                fputs(line, file);
            }
        }
        fclose(file);
    }
}

/*
write the extern file with ext extension
Parameters: file path, externLabelsCounter
returns: void
 */
void write_extern_file(char *filePath,int externLabelCounter)
{
    int i;
    char specialExternal[MAX_SPECIAL_LENGTH];
    char line[MAX_LINE_LENGTH];
    FILE *file;

    file = fopen(filePath, "w");
    if(file)
    {
        for(i=0; i < externLabelCounter; i++)
        {
            memset(specialExternal,0,MAX_SPECIAL_LENGTH);
            sprintf(line, "%s\t%s\n", externTable[i].name, dec_to_base_8(externTable[i].address+FIRST_INSTRUCTION_ADDRESS,specialExternal,TRUE));
            fputs(line, file);
        }
        fclose(file);
    }
}

/*
receives a decimal number and changes it to a spacial octal base string
Parameters: num - number, special - pointer to a char array that will hold the converted string, isAddress - TRUE if it is an address FALSE if it is an instruction line
returns: pointer to a string of the converted number
*/
char *dec_to_base_8(int num,char *special,boolean isAddress)
{

    if (!isAddress)
        make_special_string(special,num,INS_SPECIAL_LENGTH);
    else
    {
        if (num < EIGHT_POWER_ONE)
            make_special_string(special,num,ADD_SPECIAL_LENGTH_1);
        else if (num < EIGHT_POWER_TWO)
            make_special_string(special,num,ADD_SPECIAL_LENGTH_2);
        else if (num < EIGHT_POWER_THREE)
            make_special_string(special,num,ADD_SPECIAL_LENGTH_3);
        else
            make_special_string(special,num,ADD_SPECIAL_LENGTH_4);
    }
    return special;
}

/*
receives a decimal number and changes it to a spacial octal base string according to its predicted number of octal base chars (length)
Parameters: num - number, special - pointer to a char array that will hold the converted string, length - predicted number of octal base chars
returns: pointer to a string of the converted number
*/
char * make_special_string(char *special,int num, int length)
{
    int i;
    int mask = 0;
    int tmpNum;

    mask = mask | 0x7; /*mask for the 3 right bits*/
    for (i=0; i<length; i++)
    {
        tmpNum = num & mask; /*takes only the three right bits and turn them to an octal base number*/
        special[length-i-1] = num_to_special(tmpNum); /*changes the number to a special eight base char and puts it in the string*/
        num = num >> 3; /*push the bits 3 places to the right fot the next octal base number digit*/
    }
    return special;
}

/*
receives a octal digit and changes it to a spacial octal base string
Parameters: num - octal digit
returns: pointer to a string of the converted number
*/
char num_to_special (int num)
{
    switch (num)
    {
        case 0: return '!';
        case 1: return '@';
        case 2: return '#';
        case 3: return '$';
        case 4: return '%';
        case 5: return '^';
        case 6: return '&';
        case 7: return '*';
    }
    return 'E'; /*error*/
}