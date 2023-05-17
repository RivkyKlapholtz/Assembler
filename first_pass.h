#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H


/* 
 process a line of the target file, first pass
 @param symbol_table the symbol table
 @param IC instruction counter
 @param DC data counter
 @param line the current line
 @param data_image data image
 @param line_idx the index of the line
 @param line_number the line number of the .am file
 @param file_name the .am file name
 @param code_image code image
 @return true if the process done succesfully

 */
boolean process_line_st_pass(symbol ** symbol_table, int * IC, int * DC, char * line, int line_number, char * file_name, int * data_image, memory_word * code_image);

/* 
 check if the line is a comment line
 @param line the given line
 @return true if the line is a comment line
*/
boolean is_comment_line(char *);


/* 
 check if the line is an empty line
 @param line the given line
 @return true if the line is an empty line
*/
boolean is_empty_line(char *);

/*
 check if the given word is a label
 @param word the given word
 @param the symbol name 
 @return true if the word is a label
*/
boolean find_label(char *, char *);



/*
 update the real address of all data labels 
 @param table the symbol table
 @param IC the instruction counter
 @return if the string is good
*/
void update_dc(symbol ** table, int * IC);


#endif







