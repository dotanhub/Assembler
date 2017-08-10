/* -------------------------------------------------------------------------*/
/*                                 assembler.c                              */
/*                   core functions for assembling a file                   */
/* -------------------------------------------------------------------------*/

#include "assembler.h"

int instructionsTable[MAX_ARRAY]; /* saves the machine code in an array assuming maximum size for the array */
int dataTable[MAX_ARRAY]; /* saves the data code in an array assuming maximum size for the array */
int instructionCounter = 0; /* counter for the current line of instructions table */
int dataCounter = 0; /* counter for the current line of data table */
int lineNumber; /* current line in file */
int symTableCounter; /* counter for the symbol table */
symLineP symTable; /* pointer for the start of symbol table */
externLineP externTable; /* pointer for the start of extern labels table */
int externLabelCounter; /* counter for the current line of extern labels table */
int entryLabelCounter; /* counter for the current amount of entry labels */
char *currArg; /* pointer to the current argument */
boolean secondPass; /*if FALSE it is the first pass, if TRUE it is the second pass */
boolean foundError; /*if an error was found = TRUE */

/*
import the file and start to assemble line by line.
Parameters: filepath - the file path(name)
Returns: void
*/
void import_from_file(char filePath[])
{
    char fileName[MAX_FILE_NAME_L];
    char *ch;

    reset();
    if (strlen(filePath) > MAX_FILE_NAME_L)
    {

        printf("File %s: length exceeded the maximum length of:%d\n", filePath, MAX_FILE_NAME_L);
        return;
    }
    else
    {
        int i;
        int j;
        FILE *file;
        char line[MAX_LINE_LENGTH];
        char *curr = NULL;
        file = fopen(filePath, "r");
        if (file)
        {

            printf("\nFile:%s - Start reading file \n", filePath);
            curr = fgets(line, MAX_LINE_LENGTH, file);
            if (!curr)
            {
                printf("File:%s - Is empty\n", filePath);
                return;
            }
            while (curr)
            {
                lineNumber++;
                assemble_line(line);
                curr = fgets(line, MAX_LINE_LENGTH, file);
            }
            printf("\nFile:%s - Finish reading file first pass \n", filePath);
            if (!foundError)  /* only if there was no error, start the second pass */
            {
                for (i = 0; i < symTableCounter; i++) /*update address for data labels*/
                    if ((strcmp(symTable[i].sentence, "data") == 0) || (strcmp(symTable[i].sentence, "string") == 0))
                        symTable[i].address += instructionCounter;
                for (i = instructionCounter, j = 0; j < dataCounter; i++, j++)  /*adds data table to instructions table*/
                    instructionsTable[i] = dataTable[j];
                secondPass = TRUE;
                lineNumber = 0;
                instructionCounter = 0;
                fseek(file, 0, SEEK_SET);
                curr = fgets(line, MAX_LINE_LENGTH, file);
                while (curr)
                {
                    lineNumber++;
                    assemble_line(line);
                    curr = fgets(line, MAX_LINE_LENGTH, file);
                }
                printf("File:%s - Finish reading file second pass\n", filePath);
            }

            if (!foundError)  /* only if there was no error, start writing the files */
            {
                strcpy(fileName, filePath);
                ch = strrchr(fileName, '.');
                *ch = '\0';
                write_files(fileName,instructionCounter + dataCounter,dataCounter,externLabelCounter,entryLabelCounter);
                printf("File:%s - Finish writing files output\n", filePath);
            }
            fclose(file); /* close file */
            free(symTable);
            free(externTable);
        }
        else {
            printf("\nFile:%s - Cannot open file\n", filePath);
            return;
        }
    }
}

