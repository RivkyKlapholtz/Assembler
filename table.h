
#define LABEL_LEN 30

/* -------------------- */

/* type of symbol */
typedef enum type{
	CODE,
	DATA,
	EXTERN,
	ENTRY
}type;

/*symbol table*/

typedef struct symbol * ptr;

typedef struct{
	char name[LABEL_LEN];
	int address;
	type symbol_type;
	ptr next;
}symbol;


/* ------ functions --------*/
/*
 find a symbol in table by name
 @param head the head of the table
 @param name the name to find
 @return the found node
*/
symbol * find_symbol_by_name(symbol * , char *);


/*
 add a symbol to the table
 @param head the head of the table
 @param name the name to insert
 @param address the address to insert
 @param symbol_type the type og the symbol
*/
void add_symbol(symbol ** head,char * name, int address ,type symbol_type);


/*
 free all table memory allocation
 @param table the head of the table to free
*/
void free_table(symbol ** table);


