/** @file part1.c */

/*
 * Machine Problem #0
 * CS 241 Fall2014
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "part2-functions.h"

/**
 * (Edit this function to print out the ten "Illinois" lines in part2-functions.c in order.)
 */
int main()
{
first_step(81);
int*a=malloc(sizeof(int));



*a=132;
second_step(a);
free(a);

int **values;
values = malloc(sizeof(int*));
*values = malloc( sizeof(int));
*values[0]=8942;
double_step(values);
free(values[0]);
free(values);


void *b=malloc(sizeof(int));
int*c=b+5;
*c=15;
strange_step(b);
free(b);

void *d=malloc(5*sizeof(char));
char *e=d+3;
*e=0;
empty_step(d);
free(d);

char *f=malloc(5*sizeof(char));
f[3]='u';
void *g=f;
two_step(f,f);//or gf
free(f);

char*h=malloc(5*sizeof(char));
three_step(h,h+2,h+4);
free(h);

char*i=malloc(5*sizeof(char));
i[1]=0;
i[2]=i[1]+8;
i[3]=i[2]+8;
step_step_step(i,i,i);
free(i);

char*j=malloc(sizeof(char));
*j=5;
it_may_be_odd(j,5);
free(j);

char* z=malloc(10*sizeof(char));
z[0]='0';
z[1]=',';
z[2]='C';
z[3]='S';
z[4]='2';
z[5]='4';
z[6]='1';
z[7]=',';
z[8]='5';
tok_step(z);
free(z);


void* final=malloc(sizeof(int));
char*l=(char*)final;
*l=1;
l[1]=2;
l[2]=0;
l[3]=0;
the_end(final,final);
free(final);




	return 0;
}
