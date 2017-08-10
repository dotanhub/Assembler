assembler : fileswriting.o validations.o main.o parcer.o assembler.o  fileswriting.h validations.h parcer.h assembler.h defenitions.h datastructures.h
	gcc -g -ansi -Wall fileswriting.o validations.o main.o parcer.o assembler.o fileswriting.h validations.h parcer.h assembler.h -lm -pedantic -o assembler

fileswriting : fileswriting.c fileswriting.h
	gcc -c  -g -ansi fileswriting.c -lm -Wall -pedantic -o fileswriting.o 

validations.o : validations.c validations.h
	gcc -c  -g -ansi validations.c -lm -Wall -pedantic -o validations.o 

parcer.o : parcer.c parcer.h
	gcc -c  -g -ansi parcer.c -lm -Wall -pedantic -o parcer.o 

assembler.o : assembler.c assembler.h
	gcc -c -g -ansi assembler.c -lm -Wall -pedantic -o assembler.o 

main.o : main.c
	gcc -c  -g -ansi main.c -lm -Wall -pedantic -o main.o 