/*
assembles the line - goes over the line argument by argument and assembles the line, if there are errors prints them.
Parameters: line - pointer to the current line
Returns: void
*/
void assemble_line(char * line)
{
    char currLine[MAX_LINE_LENGTH]; /* copy of the current line in the file */
    boolean symbolFlag = FALSE; /* if there was a lebel in the start of the line - TRUE */
    char labelName[LABEL_MAX_LENGTH];
    int i;

    strncpy(currLine, line, MAX_LINE_LENGTH);
    currArg = strtok(currLine, " \t\n");
    while (currArg != NULL)
    {
        switch (getArgumentType(currArg)) /* recognize the current argument type */
        {
            case COMMENT:
            case EMPTY:
            {
                return;
            }
            case UNKNOWN:
            {
                printf("ERROR - Line number %d: wrong syntax\n", lineNumber);
                foundError = TRUE;
                return;
            }
            case LABEL: /* if it is label copy the label name and raise the symbol flag */
            {
                symbolFlag = TRUE;
                memset(labelName, 0, LABEL_MAX_LENGTH);
                strncpy(labelName, currArg, strlen(currArg) -1);
                currArg = strtok(NULL, " \t\n"); /* take next argument */
                break;
            }
            case DATA:
            {
                if (secondPass) /*parse data only in the first pass */
                    return;
                if (symbolFlag) /*if there was label before checks if it is already exist in the symbol table */
                {
                    for (i=0; i<symTableCounter; i++)
                        if ((strcmp(symTable[i].name,labelName))==0)
                        {
                            foundError = TRUE;
                            printf("ERROR - Line number %d: the label %s already exist\n", lineNumber, labelName);
                            return;
                        }
                    symTable = realloc(symTable,((sizeof(symbolLine))*(symTableCounter+1))); /*allocate memory for a line in the symbol table */
                    if (!symTable)
                    {
                        printf("ERROR - Line number %d: unable to allocate memory for new symbol table line\n",
                               lineNumber);
                        exit(1);
                    }
                    /* create a new line in the symbol table */
                    strcpy(symTable[symTableCounter].name, labelName);
                    symTable[symTableCounter].address = dataCounter;
                    symTable[symTableCounter].isExternal = FALSE;
                    symTable[symTableCounter].isEntry = FALSE;
                    symTable[symTableCounter].sentence = "data";
                    if (!add_data(currArg)) /* adds the data to the data table, if there were errors doesnt progress the counter so the next symbol will erase the current symbol line with errors*/
                    {
                        foundError = TRUE;
                        return;
                    }
                    symTableCounter++;
                    return;
                }
                else if (!add_data(currArg)) /* if there was no label adds the data to the data table */
                {
                    foundError = TRUE;
                    return;
                }
                break;
            }
            case STRING:
            {
                if (secondPass) /*parse data only in the first pass */
                    return;
                if (symbolFlag) /*if there was label before checks if it is already exist in the symbol table */
                {
                    for (i=0; i<symTableCounter; i++)
                        if ((strcmp(symTable[i].name,labelName))==0)
                        {
                            foundError = TRUE;
                            printf("ERROR - Line number %d: the label %s already exist\n", lineNumber, labelName);
                            return;
                        }
                    symTable = realloc(symTable, ((sizeof(symbolLine))*(symTableCounter+1))); /*allocate memory for a line in the symbol table */

                    if (!symTable)
                    {
                        printf("ERROR - Line number %d: unable to allocate memory for new symbol table line\n",
                               lineNumber);
                        exit(1);
                    }
                    /* create a new line in the symbol table */
                    strcpy(symTable[symTableCounter].name, labelName);
                    symTable[symTableCounter].address = dataCounter;
                    symTable[symTableCounter].isExternal = FALSE;
                    symTable[symTableCounter].isEntry = FALSE;
                    symTable[symTableCounter].sentence = "string";
                    if (!add_string(currArg)) /* adds the string to the data table, if there were errors doesnt progress the counter so the next symbol will erase the current symbol line with errors*/
                    {
                        foundError = TRUE;
                        return;
                    }
                    symTableCounter++;
                    return;

                }
                else if (!add_string(currArg))  /* if there was no label adds the string to the data table */
                {
                    foundError = TRUE;
                    return;
                }
                return;
            }

            case EXTERN:
            {
                if (secondPass) /*ignore on second pass */
                    return;
                if (symbolFlag)
                {
                    printf("WARNING - Line number %d: there is no need of label for .extern\n", lineNumber);
                }
                currArg = strtok(NULL, " \t\n"); /*get extern label*/
                if (currArg)
                {
                    if (!is_good_label(currArg))
                    {
                        foundError = TRUE;
                        printf("ERROR - Line number %d: the label after .extern is not valid\n", lineNumber);
                        return;
                    }
                    memset(labelName, 0, LABEL_MAX_LENGTH);
                    if (currArg[strlen(currArg)-1] == '\r') /*removes spaces from the end of the string*/
                        strncpy(labelName, currArg, strlen(currArg)-1);
                    else
                        strncpy(labelName, currArg, strlen(currArg));
                    for (i=0; i<symTableCounter; i++) /*check if the label already exist in the symbol table */
                        if ((strcmp(symTable[i].name, labelName)) == 0)
                        {
                            foundError = TRUE;
                            printf("ERROR - Line number %d: the label %s already exist\n", lineNumber, labelName);
                            return;
                        }
                    symTable = realloc(symTable, ((sizeof(symbolLine))*(symTableCounter+1))); /*allocate memory for a line in the symbol table */
                    if (!symTable)
                    {
                        printf("ERROR - Line number %d: unable to allocate memory for new symbol table line\n",
                               lineNumber);
                        exit(1);
                    }
                    /* create a new line in the symbol table */
                    strcpy(symTable[symTableCounter].name, labelName);
                    symTable[symTableCounter].address = 0;
                    symTable[symTableCounter].isExternal = TRUE;
                    symTable[symTableCounter].isEntry = FALSE;
                    symTable[symTableCounter].sentence = ".extern";
                    symTableCounter++;
                }
                else
                {
                    printf("ERROR - Line number %d: could not read label after .extern \n", lineNumber);
                    foundError = TRUE;
                }
                currArg = strtok(NULL, " \t\n"); /*check if there is another argument*/
                if ((currArg) && (!isspace(currArg[0])))
                {
                    printf("ERROR - Line number %d: only one label is allowed after .extern \n", lineNumber);
                    foundError = TRUE;
                }
                return;
            }
            case ENTRY:
            {
                if (!secondPass) /*ignore on fisrt pass */
                    return;
                if (symbolFlag)
                {
                    printf("WARNING - Line number %d: there is no need of label for .entry\n", lineNumber);
                }
                currArg = strtok(NULL, " \t\n"); /*get entry label*/
                if (currArg)
                {
                    if (!is_good_label(currArg))
                    {
                        foundError = TRUE;
                        printf("ERROR - Line number %d: the label after .entry is not valid\n", lineNumber);
                        return;
                    }
                    memset(labelName, 0, LABEL_MAX_LENGTH);
                    if (currArg[strlen(currArg)-1] == '\r') /*removes spaces from the end of the string*/
                        strncpy(labelName, currArg, strlen(currArg)-1);
                    else
                        strncpy(labelName, currArg, strlen(currArg));
                    for (i=0; i<symTableCounter; i++) /*check if the label already exist in the symbol table */
                        if ((strcmp(symTable[i].name, labelName)) == 0)
                        {
                            symTable[i].isEntry = TRUE;
                            entryLabelCounter++;
                            return;
                        }
                    printf("ERROR - Line number %d: the label after .entry is not exist \n", lineNumber);
                    foundError = TRUE;

                }
                else
                {
                    printf("ERROR - Line number %d: could not read label after .entry \n", lineNumber);
                    foundError = TRUE;
                }
                currArg = strtok(NULL, " \t\n");/*check if there is another argument*/
                if ((currArg) && (!isspace(currArg[0])))
                {
                    printf("ERROR - Line number %d: only one label is allowed after .entry \n", lineNumber);
                    foundError = TRUE;
                }
                return;
            }

            case COMMAND:
            {
                if (!secondPass) /*on first pass only - adds a label if exist */
                {
                    if (symbolFlag)
                    {
                        for (i = 0; i < symTableCounter; i++)
                            if ((strcmp(symTable[i].name, labelName)) == 0)
                            {
                                foundError = TRUE;
                                printf("ERROR - Line number %d: the label %s already exist\n", lineNumber, labelName);
                                return;
                            }
                        symTable = realloc(symTable, ((sizeof(symbolLine))*(symTableCounter+1)));
                        if (!symTable)
                        {
                            printf("ERROR - Line number %d: unable to allocate memory for new symbol table line\n",
                                   lineNumber);
                            exit(1);
                        }
                        strcpy(symTable[symTableCounter].name, labelName);
                        symTable[symTableCounter].address = instructionCounter;
                        symTable[symTableCounter].isExternal = FALSE;
                        symTable[symTableCounter].sentence = "command";
                        symTableCounter++;
                    }
                }
                 /*on first and second pass */
                if (!check_command(currArg,&instructionCounter)) /*checks the command and following operands*/
                {
                    foundError = TRUE;
                    return;
                }
            }
            return;
        }
    }
}

