prog: prog.o macro.o data.o label.o first_run.o second_run.o utilities.o export.o
	gcc -ansi -Wall -pedantic -o prog prog.o macro.o data.o label.o utilities.o first_run.o second_run.o export.o -lm

prog.o: prog.c macro.h errors.h data.h first_run.h utilities.h
	gcc -c -ansi -Wall -pedantic prog.c

macro.o: macro.c macro.h errors.h data.h utilities.h
	gcc -c -ansi -Wall -pedantic macro.c

data.o: data.c data.h
	gcc -c -ansi -Wall -pedantic data.c

label.o: label.c label.h
	gcc -c -ansi -Wall -pedantic label.c

first_run.o: first_run.c first_run.h errors.h data.h label.h second_run.h utilities.h
	gcc -c -ansi -Wall -pedantic first_run.c

second_run.o: second_run.c second_run.h errors.h data.h label.h utilities.h export.h
	gcc -c -ansi -Wall -pedantic second_run.c

utilities.o: utilities.c utilities.h errors.h data.h
	gcc -c -ansi -Wall -pedantic utilities.c

export.o: export.c export.h errors.h label.h utilities.h
	gcc -c -ansi -Wall -pedantic export.c
