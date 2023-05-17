#include "utils.h"
#include "table.h"
#include "data.h"


boolean validate_data(char * word)
{
	if(strcmp(word, "data") == 0 ||  strcmp(word, "struct") == 0 || strcmp(word, "string") == 0)
		return TRUE;

	return FALSE;
}

boolean validate_ent(char * word)
{
	if((strcmp(word, "extern") == 0) || (strcmp(word, "entry") == 0))
		return TRUE;
	return FALSE;
}


boolean process_data(int * DC, char * line, char * data, int * data_image, int line_idx, int line_number, char *file_name)
{
	int i = 0, j = 0;
	int num_args;
	char args[LINE_LENGTH][LINE_LENGTH];
	char numbers[LINE_LENGTH];
	
	/* get the arguments */
	while (line[line_idx] != EOS)
		numbers[i++] = line[line_idx++];
	numbers[i] = EOS;
	remove_EOL(numbers);
	remove_white_space_end(numbers);
	
	/* get the num of arguments and procces them */
	num_args = process_arguments(args, numbers, line_number, file_name);
	/* check missing argument */
	if (num_args < 0)
		return FALSE;
	if (num_args == 0)
	{
		print_error(line_number, "Data instruction must include at least one data item", file_name);
		return FALSE;
	}
	for(j = 0; j < num_args; j++)
	{
		if(!validate_number(args[j]))
		{
			print_error(line_number, "Invalid number", file_name);
			return FALSE;
		}
	}
	/* insert into data image, and update DC*/
	for(j = 0; j < num_args; j++)
	{
		*(data_image + *DC) = atoi(args[j]);
		(*DC)++;
	}
	return TRUE;	
}



boolean process_string(int * DC, char * line, char * data, int * data_image, int line_idx, int line_number, char *file_name)
{
	int i = 0, j = 0;
	char string[LINE_LENGTH];

	
	while(is_white_space(line[line_idx]))
		line_idx++;
	while (line[line_idx] != EOS)
		string[i++] = line[line_idx++];
	string[i] = EOS;
	remove_EOL(string);
	remove_white_space_end(string);
	{
		if(!validate_string(string, line_number, file_name))
			return FALSE;
	}
	/* insert into data image, and update DC */
	for(j = 1; j < strlen(string) -1; j++)
	{
		*(data_image + *DC) = (int)(string[j]);
		(*DC)++;
	}
	*(data_image + *DC) = 0;
	(*DC)++;
	return TRUE;	
}	
	
int validate_string(char * string, int line_number, char * file_name)
{
	/* no string definition */
	if((strlen(string) == 0))
	{
		print_error(line_number, "Missing string definition", file_name);
		return FALSE;
	}
	/* invalid string */
	if((!(string[0] == '"' && string[strlen(string)-1] == '"')) || (strlen(string) == 1))
	{
		print_error(line_number, "Invalid string definition", file_name);
		return FALSE;
	}
	return TRUE;
}
	

boolean process_struct(int * DC, char * line, char * data, int * data_image, int line_idx, int line_number, char *file_name)
{
	int i = 0, j = 0;
	int num_args;
	char args[LINE_LENGTH][LINE_LENGTH];
	char args_str[LINE_LENGTH];
	
	/* get arguments */
	while (line[line_idx] != EOS)
		args_str[i++] = line[line_idx++];
	args_str[i] = EOS;
	remove_EOL(args_str);
	remove_white_space_end(args_str);
	num_args = process_arguments(args, args_str, line_number, file_name);
	if(num_args < 0)
		return FALSE;
	/* check if num of operands is valid */
	if((num_args < 2 ||  num_args > 2))
	{
		print_error(line_number, "Struct must include exactly two fields", file_name);
		return FALSE;
	}
	/* check the first filed of the struct */
	if(!(validate_number(args[0])))
	{
		print_error(line_number, "Invalid number", file_name);
		return FALSE;
	}
	/* check the second filed of the struct */
	if(!(validate_string(args[1], line_number, file_name)))
			return FALSE;
	/* insert into data image, and update DC */
	/* insert number*/
	*(data_image + *DC) = atoi(args[0]);
	(*DC)++;
	/* inserts string */
	for(j = 1; j < strlen(args[1]) -1; j++)
	{
		*(data_image + *DC) = (int)(args[1][j]);
		(*DC)++;
	}
	*(data_image + *DC) = 0;
	(*DC)++;
	return TRUE;	
}	

boolean extern_process(symbol * symbol_table, char * symbol_name, int line_number, char * line, int line_idx, char *file_name)
{
	int i = 0;
	
	/* get arguments */
	while(is_white_space(line[line_idx]))
		line_idx++;
	while (line[line_idx] != EOS)
		symbol_name[i++] = line[line_idx++];
	symbol_name[i] = EOS;
	remove_EOL(symbol_name);
	remove_white_space_end(symbol_name);
	/*check if there is a 1 argument*/
	if(strcmp(symbol_name, "") == 0)
	{
		print_error(line_number, ".extern instruction must include a lable", file_name);
		return FALSE;
	}
	if(!validate_label(symbol_name))
	{
		print_error(line_number, "Invalid label name", file_name);
		return FALSE;
	}
	/* check if the symbol is allready exists */
	if(find_symbol_by_name(symbol_table, symbol_name))
	{
		print_error(line_number, "Label allready exist", file_name);
		return FALSE;
	}
	return TRUE;
}
	