/*
reset parameters between files
Parameters: void
Returns: void
*/
void reset()
{
    foundError=FALSE;
    symTableCounter = 0;
    externLabelCounter = 0;
    entryLabelCounter=0;
    dataCounter=0;
    instructionCounter=0;
    lineNumber = 0;
    secondPass = FALSE;
    instructionsTable[MAX_ARRAY]=0;
    symTable = NULL;
    externTable = NULL;
}

/* -------------------------------------------------------------------------*/
/*                          ASSISTANT FUNCTIONS                             */
/* -------------------------------------------------------------------------*/


/*
adds the ready machine code to the instructions table
Parameters: ptr - a pointer to a command machine code line
Returns: void
*/
void add_to_command_table(void const * const ptr)
{
    unsigned int *temp = (unsigned int *) ptr;
    instructionsTable[instructionCounter] = *temp;
    instructionCounter++;
}

/*
adds the ready machine code to data table
Parameters: ptr - a pointer to a data machine code line
Returns: void
*/
void add_to_data_table(void const * const ptr)
{
    unsigned int *temp = (unsigned int *) ptr;
    dataTable[dataCounter] = *temp;
}

/*
adds the a string to the data table
Parameters: currArg - a pointer to the current argument
Returns: TRUE if there were no errors
*/
boolean add_string(char * currArg)
{
    int i,j;

    currArg = strtok(NULL, " \t\n");
    if(!currArg) /*nothing after .string or incorrect delimiter*/
    {
        printf("ERROR - Line number %d: wrong syntax after .string\n", lineNumber);
        return FALSE;
    }
    if (currArg[0] == '\"') /*only if the wanted string start with "*/
    {
        for (i = 1; i < strlen(currArg); i++) /*add all the chars to data table*/
        {
            if (currArg[i] == '\"')
            {
                create_char_line(0);
                dataCounter++;
                break;
            }
            create_char_line(currArg[i]);
            dataCounter++;
        }
        for (j = i+1; j < strlen(currArg); j++) /*there are no other chars after " */
            if (!isspace(currArg[j]))
            {
                printf("ERROR - Line number %d: string must end with \" \n", lineNumber);
                return FALSE;
            }
        currArg = strtok(NULL, " \t\n");
        if ((currArg) && (!isspace(currArg[0]))) /*there are no other strings after the first*/
        {
            printf("ERROR - Line number %d: only one string is allowed after .string\n", lineNumber);
            return FALSE;
        }
    }
    else  /* the string doesnt start with " */
    {
        printf("ERROR - Line number %d: wrong syntax after .string\n", lineNumber);
        return FALSE;
    }
    return TRUE;
}

