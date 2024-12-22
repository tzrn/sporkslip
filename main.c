#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "hashtable.h"
#include "list.h"
#include "tokenizer.h"
#include "err.h"
#include "expr.h"

char *read_file(char *filename)
{
    FILE *f=fopen(filename,"r");
    if(!f)
    {
    	return NULL;
    }
    
    fseek(f,0,SEEK_END);
    int size=ftell(f);
    rewind(f);
    
    char *contents=malloc(size+1);
    fread(contents,size,sizeof(char),f);
    fclose(f);
    contents[size]=0;
    return contents;
}

int main(int argc, char **argv)
{
	CHECK(argc>1,"No file given",1);
    char *code=read_file(argv[1]);
	CHECK(code,"Could not open file",1);

	list *tokens=tokenize(code);
	
	//tokens->asize is set to number of symbols
	init(tokens->asize);
	eval(parse(tokens));
	free(code);
	free_tokens(tokens);
	destruct();
}
