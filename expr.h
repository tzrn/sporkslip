#ifndef EXPR_H
#define EXPR_H
#include <ffi.h>
#include "list.h"
#include "hashtable.h"
#include "tokenizer.h"

typedef struct
{
	struct expr_  *params;
	struct expr_  *body;
	//list *garb;
} lambda;

typedef struct
{
	list *s;
	int i;
} symbol;

typedef struct
{
	void (*f)();
	ffi_cif cif;
	int rtype;
} ffun;

typedef struct
{
	enum
	{
		NIL,
		
		INTA,
		STRA,
		FLOATA,
		
		SYMBOL,
		COMMAND,
		LAMBDA,
		HANDLE,
		FFUN,
		CTYPE,
		CPTR
	} type;
	union
	{
		int i;
		float f;
		char *s;
		symbol sym;
		ffun func;
		lambda lmbd;
		void *handle;
		void *cptr;
	} value;
} atom;

typedef struct expr_
{
	enum
	{
		ATOM,
		PAIR
	} type;
	union
	{
		struct expr_ *p[2];
		atom   a;
	} value;
	token *t;
} expr;

expr *parse(list *tokens);
expr *eval(expr *e);
void init(int symbolsnum);
void destruct();
#endif
