#include "utils.h"
#include "code.h"

boolean process_code_first_pass(int * IC, memory_word * code_image,char * line,int line_idx,int line_number, char * file_name, char * cmd)
{
	int num_args, i = 0;
	char operands[MAX_OPERANDS][LINE_LENGTH];
	char args[LINE_LENGTH];
	boolean status;
	
	/* get arguments*/
	while (line[line_idx] != EOS)
		args[i++] = line[line_idx++];
	args[i] = EOS;
	remove_EOL(args);
	remove_white_space_end(args);
	/* parse arguments*/
	num_args = process_arguments(operands , args, line_number, file_name);
	if(num_args < 0)
		return FALSE;
	/* validate arguments by command */
	status = validate_arguments(cmd, operands, num_args, line_number, file_name);
	if(status)
		asm_to_machine_lang_codeing(IC, code_image, operands, cmd);
	return status;
}

void asm_to_machine_lang_codeing(int * IC, memory_word * code_image, char operands[MAX_OPERANDS][LINE_LENGTH], char * cmd)
{

	opcode op = get_opcode_by_name(cmd);
	addressing_type source_addr, target_addr;
	/* 2 operands */
	if(op == MOV || op == CMP || op == ADD || op == SUB || op == LEA)
	{
		source_addr = get_addressing_type(operands[0]);
		target_addr = get_addressing_type(operands[1]);
		add_first_mem_word(code_image, IC, source_addr, target_addr, op);
		
		/* if both operands are registers */
		if(source_addr == REGISTER_ADDR && target_addr == REGISTER_ADDR)
			add_extra_word_regs(code_image,IC,operands);
		else
		{
		add_extra_mem_word(code_image, IC, source_addr, operands[0], 0);
		add_extra_mem_word(code_image, IC, target_addr, operands[1], 1);
		}
	}
	/* 1 operand */
	else if(op == NOT || op == CLR || op == INC || op == DEC || op == JMP || op == BNE || op == GET || op == PRN || op == JSR)
	{
		target_addr = get_addressing_type(operands[0]);
		add_first_mem_word(code_image, IC, 0, target_addr, op);
		add_extra_mem_word(code_image, IC, target_addr, operands[0], 1);
	}
	/* no operands */
	else if(op == RTS || op == HLT)
	{
		add_first_mem_word(code_image, IC, 0, 0, op);
	}
	
}

void add_extra_word_regs(memory_word * code_image, int * IC, char operands[MAX_OPERANDS][LINE_LENGTH])
{
	code_image[*IC- IC_VAL].reg.source_addr = (operands[0][strlen(operands[0]) -1]) - '0';
	code_image[*IC- IC_VAL].reg.dest_addr = (operands[1][strlen(operands[1]) -1]) - '0';		
	code_image[*IC - IC_VAL].reg.ARE = 0;
	(*IC)++;
}
void add_extra_mem_word(memory_word * code_image, int * IC, addressing_type address, char * operand, int reg_type)
{	
 	if(address == IMMEDIATE_ADDR)
 	{
 		code_image[*IC- IC_VAL].number.num = atoi(operand +1);
		code_image[*IC - IC_VAL].number.ARE = 0;
		(*IC)++;
 	}
 	else if(address == DIRECT_ADDR)
 	{
 		(*IC)++;
 	}
 	else if(address == ENTRY_ADRR)
 	{
 	 	(*IC)++;
 	 	code_image[*IC- IC_VAL].number.num = (operand[strlen(operand)-1]) - '0';
		code_image[*IC - IC_VAL].number.ARE = 0;
		(*IC)++;
 	}
 	else if(address == REGISTER_ADDR)
 	{
 		if(reg_type == 0)
 		{
	 		code_image[*IC- IC_VAL].reg.source_addr = (operand[strlen(operand)-1]) - '0';
	 		code_image[*IC- IC_VAL].reg.dest_addr	= 0;	
			code_image[*IC - IC_VAL].reg.ARE = 0;
		(*IC)++;	
 		}
 		else if(reg_type == 1)
 		{ 
	 		code_image[*IC- IC_VAL].reg.source_addr = 0;
	 		code_image[*IC- IC_VAL].reg.dest_addr	= (operand[strlen(operand) -1]  - '0');		
			code_image[*IC - IC_VAL].reg.ARE = 0;
			(*IC)++;
		}
 	}		
}

void add_first_mem_word(memory_word * code_image, int * IC, addressing_type source_addr, addressing_type target_addr, opcode op)
{
	code_image[*IC- IC_VAL].machine_word.opcode = op;
	code_image[*IC - IC_VAL].machine_word.source_addr = source_addr;
	code_image[*IC - IC_VAL].machine_word.dest_addr = target_addr;
	code_image[*IC - IC_VAL].machine_word.ARE = 0;
	(*IC)++;	
}


