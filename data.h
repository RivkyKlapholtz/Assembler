#ifndef _DATA_H
#define _DATA_H


/*********** functions *************/

/*
 check if the word is data instruction
 @param word the instruction
 @return true if the instrction is o.k.
*/
boolean validate_data(char *);

/*
  check if the word is entry/extern 
 @param word the instruction
 @return true if the instrction is o.k.
*/
boolean validate_ent(char *);

/*
 process a data sentence 
 @param DC data counter
 @param line the current line
 @param data the curr data word to procces 
 @param data_image data image
 @param line_idx the index of the line
 @param line_number the line number of the .am file
 @param file_name the .am file name
 @return true if the process done succesfully
*/
boolean process_data(int * DC, char * line, char * data, int * data_image, int line_idx, int line_number, char *file_name);

/*
 process a string sentence 
 @param DC data counter
 @param line the current line
 @param data the curr data word to procces 
 @param data_image data image
 @param line_idx the index of the line
 @param line_number the line number of the .am file
 @param file_name the .am file name
 @return true if the process done succesfully
*/
boolean process_string(int * DC, char * line, char * data, int * data_image, int line_idx, int line_number, char *file_name);

/*
 process a data sentence 
 @param DC data counter
 @param line the current line
 @param data the curr data word to procces 
 @param data_image data image
 @param line_idx the index of the line
 @param line_number the line number of the .am file
 @param file_name the .am file name
 @return true if the process done succesfully
*/
boolean process_struct(int * DC, char * line, char * data, int * data_image, int line_idx, int line_number, char *file_name);

/*
 process an extern sentence 
 @param table the symbol table
 @param symbol_name the name of the external label
 @param data_image data image
 @param line_idx the index of the line
 @param line_number the line number of the .am file
 @param file_name the .am file name
 @return true if the process done succesfully
*/
boolean extern_process(symbol *, char * symbol_name, int line_idx, char * line, int line_number, char *file_name);

/*
 check if a string is valid
 @param line_idx the index of the line
 @param line_number the line number of the .am file
 @param file_name the .am file name
 @return if the string is good
*/
int validate_string(char * string, int line_number, char * file_name);


#endif
