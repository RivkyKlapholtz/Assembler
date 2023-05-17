
#include "utils.h"
#include "table.h"
#include "code.h"
#include "second_pass.h"
#include "first_pass.h"


boolean process_line_nd_pass(symbol ** symbol_table, int * IC, char * line,  int line_number, char * file_name, memory_word * code_image, externals ** external_table)
{
	char curr_word[LINE_LENGTH];
	char symbol_name[LABEL_LEN];
	int line_idx = 0;
	

	if(is_comment_line(line) || is_empty_line(line))
		return TRUE;
	/* get the first word */
	line_idx = get_word(line, curr_word, line_idx);
	if(find_label(curr_word , symbol_name))
		line_idx = get_word(line, curr_word, line_idx);
	if(is_data(curr_word))
		return TRUE;
	else if(strcmp(".entry", curr_word) == 0)
	{
		line_idx = get_word(line, symbol_name, line_idx);
		/* check if there are more arguments */
		while(line[line_idx] != EOS)
		{
			if((!is_white_space(line[line_idx])) && line[line_idx] != '\n')
				{
					print_error(line_number, ".entry instruction must include an exactly one argument", file_name);
					return FALSE;
				}
			line_idx++;
		}
		/* if there ia no operand */
		if(strcmp("", symbol_name) == 0)
		{
			print_error(line_number, ".entry instruction must include an argument", file_name);
			return FALSE;
		}
		/* mark all entry symbols in table */
		else
		{
			if(!update_entry(symbol_table, symbol_name))
			{
				print_error(line_number, ".entry symbol was not found", file_name);
					return FALSE;
			}
			return TRUE;
		}
	}
	else
	{
		process_code_second_pass(symbol_table, IC, line,line_idx, line_number, file_name,code_image, curr_word, external_table);
	}
	return TRUE;
}


boolean process_code_second_pass(symbol ** symbol_table, int * IC, char * line, int line_idx, int line_number, char * file_name, memory_word * code_image, char * cmd, externals ** external_table)
{
	int i = 0;
	char operands[MAX_OPERANDS][LINE_LENGTH];
	char args[LINE_LENGTH];
	boolean status;
	opcode opcode = get_opcode_by_name(cmd);
	
	/* get arguments*/
	while (line[line_idx] != EOS)
		args[i++] = line[line_idx++];
	args[i] = EOS;
	remove_EOL(args);
	remove_white_space_end(args);
	/* parse arguments*/
	process_arguments(operands , args, line_number, file_name);
	status = codeing_words_second_pass(IC, code_image, operands, opcode, symbol_table, line_number, file_name, external_table);
	return status;
}

boolean codeing_words_second_pass(int * IC, memory_word * code_image, char operands[MAX_OPERANDS][LINE_LENGTH], opcode op, symbol ** symbol_table, int line_number, char * file_name,externals ** external_table)
{
	addressing_type source_addr, target_addr;
	boolean status1 = TRUE, status2 = TRUE;
	
	/* skip the first memory word */
	(*IC)++;
	/* 2 operands instructions  */
	if(op == MOV || op == CMP || op == ADD || op == SUB || op == LEA)
	{
		source_addr = get_addressing_type(operands[0]);
		target_addr = get_addressing_type(operands[1]);
		/* if both operands are registers */
		if(source_addr == REGISTER_ADDR && target_addr == REGISTER_ADDR)
			(*IC)++;
		else
		{
			if(source_addr == REGISTER_ADDR || source_addr == IMMEDIATE_ADDR)
				(*IC)++;
			if(source_addr == DIRECT_ADDR || source_addr == ENTRY_ADRR)
				status1 = add_mem_word_nd_pass(symbol_table, operands[0], code_image, IC, source_addr, external_table);
			if(target_addr == REGISTER_ADDR || target_addr == IMMEDIATE_ADDR)
				(*IC)++;
			if(target_addr == DIRECT_ADDR || target_addr == ENTRY_ADRR)
				status2 = add_mem_word_nd_pass(symbol_table, operands[1], code_image, IC, target_addr, external_table);
		}
	}
	/* 1 operand instructions */
	else if(op == NOT || op == CLR || op == INC || op == DEC || op == JMP || op == BNE || op == GET || op == PRN || op == JSR)
	{

		source_addr = get_addressing_type(operands[0]);
		if(source_addr == REGISTER_ADDR || source_addr == IMMEDIATE_ADDR)
				(*IC)++;
		else if(source_addr == DIRECT_ADDR || source_addr == ENTRY_ADRR)
			status1 = add_mem_word_nd_pass(symbol_table, operands[0], code_image, IC, source_addr, external_table);
	}
	/* if the symbol was not found */
	if((!status1) || (!status2))
		print_error(line_number, "Label is undefined", file_name);
	return status1 &&  status2;
}

