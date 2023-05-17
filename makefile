run: main.o macro.o utils.o table.o data.o code.o first_pass.o second_pass.o output_files.o
	gcc -ansi -Wall -pedantic -g main.o macro.o utils.o table.o data.o code.o first_pass.o second_pass.o output_files.o -o run
main.o: main.c
	gcc -c -ansi -Wall -pedantic -g main.c -o main.o
macro.o: macro_process.c macro_process.h 
	gcc -c -ansi -Wall -pedantic -g macro_process.c -o macro.o
utils.o: utils.c utils.h
	gcc -c -ansi -Wall -pedantic -g utils.c -o utils.o
table.o: table.c table.h
	gcc -c -ansi -Wall -pedantic -g table.c -o table.o
data.o: data.c data.h
	gcc -c -ansi -Wall -pedantic -g data.c -o data.o
code.o: code.c code.h
	gcc -c -ansi -Wall -pedantic -g code.c -o code.o
first_pass.o: first_pass.c first_pass.h
	gcc -c -ansi -Wall -pedantic -g first_pass.c -o first_pass.o
second_pass.o: second_pass.c second_pass.h
	gcc -c -ansi -Wall -pedantic -g second_pass.c -o second_pass.o
output_files.o: output_files.c output_files.h
	gcc -c -ansi -Wall -pedantic -g output_files.c -o output_files.o
	

