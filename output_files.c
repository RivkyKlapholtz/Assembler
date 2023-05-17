#include "utils.h"
#include "table.h"
#include "code.h"
#include "second_pass.h"
#include "first_pass.h"
#include "output_files.h"




void write_output_files(symbol ** symbol_table,int * data_image,memory_word * code_image ,externals ** external_table,int IC,int DC,char * file_name)
{
	write_object(IC, DC, file_name, data_image, code_image);
	write_ent(symbol_table, file_name);
	write_ext(external_table, file_name);

}

void write_ext(externals ** external_table, char * file_name)
{
	char temp[LINE_LENGTH];
	char num_in_base[3];
	externals * h1;
	h1 = *external_table;
	strcpy(temp, file_name);
	if((h1))
	{
		FILE * ext_file;
		ext_file = fopen(strcat(temp, ".ext"), "w+");
		while(h1!=NULL)
		{
			dec_to_32_base(h1->address, num_in_base);
			fprintf(ext_file, "%s %s\n", h1->name, num_in_base);
			h1 = (externals *)h1->next;
		}
		fclose(ext_file);
	}
}



void write_ent(symbol ** symbol_table,char * file_name)
{
	char temp[LINE_LENGTH];
	char num_in_base[3];
	FILE * ent_file;
	boolean flag = FALSE;
	symbol * h1;
	h1 = *symbol_table;
	
	strcpy(temp, file_name);
	ent_file = fopen(strcat(temp, ".ent"), "w+");
	while(h1!=NULL)
	{
		if(h1->symbol_type == ENTRY)
		{
			flag = TRUE;
			dec_to_32_base(h1->address, num_in_base);
			fprintf(ent_file, "%s %s\n", h1->name, num_in_base);
		}
		h1 = (symbol *)h1->next;
	}
	if(!flag)
		remove(temp);
	fclose(ent_file);
}



void write_object(int IC,int DC, char * file_name, int * data_image, memory_word * code_image)
{
	char temp[LINE_LENGTH];
	char converted1[CONV_LEN];
	char converted2[CONV_LEN];
	int i;
	FILE * ob_file;

	strcpy(temp, file_name);
	ob_file = fopen(strcat(temp, ".ob"), "w+");
	dec_to_32_base(IC-IC_VAL, converted1);
	dec_to_32_base(DC, converted2);
	fprintf(ob_file, "%s %s\n", converted1, converted2);
	for(i = IC_VAL; i < IC; i++)
	{
		unsigned int * p = (unsigned int*)(&code_image[i-IC_VAL]);
		dec_to_32_base(i, converted1);
		dec_to_32_base(*p, converted2);
		fprintf(ob_file, "%s %s\n", converted1, converted2);
	}	
	for(i = IC; i < IC+DC; i++)
	{
		unsigned int * p = (unsigned int*)(&data_image[i-IC]);
		int mask = 1023;
		(*p) = (*p) & mask;
		dec_to_32_base(i, converted1);
		dec_to_32_base(*p, converted2);
		fprintf(ob_file, "%s %s\n", converted1, converted2);
	}
	fclose(ob_file);
}

void dec_to_32_base(int num, char * result)
{
	char base_32[] = {'!','@','#','$','%','^','&','*','<' ,'>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
	result[0] = base_32[((num / BASE) % BASE)];
	result[1] = base_32[(num % BASE)];
	result[2] = EOS;
}
