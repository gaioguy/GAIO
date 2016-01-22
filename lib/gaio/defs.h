/*
 * Copyright (c) 2003-2009 djs² GmbH, for details see COPYING
 *
 * defs.h
 *
 */

#ifndef _defs_h
#define _defs_h

#include <stdlib.h>
#include <stdio.h>
#include <mex.h>

#define MAXDIM 256
#define sign(x) ((x) > 0 ? 1 : ((x)==0 ? 0 : -1))
#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))
#define mindex(i,j) ((i) + dim*(j))


#define NEW(ptr, nr) if(!(ptr = calloc((size_t)(nr), sizeof(*ptr))) && nr) { printf("calloc of %lu bytes failed on line %d of file %s\n", (unsigned long int)(nr * sizeof(*ptr)), __LINE__, __FILE__); printf("ptr%p\n", ptr); exit(EXIT_FAILURE); }

#define NEW1(ptr, type, nr) if(!(ptr = (type)calloc((size_t)(nr), sizeof(*ptr))) && nr) { printf("calloc of %lu bytes failed on line %d of file %s\n", (unsigned long int)(nr * sizeof(*ptr)), __LINE__, __FILE__); printf("ptr%p\n", ptr); exit(EXIT_FAILURE); }

#define RENEW(ptr, nr) if(!(ptr = realloc(ptr, (size_t)(nr) * sizeof(*ptr))) && nr) { printf("realloc of %lu bytes failed on line %d of file %s\n", (unsigned long int)(nr * sizeof(*ptr)), __LINE__, __FILE__); printf("ptr%p\n", ptr); exit(EXIT_FAILURE); }

#define RENEW1(ptr, type, nr) if(!(ptr = (type)realloc(ptr, (size_t)(nr) * sizeof(*ptr))) && nr) { printf("realloc of %lu bytes failed on line %d of file %s\n", (unsigned long int)(nr * sizeof(*ptr)), __LINE__, __FILE__); printf("ptr%p\n", ptr); exit(EXIT_FAILURE); }

#define FREE(ptr)  free(ptr)


/*#define contains(a, b, x)  (((x)>(a)) && ((x)<(b)))*/

/* 1, if the intervals [a,b] and [c,d] intersect */
#define interval_intersect(a, b, c, d)  !((((a)<(c)) && ((b)<(c))) || (((a)>(d)) && ((b)>(d))))

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;
typedef void (*VecFunc)(double *x, double *u, double *fx);
typedef void (*ConstFunc)(double *x);
typedef int (*IntFunc)(int x);
typedef double (*DoubleFunc)(double x);
typedef double (*CostFunc)(double *x, double *u);

void fputns(int c, int n, FILE *stream);

/* returns the size of file f (and rewinds) */
long fsize(FILE *f);

/* returns the position of the first newline within f,
   0 on error (and rewinds) */
long fnewline(FILE *f);

/* returns the number of tokens within the string s*/
int strcnttok(char *s, const char *delim);

/* computes the sum of the chars in s */
int sumchars(char *s, int length);

#ifdef __cplusplus
}
#endif

#endif
