#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H



/*
	process a line of the target file, second pass
    @param symbol_table the symbol table
    @param IC instruction counter
    @param line the current line content
    @param line_number the line number of the .am file
 	@param file_name the .am file name
    @param code_image code image
    @param external_table table of extern symbols
    @return true if the process done succesfully 
*/
boolean process_line_nd_pass(symbol ** symbol_table, int * IC, char * line,  int line_number, char * file_name, memory_word * code_image, externals ** external_table);


/*
	check if the command is a data command
    @param curr_word the command to check
    @return true if the command is a data command
*/
boolean is_data(char * curr_word);


/*
	update the entry symbols in the symbol table
    @param table symbil table
    @param symbol_name the name of the symbol
    @return true if the update done successfully
*/
boolean update_entry(symbol ** table, char * symbol_name);


/*
	procces instructions command, first pass
    @param symbol_table the symbol table
    @param IC instruction counter
    @param line the current line content
    @param line_number the line number of the .am file
 	@param file_name the .am file name
    @param code_image code image
    @param cmd the command
    @param external_table table of extern symbols
    @return true if process done succesfully 
*/
boolean process_code_second_pass(symbol ** symbol_table, int * IC, char * line, int line_idx, int line_number, char * file_name, memory_word * code_image, char * cmd, externals ** external_table);


/*
	codeing the remaining words in the second pass
    @param instruction counter
    @param code_image code image
    @param operands the arguments of command 
    @param op command opcode
    @param symbol_table the symbol table
    @param line_number the line number of the .am file
 	@param file_name the .am file name
    @param external_table table of extern symbols
    @return true if coding done succesfully  
*/
boolean codeing_words_second_pass(int * IC, memory_word * code_image, char operands[MAX_OPERANDS][LINE_LENGTH], opcode op, symbol ** symbol_table, int line_number, char * file_name, externals ** external_table);

/*
	add memory word of each instruction, if one operand is symbol
    @param symbol_table symbol table
    @param opreand the operand
    @param code_image code image
	@param IC instructions counter
	@param address the addressing type of operand 
    @param external_table table of extern symbols
    @return true if adding done succesfully  urns 
*/
boolean add_mem_word_nd_pass(symbol ** symbol_table, char * operand, memory_word * code_image, int * IC, addressing_type address, externals ** external_table);

/*
	get the type of symbol from symbol table
    @param operand the operand
    @param table symbol table
    @return the type
*/
type get_type_from_table(char * operand, symbol * table);

/*
	get the address of symbol from symbol table
    @param operand the operand 
    @param table symbol table
    @return the address
*/
int get_symbol_address(char * operand, symbol * table);

/*
	add an extern symbol to externals table
    @param head the externals table
    @param name the name of the symbols to add
    @param IC instructions counter
*/
void add_external(externals ** head, char * name, int * IC);

#endif
