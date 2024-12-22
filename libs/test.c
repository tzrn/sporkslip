#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_str  (char *str) { printf("%s",str); }
void print_int  (int a)     { printf("%d",a);   }
void print_float(float a)   { printf("%f",a);   }

float itof(int a) { return (float)a; }
int ftoi(float a) { return (int)a; }

int divf (float a,float b){ return a/b; }
int sum  (int a,int b){ return a+b; }
int sub  (int a,int b){ return a-b; }
int divi (int a,int b){ return a/b; }
int mul  (int a,int b){ return a*b; }
int more (int a,int b){ return a>b; }
int less (int a,int b){ return a<b; }
int band (int a,int b){ return a&b; }
int mod  (int a,int b){ return a%b; }

int ieq  (int a,int b){ return a==b; }
int and  (int a,int b){ return a&&b; }
int or   (int a,int b){ return a||b; }

int not   (int a){ return !a; }
int minus (int a){ return -a; }

void sys  (char *c) { system(c); }
void eep  (int a)   { sleep(a);  }

char *readf(char *filename)
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

char *slice(char *s,int a, int b)
{
    int size = sizeof(char)*(b-a);
    char *slice = malloc(size+1);
    memcpy(slice,s+a,size);
    slice[size]=0;
    return slice;
}

char *concat(char *s1, char *s2)
{
    int l1=strlen(s1);
    int l2=strlen(s2);
    int size=l1+l2;
    char *s3=malloc(sizeof(char)*(size+1));
    memcpy(s3,s1,l1);
    memcpy(s3+l1,s2,l2);
    s3[size]=0;
    return s3;
}

int sref(char *s, int n)
{
    return s[n];
}