boolean add_mem_word_nd_pass(symbol ** symbol_table, char * operand, memory_word * code_image, int * IC, addressing_type address, externals ** external_table)
{
	int symbol_addr, are, i = 0;
	type symbol_type;
	char struct_name[LINE_LENGTH];
	if(address == DIRECT_ADDR)
	{
		symbol_addr = get_symbol_address(operand, *symbol_table);
		if(symbol_addr < 0)
			return FALSE;
		else
		{
			symbol_type = get_type_from_table(operand, *symbol_table);
			if(symbol_type == EXTERN)
			{
				add_external(external_table, operand, IC);
				are = 1;
			}
			else
				are = 2;
			code_image[*IC- IC_VAL].address.addr = symbol_addr;
			code_image[*IC - IC_VAL].address.ARE = are;
			(*IC)++;
		}
	}
	else if(address == ENTRY_ADRR)
	{
		while(operand[i] != '.')
		{
			struct_name[i] = operand[i];
			i++;
		}
		struct_name[i] = EOS;
		symbol_addr = get_symbol_address(struct_name, *symbol_table);
		if(symbol_addr < 0)
			return FALSE;
		else
		{
			symbol_type = get_type_from_table(struct_name, *symbol_table);
			if(symbol_type == EXTERN)
			{
				add_external(external_table, struct_name, IC);
				are = 1;
			}
			else
				are = 2;
			code_image[*IC- IC_VAL].address.addr = symbol_addr;
			code_image[*IC - IC_VAL].address.ARE = are;
			(*IC)+=2;
		}
	
	}
	return TRUE;
}


void add_external(externals ** head, char * name, int * IC)
{

	externals * h = * head;
	externals * new_symbol;
	
	new_symbol = (externals *)malloc(sizeof(externals));
	if(!new_symbol)
	{
		printf("memory allocation faild");
		exit(0);
	}
	strcpy(new_symbol->name ,name);
	new_symbol->address = *IC;
	new_symbol->next = NULL;
	 /* the first item of the table */
	if(h == NULL)
	{
		*head = new_symbol;
	}
	/* add the symbol in the end of the list*/
	else
	{
		while(h->next != NULL)
			h = (externals *)h->next;
		h->next =(ex_ptr)new_symbol;
	}
}


type get_type_from_table(char * operand, symbol * table)
{
	symbol * h1;
	h1 = table;
	while(h1!=NULL)
	{
		if(strcmp(operand, (h1->name)) == 0)
		{
			return h1->symbol_type;
		}
		h1 = (symbol *)h1->next;
	}
	return DATA;
}


int get_symbol_address(char * operand, symbol * table)
{
	symbol * h1;
	h1 = table;
	while(h1!=NULL)
	{
		if(strcmp(operand, (h1->name)) == 0)
		{
			return h1->address;
		}
		h1 = (symbol *)h1->next;
	}
	return INVALID;		

}


boolean update_entry(symbol ** table, char * symbol_name)
{
	boolean found = FALSE;
	symbol * h1 = * table;
	while(h1!= NULL)
	{
		if(strcmp(h1->name, symbol_name) == 0)
		{
			found = TRUE;
			h1->symbol_type = ENTRY;
		}
		h1 = (symbol *)h1->next;
	}
	return found;
}





boolean is_data(char * curr_word)
{
	if(strcmp(".data", curr_word) == 0 || strcmp(".string", curr_word) == 0 || strcmp(".struct", curr_word) == 0 || strcmp(".extern", curr_word) == 0)
		return TRUE;
	return FALSE;
}
