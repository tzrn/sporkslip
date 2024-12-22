#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <ffi.h>

#include "expr.h"
#include "hashtable.h"
#include "err.h"

#define car(e) (e->value.p[0])
#define cdr(e) (e->value.p[1])

extern int err;
token *t;
expr **currenv;
list *currgarb;
int envlen;
int envsize;
expr nil={0};
hash_table libs;
ffi_type *ctypes[RVOID+1];

void *newgarb(int size)
{
	void *ptr=malloc(size);
	add(currgarb,&ptr);
	return ptr;
}

expr *new_expr()
{
	return newgarb(sizeof(expr));
}

int isnil(expr *e)
{
	return e->type==ATOM &&
	       e->value.a.type==NIL;
}

expr *cons(expr *e1, expr *e2)
{
	expr *e=new_expr();
	e->type=PAIR;
	e->value.p[0]=e1;
	e->value.p[1]=e2;
}

expr *parse_atom()
{
	expr *e=new_expr();
	e->type=ATOM;
	atom *a=&e->value.a;
	switch(t->type)
	{
		case INT:
		a->type=INTA;
		a->value.i=t->value.i;
		break;
				
		case FLOAT:
		a->type=FLOATA;
		a->value.f=t->value.f;
		break;
		
		case STR:
		a->type=STRA;
		a->value.s=t->value.word->elements;
		break;
		
		case SYMBOLT:
		a->type=SYMBOL;
		a->value.sym.i=t->i2;
		a->value.sym.s=t->value.word;
		//a->value.i=t->i2;
		break;
		
		case COMMANDT:
		a->type=t->value.i>RVOID?COMMAND:CTYPE;
		a->value.i=t->value.i;
		break;
	}
	return e;
}

expr *parse_list();
expr *parse_expr()
{
	expr *new;
	
	if(t->type==OPEN_PAR)
	{
		++t;
		new=parse_list();
	}
	else
	{
		new=parse_atom();
	}
	++t;
	
	return new;
}

expr *parse_list()
{
	token *start=t;
	expr *e;
	
	if(t->type==CLOSE_PAR)
	{
		e=&nil;
	}
	else
	{
		expr *new=parse_expr();
		e=cons(new,parse_list());
		e->t=start;
	}
	
	return e;
}

expr *parse(list *tokens)
{
	t=tokens->elements;
	return parse_expr();
}

expr *eval(expr *e);
expr *resolve_if(expr *e)
{
	expr *args=cdr(e);
	//printf("resolving int cond - %d\n",eval(car(args))->value.a.value.i);
	return  eval(car(args))->value.a.value.i?
		    car(cdr(args)):
		    car(cdr(cdr(args)));
}

void setint(expr *e,int i)
{
	e->type=ATOM;
	e->value.a.type=INTA;
	e->value.a.value.i=i;
}

int checkatype(expr *e,int type)
{
	e=eval(e);
	if(e->type==PAIR)
	{
		return 0;
	}
	return e->value.a.type==type;
}