/*
adds the data numbers to the data table
Parameters: currArg - a pointer to the current argument
Returns: TRUE if there were no errors
*/
boolean add_data(char * currArg)
{
    int currNum, data;
    currArg = strtok(NULL, " , ");
    if(!currArg)/*nothing after .data or incorrect delimiter*/
    {
        printf("ERROR - Line number %d: wrong syntax after .data\n", lineNumber);
        return FALSE;
    }
    while ((currArg) && (!isspace(currArg[0])))/*as long as number was found*/
    {
        currNum = sscanf(currArg, "%d", &data);
        if(currNum > 0)
        {
            create_data_line(data);/*save data in data table*/
            dataCounter++;
        }
        else
        {
            printf("ERROR - Line number %d: invalid data\n", lineNumber);
            return FALSE;
        }
        currArg = strtok(NULL, " , ");/*get next number*/
    }
    return TRUE;
}

/*
gets label name and address of a line in the instructions table, adds a line to the extern table
Parameters: labelName, icAddress - address of a line in the instructions table
Returns: void
*/
void add_to_extern_table(char* labelName , int icAddress)
{
    externTable = realloc(externTable,((sizeof(externLine))*(externLabelCounter+1)));
    if (!externTable)
    {
        printf("ERROR - Line number %d: unable to allocate memory for new extern table line\n",
               lineNumber);
        exit(1);
    }
    strcpy(externTable[externLabelCounter].name, labelName);
    externTable[externLabelCounter].address = icAddress;
    externLabelCounter++;
}