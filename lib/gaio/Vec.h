/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Vec.h
 * 
 */

#ifndef _Vec_h
#define _Vec_h

#include <stdio.h>
#include <gaio/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

void VecPrint(FILE *out, double *x, int dim);
void VecSprint(char *s, double *x, int dim);
double *VecRead(FILE *in, int dim);
void VecCopy(double *from, double *to, int dim);
void VecFill(double *x, double v, int dim);
void VecAdd(double *x, double a, double *y, int dim, double *z);
int StringToVec(char *s, double *x, int dim);
int StringToBVec(char *s, byte *x, int dim);
double VecMax(double *x, int dim);
double VecNorm(double *x, int dim);

void MatIdentity(double *I, int dim);

#ifdef __cplusplus
}
#endif

#endif

