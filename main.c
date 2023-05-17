#include "utils.h"
#include "table.h"
#include "macro_process.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_files.h"

/*
	get a file by its name 
	@param cfile_name the name of the file to open
	@return the open file
*/
FILE * get_file(char * file_name);

/*
	pharse a file- assembler
	@param filethe name of the file to procces
	@param file_name the name of the file
*/
void assembler_process(FILE * file, char * file_name);


int main(int argc, char ** argv)
{
	int i;
	FILE * assembly_file;
	FILE * pre_compilation_file;
	char tmp_name[MAX_FILE_NAME_LEN];
	char * file_name;
	boolean macro_status;
	
	/* no file was given */
	if(argc == 1)
	{
		printf("Erorr. Enter a file name\n");
		exit(0);
	}
	/* files loop */
	for(i = 1; i < argc; i++)
	{
		file_name = argv[i];
		strcpy(tmp_name, argv[i]);
		assembly_file = get_file(file_name);
		macro_status = macro_process(assembly_file, file_name);
		/* if some errors was found in macro procces, dont continue to first pass*/
		if(macro_status)
		{
			pre_compilation_file = fopen(strcat(tmp_name, ".am"), "r+");
			assembler_process(pre_compilation_file, file_name);
			fclose(pre_compilation_file);
		}
		fclose(assembly_file);
	}
	return 0;
}

/* get the file by its name */
FILE * get_file(char * file_name)
{
	char temp[LINE_LENGTH];
	FILE * file;
	strcpy(temp, file_name);
	file = fopen(strcat(temp, ".as"), "r+");
    if(!file)
    {
        printf("%s: the file does not exist\n", file_name);
        exit(0);
    }

	return file;
}


/* the assembler process */
void assembler_process(FILE * file, char * file_name)
{
	/* first pass*/
	int IC = IC_VAL, DC = 0;
	int line_number;
	char line[LINE_LENGTH]; 
	symbol * symbol_table = NULL;
	int data_image[MAX_DATA_IMAGE];
	memory_word code_image[MAX_CODE_IMAGE];
	externals * external_table = NULL;

	/* if there are errors in the input, dont continue to second pass */
	boolean is_succeed = TRUE;
	for(line_number = 1; fgets(line, LINE_LENGTH , file) != NULL; line_number++)
	{
		if(!(process_line_st_pass(&symbol_table , &IC, &DC, line, line_number, file_name, data_image, code_image)))
			is_succeed = FALSE;
	}
	/* update all data symbol addresses*/
	update_dc(&symbol_table, &IC);
	/* second pass - if first pass was done succecfully*/
	if(is_succeed)
	{
		/* reset the pointer to the first line in the file */
		rewind(file);
		IC = IC_VAL;
		for(line_number = 1; fgets(line, LINE_LENGTH , file) != NULL; line_number++)
		{	
			if(!process_line_nd_pass(&symbol_table, &IC, line, line_number,file_name, code_image, &external_table))
				is_succeed = FALSE;
		}
		if(is_succeed)
			write_output_files(&symbol_table, data_image, code_image ,&external_table, IC, DC, file_name);
	}
	
	/* free all  memory allocation */
	free_table(&symbol_table);
	free_extern(&external_table);
	
} 





