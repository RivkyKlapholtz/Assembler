#include "table.h"
#include "utils.h"


symbol * find_symbol_by_name(symbol * head, char * name)
{
	symbol * h1;
	h1 = head;
	while(h1 != NULL)
	{
		if(strcmp(name, (h1->name)) == 0)
		{
			return h1;
		}
		h1 = (symbol *)h1->next;
	}
	return NULL;		
}


void add_symbol(symbol ** head,char * name, int address ,type symbol_type)
{
	symbol * h = * head;
	symbol * new_symbol;
	new_symbol = (symbol *)malloc(sizeof(symbol));
	if(!new_symbol)
	{
		printf("memory allocation faild");
		exit(0);
	}
	strcpy(new_symbol->name ,name);
	new_symbol->address = address;
	new_symbol->symbol_type = symbol_type;	
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
			h = (symbol *)h->next;
		h->next =(ptr)new_symbol;
	}
}



void free_table(symbol ** table)
{
    symbol * h1, * h2;
	h1 = * table;
	while(h1 != NULL)
	{
		h2 = (symbol *)h1->next;
		free(h1);
		h1 = h2;
	}
}


