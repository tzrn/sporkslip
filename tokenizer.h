#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "hashtable.h"

typedef enum
{
	RINT,RFLOAT,RPTR,GPTR,GSTR,
	/***************/
	RVOID, //must be last ctype
	
	KNIL,
	KLAMBDA,
	KDEFINE,
	KFFUN,
	KLD,
	KCAR,
	KCDR,
	KCONS,
	KIF,
	RRARGS,
	
	ISINT,
	ISFLOAT,
	ISNIL,
	ISSTR
} keyword;

typedef enum
{
	OPEN_PAR,
	CLOSE_PAR,

	STR,INT,FLOAT,
	SYMBOLT,COMMANDT
} toktype;

typedef struct
{
	toktype type;
	union
	{
		list *word;
		int i;
		float f;
	} value;
	int i2;
	int line;
	int col;
} token;

list *tokenize(char *text);
void free_tokens(list *tokens);
#endif
