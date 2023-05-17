#include "utils.h"
#include "table.h"
#include "data.h"
#include "code.h"
#include "first_pass.h"

boolean process_line_st_pass(symbol ** symbol_table, int * IC, int * DC, char * line, int line_number, char * file_name, int * data_image, memory_word * code_image)
{
	char curr_word[LINE_LENGTH];
	char symbol_name[LABEL_LEN];
	boolean label = FALSE;
	int line_idx = 0;
	char * data;
	type symbol_type;
	
	
	line_idx = get_word(line, curr_word, line_idx);
	/* if the line is empty / comment*/
	if(is_comment_line(line) || is_empty_line(line))
		return TRUE;
	/* there is a label*/
	if(find_label(curr_word , symbol_name))
	{
		label = TRUE;
		/* check if there is an error in symbol name */
		if(strlen(curr_word) > LABEL_LEN)
		{
			print_error(line_number, "Label name is too long", file_name);
			return FALSE;
		}
		if(!validate_label(symbol_name))
		{
			print_error(line_number, "Invalid label name", file_name);
			return FALSE;
		}
		/* check if the symbol is allready exists */
		if(find_symbol_by_name(*symbol_table, symbol_name))
		{
			print_error(line_number, "Label allready exist", file_name);
			return FALSE;
		}
	}
	/* if there is no label, start from begining*/
	if(!label)
		line_idx = 0;
	/* get next word*/
	line_idx = get_word(line, curr_word, line_idx);
	/* if the label is set alone on a line - error */
	if(strcmp(curr_word, "") == 0)
	{
			print_error(line_number, "Label has no instruction", file_name);
			return FALSE;
	}
	/* ------------- data instructions ------------- */
	if(curr_word[0] == '.')
	{
		data = curr_word + 1;
		/* check if the command is data, string or struct */
		if(validate_data(data))
		{
			symbol_type = DATA;
			add_symbol(symbol_table,symbol_name, *DC, symbol_type);
			if(strcmp(data, "data") == 0)
			{
				if(!process_data(DC, line, data, data_image, line_idx, line_number, file_name))
					return FALSE;
			}
			if(strcmp(data, "string") == 0)
			{
				if(!process_string(DC, line, data, data_image, line_idx, line_number, file_name))
					return FALSE;
			}
			if(strcmp(data, "struct") == 0)
			{
				if(!process_struct(DC, line, data, data_image, line_idx, line_number, file_name))
					return FALSE;
			}
		}
		/* check if the command is extern */
		else if(validate_ent(data))
		{
			if(strcmp(data, "extern") == 0)
			{
				if(!extern_process(*symbol_table, symbol_name, line_number, line, line_idx, file_name))
					return FALSE;
				else
				{
					symbol_type = EXTERN;
					add_symbol(symbol_table, symbol_name, 0 , symbol_type);
				}			
			}
		}
		else
		{
			print_error(line_number, "Undefined command", file_name);
			return FALSE;
		}
				
	}	
	/* ------------- code instructions ------------- */
	else
	{
		if(label)
		{
			symbol_type = CODE;
			add_symbol(symbol_table, symbol_name, *IC , symbol_type);
		}
		/* procces code instruction*/
		return process_code_first_pass(IC, code_image, line, line_idx, line_number,file_name, curr_word);
	}
	/* no errors found */
	return TRUE;	
}

void update_dc(symbol ** table, int * IC)
{
	symbol * h1 = * table;
	while(h1!= NULL)
	{
		if(h1->symbol_type == DATA)
		{
			h1->address = ((*IC) + (h1->address));
		}
		h1 = (symbol *)h1->next;
	}
}


boolean is_comment_line(char * line)
{
	int i = 0;
	while (is_white_space(line[i]) && line[i] != EOS)
			i++;
	if(line[i] == ';')
		return TRUE;
	return FALSE;
}

boolean is_empty_line(char * line)
{
	int i = 0;
	while(line[i] != EOL && line[i] != 13 && line[i] != EOS)
	{
		if(!is_white_space(line[i])) 
			return FALSE;
		i++;
	}
	return TRUE;
}

boolean find_label(char * word, char * symbol_name)
{
	char tmp_word[LINE_LENGTH];
	strcpy(tmp_word, word);

	if(tmp_word[strlen(tmp_word)-1] == ':')
	{
		int i = 0, j = 0; 
		while (tmp_word[i] != EOS && tmp_word[i] != ':' && i < LABEL_LEN -1)		
			symbol_name[j++] = tmp_word[i++];
		symbol_name[j] = EOS;
		return TRUE;
	}
	return FALSE;
	
}



	
	
