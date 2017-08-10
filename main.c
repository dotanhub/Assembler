
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembler.h"

/*
int main(int argc, char *argv[])
receives file names, adds extensions, perform assembly
Parameters:argc, argv
Return: Non.
*/
int main(int argc, char *argv[])
{
    char *filePath;
    int i;

    if (argc == 1)
    {
        printf("Error - Missing arguments, At least one Input Data File is needed!\n");
        return EXIT_FAIL;
    }

    for(i=1; i < argc; i++)
    {
        filePath = (char*) malloc ( (int)strlen(argv[i]) + (int)strlen(AS_EXTENSION) + 1);
        strcpy (filePath, argv[i]);
        strcat (filePath, AS_EXTENSION);
        import_from_file(filePath);/*start assembler*/
        free(filePath);
    }
    return EXIT_SUCCEED;
}

