#ifndef _CODE_H
#define _CODE_H




/*********** functions *************/

/*
	procces instructions command, first pass
    @param IC instruction counter
    @param code_image code image
    @param line current line content
    @param line_idx the current index of line
    @param line_number the .am line number
    @param file_name the name of the .am file
    @param cmd the command
    @returns true if process done succesfully
*/
boolean process_code_first_pass(int * IC, memory_word * code_image,char * line,int line_idx,int line_number, char * file_name, char * cmd);

/*
	validate the arguments of the current instruction
    @param cmd the command
    @param operands the arguments of the command
    @param num_args the total number of arguments
    @param line_number the .am line number
    @param file_name  the name of the .am file
    @return true if argumanets are valid
*/
boolean validate_arguments(char * cmd, char operands[MAX_OPERANDS][LINE_LENGTH], int num_args, int line_number, char * file_name);

/*
	validtae the arguments according to the opcode
    @param curr_opcode the opcode of the command
    @param operands the arguments of the command
    @param num_args the total number of arguments
    @param line_number the .am line number
    @param file_name the name of the .am file
    @return true if arguments are valid according to the opcode 
*/
boolean validate_args_by_opcode(opcode curr_opcode, char operands[MAX_OPERANDS][LINE_LENGTH], int num_args, int line_number, char * file_name);

/*
	get opcode by name of command
    @param cmd the command
    @return the command's opcode
*/
opcode get_opcode_by_name(char * cmd);

/*
	validte num of opernds according the opcode
    @param num_args total number of arguments
    @param curr_opcode the opcode
    param line_number the .am line number
    @param file_name the name of the .am file
    @return true if num of arguments is valid according to the opcode
*/
boolean validate_num_of_operands(int num_args, opcode curr_opcode, int line_number, char * file_name);


/*
	get addressing type of operand
    @param operand the given operand
    @return the addressing type of operand
*/
addressing_type get_addressing_type(char * operand);

/*
	validate the addressing type of the operands according the opcode
    @param operands the operands to check
    @param num_args total num of operands
    @param curr_opcode the opcode
    @param line_number the .am line number
    @param file_name the name of the .am file
    @return true if  the addressing type of the operands is valid
*/
boolean validate_address(char operands[MAX_OPERANDS][LINE_LENGTH], int num_args, opcode curr_opcode, int line_number, char * file_name);

/*
	add the first memory word of each instruction
    @param code_image code image
    @param source_addr the addressing type of the first operand
    @param target_addr the addressing type of the second operand
    @param op the opcode
*/
void add_first_mem_word(memory_word * code_image, int * IC, addressing_type source_addr, addressing_type target_addr, opcode op);

/*
	add extra memory word of each instruction, if needed by opcode
    @param code_image code image
    @param IC the instruction counter
    @param address the addressing type of the operand
    @param operand the operand
    @param reg_type type of register - sourse or target
*/
void add_extra_mem_word(memory_word * code_image, int * IC, addressing_type address, char * operand, int reg_type);

/*
	add extra memory word if given operands are both resisters
    @param code_image code image
    @param IC instruction counter
    @param operands the given operands
*/
void add_extra_word_regs(memory_word * code_image, int * IC, char operands[MAX_OPERANDS][LINE_LENGTH]);

/*
	codeing the instructions to memory
    @param IC instruction counter
    @param code_image code image
    @param operands the given operands
    @param cmd command
*/
void asm_to_machine_lang_codeing(int * IC, memory_word * code_image, char operands[MAX_OPERANDS][LINE_LENGTH], char * cmd);

#endif