expr *run_cmd(expr *e)
{
	keyword cmd=car(e)->value.a.value.i;
	expr *args=cdr(e);
	
	switch(cmd)
	{
		case KDEFINE:
		int envoffset=car(args)->value.a.value.sym.i;
		expr *val=eval(car(cdr(args)));
		currenv[envoffset]=val;
		//printf("cmd defining: %s\n",car(args)->value.a.value.sym.s->elements);
		return &nil;
		
		case KLAMBDA:
		//copy current env and in apply just write over
		e->type=ATOM;
		atom *a=&e->value.a;
		a->type=LAMBDA;
		lambda *l=&a->value.lmbd;
		l->params=car(args);
		//printf("cmd lambda: lmbd ptr %p\n",e);
		l->body=cdr(args);
		return e;
		
		case KCAR : return car(eval(car(args)));
		case KCDR : return cdr(eval(car(args)));
		case KCONS:
		e->type=PAIR;
		e->value.p[0]=eval(car(args));
		e->value.p[1]=eval(car(cdr(args)));
		return e;
		
		case ISINT   :setint(e,checkatype(car(args),INTA));    return e;
		case ISFLOAT :setint(e,checkatype(car(args),FLOATA));  return e;
		case ISNIL   :setint(e,checkatype(car(args),NIL));     return e;
		case ISSTR   :setint(e,checkatype(car(args),STRA));    return e;
		
		case KIF:
		return eval(resolve_if(e));
		       
		case KLD: // (ld "a.so")
		void *handle=dlopen(eval(car(args))->value.a.value.s,RTLD_LAZY);
		if(handle==NULL)
		{
			printf("cmd ld: could not load lib\n");
		}
		else
		{
			//printf("cmd ld: loaded lib %p\n",handle);
		}
		e->type=ATOM;
		e->value.a.type=HANDLE;
		e->value.a.value.handle=handle;
		return e;
		
		case KFFUN:  //(f lib sum int int int)
		handle=eval(car(args))->value.a.value.handle;
		e->type=ATOM;
		e->value.a.type=FFUN;
		symbol *c=&car(cdr(args))->value.a.value.sym;
		char *str=c->s->elements;
		e->value.a.value.func.f=dlsym(handle,str);
		
	    list *types=newlist(ffi_type *);
	    expr *ct=cdr(cdr(args));

		while(!isnil(cdr(ct)))
		{
			//printf("ctype %d\n",car(ct)->value.a.value.i);
			add(types,&ctypes[car(ct)->value.a.value.i]);
			ct=cdr(ct);
		}
		int rtype=car(ct)->value.a.value.i;
		e->value.a.value.func.rtype=rtype;
		//printf("rtype %d\n",rtype);
		
		ffi_cif *cif=&e->value.a.value.func.cif;
		if(ffi_prep_cif(cif,FFI_DEFAULT_ABI,types->lsize,
						ctypes[rtype],types->elements) == FFI_OK)
		{
			//printf("prepared cif\n");
		}
		
		add(currgarb,&types);
		add(currgarb,&types->elements);
		currenv[c->i]=e;
		//printf("cmd ffun: loaded %s %p\n",str,e->value.a.value.func.f);
		return &nil;
	}
}

void setup_stack(expr *arg, lambda *l)
{
	for(expr *param=l->params;!isnil(param);param=cdr(param))
	{
		expr *argval=eval(car(arg));
		currenv[car(param)->value.a.value.sym.i]=argval;
		//printf("setup arg %p\n",&newenv[car(param)->value.a.value.sym.i]);
		arg=cdr(arg);
	}
	
	currenv[0]=eval(arg);
}

expr *apply(expr *e)
{
	//printf("APPLYING\n");
	lambda *l=&car(e)->value.a.value.lmbd;
	//printf("lmbd ptr %p\n",car(e));
	
	expr **env=malloc(envsize);
	memcpy(env,currenv,envsize);
	expr **oldenv=currenv;
	currenv=env;
	
	list *oldgarb=currgarb;
	list *garb=newlist(garb);
	currgarb=garb;
	setup_stack(cdr(e),l);
	
	start:
	expr *ex=l->body;
	while(!isnil(cdr(ex)))
	{
		//printf("evaling %d\n",car(ex)->type);
		eval(car(ex));
		ex=cdr(ex);
	}
	
	expr *endexpr=car(ex);
	if(endexpr->type==PAIR&&
	   car(endexpr)->type==ATOM&&
	   car(endexpr)->value.a.type==COMMAND&&
	   car(endexpr)->value.a.value.i==KIF)
	{
		endexpr=resolve_if(endexpr);
	}
	
	if(endexpr->type==PAIR)
	{
		expr *head=eval(car(endexpr));
		//could do expr *head = eval(car(endexpr));
		//and then cons new car and old cdr
		
		if(head->type==ATOM&&
		   head->value.a.type==LAMBDA&&
		   head->value.a.value.lmbd.body==l->body)
		{
			//printf("TCO\n");
			
			garb=newlist(expr *);
			list *todel=currgarb;
			currgarb=garb;
			
			//memcpy(env,currenv,envsize);
			//currenv=env;

			setup_stack(cdr(endexpr),l);
			dellist(todel,free);

			goto start;
		}
	}
	
	currgarb=oldgarb;
	e=eval(endexpr);
	currenv=oldenv;
	//printf("garb->lsize - %d\n",garb->lsize);
	dellist(garb,free);
	
	return e;
}

