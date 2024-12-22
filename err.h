#ifndef ERR_H
#define ERR_H

#include "tokenizer.h"

#define CHECK(statement,err,ret) \
if(!(statement))\
	{fprintf(stderr,"Error: %s.\n",err);\
	 return ret;}
	 
#define ERR(line,col,errtext) \
		fprintf(stderr,"\x1B[31m%d,%d: %s.\n\x1B[0m",\
			line,col,errtext);\
		err=1

#define tokeninfo(t)\
	fprintf(stderr,"\x1B[31mLn %d, Col %d\n\x1B[0m",\
			t->line,t->col);\
	print_token(*t);\
	putchar('\n');
#endif
