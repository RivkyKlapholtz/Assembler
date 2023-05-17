#include "utils.h"


int is_white_space(char c)
{
	return (c == SPACE) || (c == TAB);
}

void remove_EOL(char * line)
{
	if(line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';
}

void remove_white_space_end(char * str)
{
	int i = strlen(str) -1;
	while(is_white_space(str[i]))
	{
		str[i] = '\0';
		i--;
	}
}

int get_word(char * line, char * first_word, int i)
{
		int j = 0;
		while (is_white_space(line[i]) && line[i] != EOS)
			i++;
		while (!is_white_space(line[i]) && line[i] != EOS && line[i] != '\n')
			first_word[j++] = line[i++];
		first_word[j] = EOS;
		return i;
}

void print_error(int line, char * error, char * file_name)
{
	printf("%s.am:%d: error: %s\n", file_name, line, error);
}


int is_languge_word(char * str)
{
	char * lan_words[NUM_OF_LAN_WORDS] = {"data", "struct", "string", "entry", "extern"};
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
	/* name of instruction */
	for(i = 0; i < NUM_OF_LAN_WORDS; i++)
	{
		if(strcmp(lan_words[i], str) == 0)
			return 1;
	}
	for(i = 0; i < NUM_OF_OPERATIONS; i++)
	{
		if(strcmp(cmd_opcode_table[i].cmd, str) == 0)
			return 1;
	}
	/* name of register */
	if(is_register(str))
		return 1;
	return 0;
}

int process_arguments(char arg_list[LINE_LENGTH][LINE_LENGTH] , char * args, int line_number, char * file_name)
{
	int i = 0, j = 0, arg_idx = 0;
	int n = strlen(args);
	while (i < n)
	{
		j = 0;
		while (is_white_space(args[i]) && i < n)
			i++;
	
		if(args[i] == COMMA)
		{
			if(arg_idx == 0)  /* Comma after command*/ 
				print_error(line_number, "Invalid comma", file_name);
			else  /* Comma in the start of the new word - multiple commas*/ 
				print_error(line_number, "Multiple consecutive commas", file_name);
			return INVALID;
		}
		while (args[i] != COMMA && i < n)
		{
			arg_list[arg_idx][j] = args[i];

			j++;
			i++;
		}
		arg_list[arg_idx][j] = EOS;
		arg_idx++;
		i++;
	}

	/* check if last char is COMMA */
	if (args[n - 1] == COMMA)
	{
		print_error(line_number, "Invalid comma", file_name);
		return INVALID;	
	}
	/* remove white spaces from end of each arg */
	for (i = 0; i < arg_idx; i++)
	{
		j = strlen(arg_list[i]);
		while (is_white_space(arg_list[i][j - 1]) && j > 0)
		{
			j--;
		}
		arg_list[i][j] = EOS;
	}
	return arg_idx; /*number of arguments*/
}


boolean validate_number(char * num)
{
	int i = 0;
	if(num[0] == '-' || num[0] == '+')
		num = num + 1;
	while(num[i] != EOS)
	{
		if(!isdigit(num[i]))
			return FALSE;
		i++;
	}
	return TRUE;
}

boolean validate_label(char * symbol_name)
{
	int i;
	/* can't start with a number */
	if(isdigit(symbol_name[0]))
		return FALSE;
	for(i = 0; i < strlen(symbol_name); i++)
	{
		if(!isalnum(symbol_name[i]))
			return FALSE;
	}
	if(is_languge_word(symbol_name))
	{
		return FALSE;
	}
	return TRUE;
}


boolean is_register(char * operand)
{
	char * registers[NUM_OF_REGISTERS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
	int i;
	for(i = 0; i < NUM_OF_REGISTERS; i++)
	{
		if(strcmp(registers[i], operand) == 0)
			return TRUE;
	}
	return FALSE;
}

void free_extern(externals ** list)
{
    externals * h1, * h2;
	h1 = * list;
	while(h1 != NULL)
	{
		h2 = (externals *)h1->next;
		free(h1);
		h1 = h2;
	}
}


