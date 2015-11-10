/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Vec.c
 * 
 */

#include <gaio/Vec.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void VecPrint(FILE *out, double *x, int dim) {  
  int i;
  if (x) {
    for (i=0; i<dim; i++) fprintf(out, "%.15g ", x[i]);
    fprintf(out, "\n");
  } else
    fprintf(stderr, "warning: libgaio: x=0.\n");
}

void VecSprint(char *s, double *x, int dim) {  
  int i = 0, w = 0;
  do {
    w += sprintf(s+w, "%g ", x[i]);
  } while (++i < dim);
}

double *VecRead(FILE *in, int dim) {
  int i;
  double *x;
  NEW1(x, double*, dim);
  for (i=0; i<dim; i++) fscanf(in, "%lg", &(x[i]));
  fscanf(in, "\n");
  return x;
}

void VecCopy(double *from, double *to, int dim) {  
  int i;
  for (i=0; i<dim; i++) to[i] = from[i];
}

void VecFill(double *x, double v, int dim) {  
  int i;
  for (i=0; i<dim; i++) x[i] = v;
}

void VecAdd(double *x, double a, double *y, int dim, double *z) {  
  int i;
  for (i=0; i<dim; i++) z[i] = x[i] + a*y[i];
}

int StringToVec(char *s, double *x, int dim) {  
  int i = 0;
  char *buffer, *tok;
  NEW1(buffer, char*, strlen(s)+1);
  strcpy(buffer, s);
  tok = strtok(buffer, " ");
  while (tok && i<dim) {
    sscanf(tok, "%lg", &(x[i]));
    i++;
    tok = strtok(0, " ");
  }
  FREE(buffer);
  return i;
}	   

int StringToBVec(char *s, byte *x, int dim) {
  int i = 0, y;
  char *buffer, *tok;
  NEW1(buffer, char*, strlen(s)+1);
  strcpy(buffer, s);
  tok = strtok(buffer, " ");
  while (tok && i<dim) {
    sscanf(tok, "%d", &y);
    x[i] = (byte) y;
    i++;
    tok = strtok(0, " ");
  }
  FREE(buffer);
  return i;
}
 
double VecMax(double *x, int dim) {
  int i;
  double max = 0;
  for (i=0; i<dim; i++) 
    if (x[i] > max)
      max = x[i];
  return max;
}

double VecNorm(double *x, int dim) {
  int i;
  double norm = 0, axg;

  for (i=0; i<dim; i++) norm += x[i]*x[i];

  axg = sqrt(norm);

  return axg;
}

void MatIdentity(double *I, int dim) {
  int i;

  for (i=0; i<dim*dim; i++) I[i] = 0;
  for (i=0; i<dim; i++) I[i + i*dim] = 1;
}
 
