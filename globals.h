#ifndef _GLOBALS_H
#define _GLOBALS_H



/* -------------- globals definitions--------------*/
#define IC_VAL 100 
#define MAX_DATA_IMAGE 1000
#define MAX_CODE_IMAGE 1000
#define NUM_OF_LAN_WORDS 5
#define NUM_OF_DATA_WORDS 3
#define NUM_OF_OPERATIONS 16
#define NUM_OF_REGISTERS 8
#define MAX_OPERANDS 80
#define LINE_LENGTH 81
#define LABEL_LEN 30
#define MAX_FILE_NAME_LEN 200
#define CONV_LEN 3
#define BASE 32

/* -------------  structs ------------------------*/


typedef enum boolean{
	FALSE = 0,
	TRUE = 1
}boolean;

/* memory words*/
typedef union words{
	struct{
		unsigned int ARE: 2;
		unsigned int dest_addr: 2;
		unsigned int source_addr: 2;
		unsigned int opcode: 4;
		}machine_word;
	struct{
		unsigned int ARE: 2;
		unsigned int num: 8;
		}number;
	struct{
		unsigned int ARE: 2;
		unsigned int addr: 8;
		}address;
	struct{
		unsigned int ARE: 2;
		unsigned int dest_addr: 4;
		unsigned int source_addr: 4;
		}reg;	
}memory_word;



/* opcodes  */
typedef enum opcode{
	MOV,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	GET,
	PRN,
	JSR,
	RTS,
	HLT,
	ERROR_OP
}opcode;

typedef enum addressing_type{
	IMMEDIATE_ADDR,
	DIRECT_ADDR,
	ENTRY_ADRR,
	REGISTER_ADDR,
	ERROR
}addressing_type;

typedef struct cmd_opcode{
	char * cmd;
	opcode op;
}cmd_opcode;

typedef struct externals * ex_ptr;

typedef struct externals{
	char name[LINE_LENGTH];
	int address;
	ex_ptr next;
}externals;


/* -------------globals-------------*/




#endif

