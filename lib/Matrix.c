/*
 * Copyright (c) 1998,99 Oliver Junge, Stefan Sertl,
 *               2003-2009 djs² GmbH, for details see COPYING
 * 
 * Matrix.c
 * 
 */

#include <gaio/Matrix.h>
#include <gaio/defs.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

Matrix *MatrixNew(int rows, int cols) {
  Matrix *A;

  NEW1(A,Matrix*,1);
  A->rows = rows;
  A->cols = cols;
  NEW1(A->d, double*,rows*cols);
  A->memFlag = 1;

  return A;
}

Matrix *MatrixRef(int rows, int cols, double *d) {
  Matrix *A;

  NEW1(A,Matrix*,1);
  A->rows = rows;
  A->cols = cols;
  A->d = d;
  A->memFlag = 0;

  return A;
}
  
void MatrixFree(Matrix **A) {
  if (*A) {
    if ((*A)->memFlag && (*A)->d) {
      FREE((*A)->d);
    }
    FREE(*A);
    *A=0;
  }
}

void MatrixSetElement(Matrix *A, int i, int j, double value) {
  assert(i>=0);
  assert(i<A->rows);
  assert(j>=0);
  assert(j<A->cols);
  A->d[j*(A->rows)+i] = value;
}

double MatrixGetElement(Matrix *A, int i, int j) {
  assert(i>=0 && i<A->rows);
  assert(j>=0 && j<A->cols);
  return A->d[j*(A->rows)+i];
}

void MatrixAddToElement(Matrix *A, int i, int j, double value) {
  assert(i>=0 && i<A->rows);
  assert(j>=0 && j<A->cols);
  A->d[j*(A->rows)+i] += value;
}

int MatrixSameRange(Matrix *A, Matrix *B) {
  return (A->rows==B->rows && A->cols==B->cols);
}

void MatrixCopy(Matrix *A, Matrix *B) {
  int i;

  assert(MatrixSameRange(A, B));
  for (i=0; i<(A->rows*A->cols); i++)
      B->d[i] = A->d[i];
}

void MatrixIdentity(Matrix *I) {
  int i, dim;

  assert(I->rows==I->cols);
  dim = I->rows;

  for (i=0; i<dim*dim; i++) I->d[i] = 0.0;
  for (i=0; i<dim; i++) I->d[i*dim] = 1.0;
}  

Matrix *MatrixTranspose(Matrix *A) {
  int i, j;
  Matrix *AT;

  AT = MatrixNew(A->cols, A->rows);
  for (i=0; i<A->rows; i++)
    for (j=0; j<A->cols; j++) 
      AT->d[j + i*(A->cols)]  = A->d[i + j*(A->rows)];

  return AT;
}

void MatrixCopyTo(Matrix *A, double *a) {
  int i;

  for (i=0; i<(A->rows*A->cols); i++)
      a[i] = A->d[i];
}  

void MatrixMult(Matrix *A, Matrix *B, Matrix *C) {
  int i, j, k;

  assert(A->cols==B->rows);
  assert(A->rows==C->rows);
  assert(B->cols==C->cols);

  for (i=0; i<A->rows; i++)
    for (j=0; j<B->cols; j++) {
      C->d[j*(C->rows) + i] = 0;
      for (k=0; k<A->cols; k++) 
	C->d[j*(C->rows) + i] += A->d[k*(A->rows)+i]
	                           *B->d[j*(B->rows)+k];
    }
}

void MatrixMultTranspose(Matrix *A, Matrix *BT, Matrix *C) {
  int i, j, k;

  assert(A->cols==BT->rows);
  assert(A->rows==C->rows);
  assert(BT->cols==C->cols);

  for (i=0; i<A->rows; i++)
    for (j=0; j<BT->cols; j++) {
      C->d[i + j*(C->rows)] = 0;
      for (k=0; k<A->cols; k++) 
	C->d[i + j*(C->rows)] += A->d[i + k*(A->rows)]*BT->d[j + k*(BT->rows)];
    }
}

void MatrixTransposeMult(Matrix *AT, Matrix *B, Matrix *C) {
  int i, j, k;

  assert(AT->rows==B->rows);
  assert(AT->cols==C->rows);
  assert(B->cols==C->cols);

  for (i=0; i<AT->rows; i++)
    for (j=0; j<B->cols; j++) {
      C->d[i + j*(C->rows)] = 0;
      for (k=0; k<AT->cols; k++) 
	C->d[i + j*(C->rows)] += AT->d[k + i*(AT->rows)]*B->d[k + j*(B->rows)];
    }
}

void MatrixPrint(FILE *out, Matrix *A) {
  int i, j;
  if (A) {
    for (i=0; i<A->rows; i++) {
      for (j=0; j<A->cols; j++)
	fprintf(out, "%g ", A->d[j*(A->rows)+i]);
      fprintf(out, "\n");
    }
  }
}

Matrix *MatrixPow(Matrix *A, unsigned int p) {
  unsigned int i;
  Matrix *B, *C;

  assert(A->rows==A->cols);

  B = MatrixNew(A->rows, A->cols);
  C = MatrixNew(A->rows, A->cols);
  MatrixCopy(A, B);
  for (i=1; i<p; i++) {
    MatrixMult(A, B, C);
    MatrixCopy(C, B);
  }
  MatrixFree(&C);

  return B;
}

void MatrixApply(Matrix *A, DoubleFunc f) {
  int i;
  
  for (i=0; i<(A->rows)*(A->cols); i++) A->d[i] = f(A->d[i]);
}
 
Matrix *MatrixAbs(Matrix *A) {
  int i;
  Matrix *Abs;

  Abs = MatrixNew(A->rows, A->cols);

  for (i=0; i<(A->rows*A->cols); i++)
      Abs->d[i] = fabs(A->d[i]);

  return Abs;
}

double MatrixMin(Matrix *A) {
  int i;
  double min = 0.0;

  if (A->rows*A->cols > 0) {
    min = A->d[0];
    for (i=1; i<(A->rows*A->cols); i++)
      if (A->d[i] < min)
	min = A->d[i];
  }
  return min;
}

double MatrixMax(Matrix *A) {
  int i;
  double max = 0.0;

  if (A->rows*A->cols > 0) {
    max = A->d[0];
    for (i=1; i<(A->rows*A->cols); i++)
      if (A->d[i] > max)
	max = A->d[i];
  }
  return max;
}

double MatrixSum(Matrix *A) {
  int i;
  double sum = 0;

  for (i=0; i<(A->rows*A->cols); i++)
    sum += A->d[i];

  return sum;
}
 
Matrix *MatrixColumn(Matrix *A, int j) {
  int i;
  Matrix *col;

  col = MatrixNew(A->rows, 1);
  for (i=0; i<A->rows; i++)
    col->d[i] = A->d[j*(A->rows)+i];

  return col;
}

char *MatrixFindGeq(Matrix *A, double v) {
  int i, n = A->rows*A->cols;
  char *flags;
  
  NEW1(flags, char*, n+1);
  flags[n]=0;

  for (i=0; i<n; i++) flags[i] = '0';
  for (i=0; i<n; i++) if (A->d[i] >= v) flags[i] = '1';

  return flags;
}
