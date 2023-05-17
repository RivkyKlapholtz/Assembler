#ifndef _OUTPUT_FILES_H
#define _OUTPUT_FILES_H




/*
 @param symbol_table symbol table 
 @param external_table the table of all externals & addresses 
 @param data_image data image
 @param code_image code image
 @param external_table the table of all externals & addresses 
 @param IC the instraction counter 
 @param DC the data counter
 @param file_name the name of .am file

*/
void write_output_files(symbol ** symbol_table,int * data_image ,memory_word * code_image ,externals ** external_table, int IC, int DC, char * file_name);

/*
 convert decimal number to base-32 number
 @param num the given decimal number
 @param result the converted number
*/
void dec_to_32_base(int num, char * result);

/*
 write the object file
 @param IC the instraction counter 
 @param DC the data counter
 @param file_name the name of .am file
 @param data_image data image
 @param code_image code image
*/
void write_object(int IC,int DC, char * file_name, int * data_image, memory_word * code_image);

/*
 write the entries file
 @param symbol_table symbol table  
 @param file_name the name of .am file
*/
void write_ent(symbol ** symbol_table,char * file_name);

/*
 write the externs file
 @param external_table the table of all externals & addresses
 @param file_name the name of .am file
*/
void write_ext(externals ** external_table, char * file_name);

#endif
