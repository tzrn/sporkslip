#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tokenizer.h"

void skip_comment(char **text)
{
	for(;;)
	{
		switch(*(++*text))
		{
			case '\n':
			case 0:
			return;
		}
	}
}

int is_10digit(char c)
{
	return c>='0'&&c<='9';
}

int is_16digit(char c)
{
	return c>='0'&&c<='9'||
	       c>='a'&&c<='f'||
	       c>='A'&&c<='F';
}

int is_float(char *str)
{
	int dot=0;
	while(*str)
	{
		if(*str=='.')
		{
			if(dot++!=0)
			{
				return 0;
			}
		}
		else if(!is_10digit(*str))
		{
			return 0;
		}
		str++;
	}
	return 1;
}

int check_string(char *str,int (*func)(char))
{
	while(*str)
	{
		if(!func(*str))
		{
			return 0;
		}
		++str;
	}
	return 1;
}

int isichar(char c)
{
	switch(c) //tokens that separate identifiers
	{
		case 0:
		case '(':case ')':
		case ' ':case '\t':case '\n':
		case ';':case '"':
		return 0;
	}
	return 1;
}

void free_tokens(list *tokens)
{
	for(int i=0;i<tokens->lsize;i++)
	{
		token *t=get(tokens,i);
		switch(t->type)
		{
			case STR:case SYMBOLT:
				dellist(t->value.word,NULL);
			break;
		}
	}
	dellist(tokens,NULL);
}

#define SET(kw,type) table_seti(&kwords,kw,type)
list *tokenize(char *text)
{
	hash_table kwords;
	init_table(&kwords);
	SET("nil",KNIL);
	SET("l",KLAMBDA);
	SET("d",KDEFINE);
	SET("f",KFFUN);
	SET("ld",KLD);
	SET("if",KIF);
	SET("car",KCAR);
	SET("cdr",KCDR);
	SET("cons",KCONS);

	SET("int",RINT);
	SET("float",RFLOAT);
	SET("void",RVOID);
	SET("ptr",RPTR);
	SET("gptr",GPTR);
	SET("gstr",GSTR);
	
	SET("int?",ISINT);
	SET("float?",ISFLOAT);
	SET("nil?",ISNIL);
	SET("str?",ISSTR);

	list *tokens=newlist(token);
	token new;
	int line=1;
	char *colstart=text-1;
	hash_table symbols;
	
	init_table(&symbols);
	table_seti(&symbols,"...",0);
	int lastsymbol=symbols.lsize;

	new.type=OPEN_PAR;
	add(tokens,&new);
	for(;;)
	{
		new.col=text-colstart;
		switch(*text)
		{
			case '(':
			new.type=OPEN_PAR;
			break;
				
			case ')':
			new.type=CLOSE_PAR;
			break;

			case ';': //no comment
			skip_comment(&text);
			continue;

			case '\n': //fallthrough
			++line;
			colstart=text;
			case ' ':
			case '\t':
			++text;
			continue;

			case '"':
			new.type=STR;
			list *str=newlist(char);
			while(*(text+1)&&*++text!='"') //idk this works i guess
			{
				if(*text=='\\')
				{
					++text;
				}
				addi(str,*text);
			}
			addi(str,0);
			new.value.word=str;
			break;

			default:
			list *word=newlist(char);
			do
			{
				addi(word,*text);
			}
			while(isichar(*++text));
			--text;
			addi(word,0);
			
			//this could be redone i guess
			char *strval=word->elements;
			if(check_string(strval,is_10digit))
			{
				new.type=INT;
				new.value.i=strtol(strval,NULL,10);
				dellist(word,NULL);
			}
			else
			if(strval[0]=='#'&&check_string(strval+1,is_16digit))
			{
				new.type=INT;
				new.value.i=strtol(strval+1,NULL,16);
				dellist(word,NULL);
			}
			else
			if(is_float(strval))
			{
				new.type=FLOAT;
				new.value.f=atof(strval);
				dellist(word,NULL);
			}
			else
			{
				int *kw;
				kw=table_get(&kwords,word->elements);
				if(kw!=NULL)
				{
					new.type=COMMANDT;
					new.value.i=*kw;
					dellist(word,NULL);
				}
				else
				{
					kw=table_get(&symbols,word->elements);
					int c=lastsymbol;
					if(kw==NULL)
					{
						table_seti(&symbols,word->elements,lastsymbol);
						c=lastsymbol++;
					}
					else
					{
						c=*kw;
					}
					new.type=SYMBOLT;
					new.i2=c;
					new.value.word=word;
				}
			}
			break;

			case 0:
			new.type=CLOSE_PAR;
			add(tokens,&new);
			tokens->asize=symbols.lsize;
			free_table(&kwords,free);
			free_table(&symbols,free);
			return tokens;
		}

		new.line=line;
		add(tokens,&new);
		++text;
		//print_token(new);
	}
	
	return NULL;
}
