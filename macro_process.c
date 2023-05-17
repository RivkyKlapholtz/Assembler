
#include "utils.h"
#include "macro_process.h"


/* ----- pre processor -----*/

boolean macro_process(FILE * source_file, char * file_name)
{
	macro * macro_table;
	char first_word[LINE_LENGTH] = "";
	char line[LINE_LENGTH + 2];
	char macro_name[MAX_NAME_LEN];
	char macro_content[MAX_CONTENT] = "";
	int table_index = 0;
	int is_in_table = INVALID;
	FILE * target_file;
	char temp[LINE_LENGTH];
	int line_number = 0;
	boolean error = TRUE;
	char c;
	
	strcpy(temp, file_name);
	macro_table = (macro *)malloc(sizeof(macro));
	if(!macro_table)
	{
		printf("memory allocation falied");
		exit(0);
	}
	target_file = fopen(strcat(temp, ".am"), "w+");
	
	/* reading the source file */
	while(!feof(source_file))
	{
		int i = 0;
		/* reached the end of the file*/
		if (fgetc(source_file) == EOF)
			break;
		fseek(source_file, -1, SEEK_CUR);
		/* get the line */
		fgets(line, LINE_LENGTH + 2, source_file);
		line_number++;
		/* check if line length  > 80*/
		if(strlen(line) > LINE_LENGTH)
		{
			print_error_macro(line_number, "line is too long", file_name);
			/* set the pointer to the next line */
			do {
				c = fgetc(source_file);
			} while (c != '\n' && c != EOF);
			error =  FALSE;
		}
		i = get_first_word(line, first_word);
		/* there is a macro */
		if(strcmp(first_word, "macro") == 0)
		{
			get_macro_name(line, macro_name, i);
			/* check if the macro name is valid */
			if(!valiate_macro_name(macro_name, macro_table, table_index))
			{
				print_error_macro(line_number, "Invalid macro name", file_name);
				fputs(line, target_file);
				error = FALSE;
				continue;
			}
			if(find_macro(macro_table, macro_name, table_index) >= 0)
			{
				print_error_macro(line_number, "macro is allready defined", file_name);
				fputs(line, target_file);
				error = FALSE;
				continue;
			}
			/* get the next line */
			fgets(line, LINE_LENGTH + 2 , source_file);
			line_number++;
			/* check if line length  > 80 */
			if(strlen(line) > LINE_LENGTH)
			{
				print_error_macro(line_number, "line is too long", file_name);
				/* set the pointer to the next line */
				do{
					c = fgetc(source_file);
				} while (c != '\n' && c != EOF);
				error =  FALSE;
			}
			/* processing the macro*/
			while(1)
			{
				get_first_word(line, first_word);
				if(strcmp(first_word, "endmacro") != 0)
				{
					strcat(macro_content, line);
					fgets(line, LINE_LENGTH +2 , source_file);
					line_number++;
					/* check if line length  > 80*/
					if(strlen(line) > LINE_LENGTH)
					{
						print_error_macro(line_number, "line is too long", file_name);
						/* set the pointer to the next line */
						do{
							c = fgetc(source_file);
						} while (c != '\n' && c != EOF);
						error =  FALSE;
					}
				}
				else
					break;
			}
			/* insert the macro into the table */
			insert_into_table(&macro_table, table_index, macro_name, macro_content);
			/* reset macro*/
			strcpy(macro_content ,"");
			table_index++;
		}
		/* if the first word is a name of macro get the macro content and copy to the targrt_file */
		else
		{
			is_in_table = find_macro(macro_table, first_word, table_index);
			if(is_in_table >= 0)
			{
				fputs(macro_table[is_in_table].content, target_file);
			}
			/* copy the line to new file */
			else
			{
				fputs(line ,target_file);
			}
		}
	}
	/* free memory allocation */
	free(macro_table);
	fclose(target_file);
	return error;
}



int get_first_word(char * line, char * first_word)
{

		int i = 0, j = 0;
		while (is_white_space(line[i]) && line[i] != EOS)
			i++;
		while (!is_white_space(line[i]) && line[i] != EOS && line[i] != '\n')
			first_word[j++] = line[i++];
		first_word[j] = EOS;
		return i;

}



void get_macro_name(char * line, char * macro_name, int i)
{
	int j = 0;
	while (is_white_space(line[i]) && line[i] != EOS)
			i++;
	while (!is_white_space(line[i]) && line[i] != EOS && line[i] != '\n')
			{
				macro_name[j] = line[i];
				j++;		
				i++;	
			}
	macro_name[j] = EOS;
}



void insert_into_table(macro ** macro_table, int table_index, char * macro_name, char * macro_content)
{
	macro * tmp;
	tmp = (macro *)realloc(*macro_table, (table_index+2)*sizeof(macro));
	if(!tmp)
	{
		printf("memory allocation falied");
		exit(0);
	}
	*macro_table = tmp;
	strcpy((*macro_table)[table_index].name, macro_name);
	strcpy((*macro_table)[table_index].content, macro_content);
}




int find_macro(macro * macro_table, char * first_word, int table_index)
{
	int i;
	for(i = 0; i < table_index; i++)
	{
		if((strcmp(macro_table[i].name, first_word)) == 0)
		{
			return i;
		}
	}
	return INVALID;
}

int valiate_macro_name(char * macro_name, macro *  macro_table, int table_index)
{
	if(is_languge_word(macro_name))
		return 0;
	return 1;
}

void print_error_macro(int line, char * error, char * file_name)
{
	printf("%s.as:%d: error: %s\n", file_name, line, error);
}