boolean validate_arguments(char * cmd, char operands[MAX_OPERANDS][LINE_LENGTH], int num_args, int line_number, char * file_name)
{
	opcode curr_opcode;

	curr_opcode = get_opcode_by_name(cmd);
	/* check if command is defined */
	if(curr_opcode == ERROR_OP)
	{
		print_error(line_number, "Undefined command", file_name);
		return FALSE;
	}
	if(!validate_args_by_opcode(curr_opcode, operands, num_args, line_number, file_name))
		return FALSE;
	return TRUE;
}




boolean validate_args_by_opcode(opcode curr_opcode, char operands[MAX_OPERANDS][LINE_LENGTH], int num_args, int line_number, char * file_name)
{
	int i;
	if(!validate_num_of_operands(num_args, curr_opcode, line_number, file_name))
		return FALSE;
	for(i = 0; i < num_args; i++)
	{
		if(get_addressing_type(operands[i]) == ERROR)
		{
			print_error(line_number, "Invalid operand", file_name);
			return FALSE;
		}
	}
	if(!validate_address(operands, num_args, curr_opcode, line_number, file_name))
	{
		print_error(line_number, "Addressing type does not match the command" ,file_name);
		return FALSE;
	}
	return TRUE;
}

boolean validate_address(char operands[MAX_OPERANDS][LINE_LENGTH], int num_args, opcode curr_opcode, int line_number, char * file_name)
{
	switch(curr_opcode) 
	{
		case MOV:
			if(get_addressing_type(operands[1]) == 0)
				return FALSE;
			break;
		case CMP:
				return TRUE;
		case ADD:
				if(get_addressing_type(operands[1]) == 0)
					return FALSE;
				break;
		case SUB:
				if(get_addressing_type(operands[1]) == 0)
					return FALSE;
				break;
		case NOT:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case CLR:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case LEA:
				if((get_addressing_type(operands[0]) == 0) || (get_addressing_type(operands[0]) == 3) || (get_addressing_type(operands[1]) == 0))
					return FALSE;
				break;
		case INC:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case DEC:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case JMP:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case BNE:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case GET:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case PRN:
				return TRUE;
		case JSR:
				if(get_addressing_type(operands[0]) == 0)
					return FALSE;
				break;
		case RTS:
				return TRUE;
		case HLT:
				return TRUE;
		case ERROR_OP:
				return FALSE;
		}
		return TRUE;	
}		
					

boolean validate_num_of_operands(int num_args, opcode curr_opcode, int line_number, char * file_name)
{
	/* check if num of operands is valid */
	if(num_args > 2)
	{
		print_error(line_number, "Too many operands", file_name);
		return FALSE;
	}
	if(((curr_opcode == MOV || curr_opcode == CMP || curr_opcode == ADD || curr_opcode == SUB || curr_opcode == LEA) && (num_args != 2))
		|| ((curr_opcode == NOT || curr_opcode == CLR || curr_opcode == INC || curr_opcode == DEC || curr_opcode == JMP || curr_opcode == BNE
		|| curr_opcode == GET || curr_opcode == PRN || curr_opcode == JSR) && (num_args != 1)) 
		|| ((curr_opcode == RTS || curr_opcode == HLT) && (num_args != 0))){
			print_error(line_number, "Number of operands does not match the command", file_name);
			return FALSE;
			}
	return TRUE;
}



opcode get_opcode_by_name(char * cmd)
{
	cmd_opcode cmd_opcode_table[NUM_OF_OPERATIONS] = {
		{"mov", MOV},
		{"cmp",CMP},
		{"add",ADD},
		{"sub",SUB},
		{"not",NOT},
		{"clr",CLR},
		{"lea",LEA},
		{"inc",INC},
		{"dec",DEC},
		{"jmp",JMP},
		{"bne",BNE},
		{"get",GET},
		{"prn",PRN},
		{"jsr",JSR},
		{"rts",RTS},
		{"hlt",HLT}
	};
	int i;
	for(i = 0; i < NUM_OF_OPERATIONS; i++)
	{
		if(strcmp(cmd, cmd_opcode_table[i].cmd) == 0)
			return cmd_opcode_table[i].op;
	}
	/* coomand is invalid */
	return ERROR_OP;
}




addressing_type get_addressing_type(char * operand)
{
	if((operand[0] == '#') && (validate_number(operand + 1)))
		return IMMEDIATE_ADDR;
	if(is_register(operand))
		return REGISTER_ADDR;
	if(strrchr(operand, '.'))
	{
		int i = 0, j = 0;
		char struct_label[30];
		while(i < (strrchr(operand, '.') - operand))
			struct_label[j++] = operand[i++];
		struct_label[j] = EOS;
			if((operand[strlen(operand) -1] == '1' || operand[strlen(operand) -1] == '2') && operand[strlen(operand) -2] == '.' && validate_label(struct_label))
				return ENTRY_ADRR;
	}
	if(validate_label(operand))
		return DIRECT_ADDR;
	/* no addressing type was found, error*/
	return ERROR;
}