expr *apply_ffun(expr *e)
{
	//printf("FFFUN EVAAAL\n");
	ffun *f=&car(e)->value.a.value.func;
	
	list *values=newlist(void *);
	expr *arg=cdr(e);
	while(!isnil(arg))
	{
		atom *a=&eval(car(arg))->value.a;
		//printf("arg type %d\n",a->type);
		void *ptr;
		switch(a->type)
		{
			case INTA   :  ptr=&a->value.i;    break;
			case FLOATA :  ptr=&a->value.f;    break;
			case STRA   :  ptr=&a->value.s;    break;
			case CPTR   :  ptr=&a->value.cptr; break;
		}
		add(values,&ptr);
		arg=cdr(arg);
	}
	
	ffi_arg rc;
	void *rcptr=&rc;
	ffi_call(&f->cif,f->f,&rc,values->elements);
	dellist(values,NULL);
	
	expr *new=new_expr();
	new->type=ATOM;
	atom *a=&new->value.a;
	switch(f->rtype)
	{
		case RINT   :
		a->type=INTA;
		a->value.i=*((int *)rcptr);
		break;
		
		case RFLOAT :
		a->type=FLOATA;
		a->value.f=*((float *)rcptr);
		break;
		
		case RVOID  :
		return &nil;
		
		case GPTR: //untested
		add(currgarb,rcptr);
		case RPTR   :
		a->type=CPTR;
		a->value.cptr=*((void **)rcptr);
		break;
		
		case GSTR:
		void *val=*((void **)rcptr);
		if(val==NULL)
		{
			a->type=NIL;
			break;
		}
		//add(currgarb,&val);
		a->type=STRA;
		a->value.s=val;
		break;
	}

	return new;
}

expr *lookup(expr *s)
{
	int i=s->value.a.value.sym.i;
	//printf("lookup %p - %d type %d atype\n",currenv+i,i,);
	return eval(currenv[i]);
}

expr *eval(expr *e)
{
	/*if(e->t!=NULL)
	{
		printf("line %d col %d\n",e->t->line,e->t->col);
	}*/
	switch(e->type)
	{
		case ATOM:
		//printf("eval: atom %d\n",e->value.a.type);
		switch(e->value.a.type)
		{
			case SYMBOL:
			e=lookup(e);
			break;
		}
		
		expr *new=new_expr();
		*new=*e;
		return new;
		
		case PAIR:
		expr *ret = new_expr();
		ret->type = PAIR;
		car(ret)  = eval(car(e));
		cdr(ret)  = cdr(e);

		if(car(ret)->type==ATOM)
		{
			//printf("atom head %d\n",head->value.a.type);
			switch(car(ret)->value.a.type)
			{
				case COMMAND:
				return run_cmd(ret);
				
				case LAMBDA:
				return apply(ret);
				
				case FFUN:
				return apply_ffun(ret);
			}
		}

		cdr(ret)=eval(cdr(e));
		return ret;
	}
}

void init(int symbolsnum)
{
	//printf("symbolsnum %d\n",symbolsnum);
	envlen=symbolsnum;
	envsize=sizeof(expr *)*envlen;
	currgarb=newlist(expr *);
	currenv=newgarb(envsize);
	
	for(int i=0;i<symbolsnum;i++)
	{
		currenv[i]=new_expr();
		currenv[i]->type=NIL;
	}

	ctypes[RVOID]  = &ffi_type_void;
	ctypes[RINT]   = &ffi_type_sint;
	ctypes[RFLOAT] = &ffi_type_float;
	ctypes[RPTR]   = &ffi_type_pointer;
	ctypes[GPTR]   = &ffi_type_pointer;
	ctypes[GSTR]   = &ffi_type_pointer;
	init_table(&libs);
}

void destruct()
{
	dellist(currgarb,free);
	free_table(&libs,dlclose);
}
