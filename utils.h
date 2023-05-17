#ifndef _UTILS_H
#define _UTILS_H


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include "globals.h"

/* -------- defines ---------- */

#define INVALID -1
#define SPACE	' '
#define TAB		'\t'
#define EOS		'\0'
#define EOL		'\n'
#define COMMA ','

 
/* --------- functions ----------*/


/*
 return true if the given char is a white space
 @param c the char
 @return true if the char is white space
*/
int is_white_space(char);

/*
 get a word from a line
 @param line the line 
 @param word insert the first word into this
 @param i the index of the line
 @return the cuur index of the line
*/
int get_word(char *, char *, int);

/*
 print error mssage & the error line
 @param line the line number 
 @param error the error to print
 @param  file_name the name of the file where the error is
*/
void print_error(int, char *, char *);


/*
 return true if the given string is a assembly word
 @param str the string
 @return true if the string is a assembly word
*/
int is_languge_word(char * str);


/*
 remove the '\n' from the end of a line
 @param line the line
*/
void remove_EOL(char *);



/*
 remove the white spaece from the end of a line
 @param line the line
*/
void remove_white_space_end(char *);


/*
 remove the white spaece from the end of a line
 @param operands insert the proccesed arguments into this
 @param args the given string to process
 @param line_number the line number of the .am file
 @param file_name the .am file name
*/
int process_arguments(char [LINE_LENGTH][LINE_LENGTH], char * args, int line_number, char * file_name);

/*
 check if the a number is ligall
 @param num the number
 @return true if the number is o.k.
*/
boolean validate_number(char * num);

/*
 check if the symbol name is ligall
 @param symbol_name the name
 @return true if the name is o.k.
*/
boolean validate_label(char * );


/*
	check if operand is a register
    @param operand the operand
    @return true if operand is a register
*/
boolean is_register(char * operand);

/*
 free all memory allocation in externals tabel
 @param list the table
*/
void free_extern(externals ** list);


#endif
