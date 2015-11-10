/*
 * Copyright (c) 1998 Oliver Junge, for details see COPYING
 * 
 * SparseMatrix.c
 * 
 */

#define NZperCOL 10

#include <math.h>
#include <gaio/SparseMatrix.h>

SparseMatrix *SparseMatrixNew(int dim) {
  SparseMatrix *A;

  NEW1(A, SparseMatrix*, 1);
  A->dim = dim;
  A->nz = 0;
  A->size=dim*NZperCOL;
  NEW1(A->rows, int*, A->size);
  NEW1(A->cols, int*, A->size);
  NEW1(A->entries, double*, A->size);

  return A;
}

void SparseMatrixFree(SparseMatrix **A) {
  if (*A) {
    FREE((*A)->entries);
    FREE((*A)->cols);
    FREE((*A)->rows);
    FREE(*A);
    A = 0;
  }
}

void SparseMatrixAddEntry(SparseMatrix *A, int row, int col, double entry) {
  if (A->nz >= A->size) {
    A->size *= 2;
    A->rows = (int*)realloc(A->rows, A->size*sizeof(int));
    A->cols = (int*)realloc(A->cols, A->size*sizeof(int));
    A->entries = (double*)realloc(A->entries, A->size*sizeof(double));
  }
  A->dim = max(max(A->dim, row+1), col+1);
  A->rows[A->nz] = row;
  A->cols[A->nz] = col;
  A->entries[A->nz] = entry;
  A->nz++;
}

void SparseMatrixAddDiag(SparseMatrix *A, double a) {
  if (A) {
    int k;
    int *flag;
    NEW1(flag, int*, A->dim);

    /* looking for diagonal elemenst in existing entries */
    for (k=0; k < A->nz; k++) {
      if (A->rows[k]==A->cols[k]) {
	A->entries[k] += a;
	flag[A->rows[k]-1] = 1;
      }
    }

    /* inserting the remaining elements */
    for (k=1; k <= A->dim; k++) 
      if (!flag[k-1])
	SparseMatrixAddEntry(A, k, k, a);
  }
}

char *SparseMatrixEquilibria(SparseMatrix *A) {
  int i, n = A->dim;
  char *flags;
  NEW1(flags, char*, n+1);
  flags[n] = 0;
  
  for (i=0; i<n; i++) flags[i] = '0';
  for (i=0; i<A->nz; i++)
    if ((A->rows[i]==A->cols[i]) && A->entries[i]!=0.0)
      flags[A->rows[i]] = '1';

  return flags;
}

char *SparseMatrixFindSignChange(SparseMatrix *A, Matrix *v, int which) {
  int i, n = A->dim;
  char *flags;
  NEW1(flags, char*, n+1);
  flags[n] = 0;
  
  for (i=0; i<n; i++) flags[i] = '0';
  for (i=0; i<A->nz; i++) 
    if (A->entries[i]!=0.0) {
      double from = MatrixGetElement(v, A->cols[i], 0);
      double to = MatrixGetElement(v, A->rows[i], 0);
      if (sign(from)!=sign(to))
	switch (which) {
	case 1:  flags[A->cols[i]] = '1';
	  break;
	case -1:flags[A->rows[i]] = '1';
	  break;
	}
    }
  return flags;
}

void SparseMatrixMult(SparseMatrix *A, double *x, double *y) {
  int i, j, k;
  for (i=0; i < A->dim; i++) y[i] = 0;
  for (k=0; k < A->nz; k++) {
    i = A->rows[k];
    j = A->cols[k];
    y[i] += A->entries[k]*x[j];
  }
}

void SparseMatrixTMult(SparseMatrix *A, double *x, double *y) {
  int i, j, k;
  for (i=0; i < A->dim; i++) y[i] = 0;
  for (k=0; k < A->nz; k++) {
    j = A->rows[k];
    i = A->cols[k];
    y[i] += A->entries[k]*x[j];
  }
}

double SparseMatrixNorm(SparseMatrix *A, int which) {
  int i;
  double *sums;
  double norm = 0.0;

  NEW1(sums, double*, A->dim);
  for (i=0;i<A->dim;i++) sums[i] = 0.0;

  switch(which) {
  case -1: /* Norm(A,inf) */
    for (i=0;i<A->nz;i++) {
      sums[A->rows[i]] += fabs(A->entries[i]);
      norm = max(norm, sums[A->rows[i]]);
    }
  case 1:  /* Norm(A,1) */
    for (i=0;i<A->nz;i++) {
      sums[A->cols[i]] += fabs(A->entries[i]);
      norm = max(norm, sums[A->cols[i]]);
    }
  }

  FREE(sums);

  return norm;
}

void SparseMatrixInfo(FILE *out, SparseMatrix *A) {
  mexPrintf("sparse matrix at %p:\n", A);
  if (A) {
    int i;
    mexPrintf("  dim = %d\n", A->dim);
    mexPrintf("  nonzeros = %d\n", A->nz);
    for (i=0; i < A->nz; i++) 
      mexPrintf("  (%d, %d) = %g\n", A->rows[i]+1, A->cols[i]+1, A->entries[i]);
  }
}

void SparseMatrixPrint(FILE *out, SparseMatrix *A) {
  if (A) {
    int i;
    for (i=0; i < A->nz; i++) 
      fprintf(out, "%d %d %g\n", A->rows[i]+1, A->cols[i]+1, A->entries[i]);
  }
}

SparseMatrix *SparseMatrixLoad(FILE *in) {
  int row,col;
  double entry;
  SparseMatrix *A;
  
  NEW1(A, SparseMatrix*, 1);
  A->dim = 0;
  A->nz = 0;
  A->size = 100*NZperCOL;
  NEW1(A->rows, int*, A->size);
  NEW1(A->cols, int*, A->size);
  NEW1(A->entries, double*, A->size);

  while (fscanf(in,"%d %d %lf",&row,&col,&entry)==3) {
    A->dim=max(A->dim,max(row,col));
    if (A->nz >= A->size) {
      A->size *= 2;
      A->rows = (int*)realloc(A->rows, A->size*sizeof(int));
      A->cols = (int*)realloc(A->cols, A->size*sizeof(int));
      A->entries = (double*)realloc(A->entries, A->size*sizeof(double));
    }
    A->rows[A->nz] = row-1;
    A->cols[A->nz] = col-1;
    A->entries[A->nz] = entry;
    A->nz++;
  }
  return A;
}

char *SparseMatrixSinks(SparseMatrix *A, Matrix *scc) {

  int i, dim;
  char *flags;
  
  dim = A->dim;
  NEW1(flags, char*, dim+1);
  flags[dim] = 0;
  for (i=0; i<dim; i++) flags[i] = '1';

  for (i=0; i<A->nz; i++) 
    if (A->entries[i]!=0.0) {
      double from = MatrixGetElement(scc, A->cols[i], 0);
      double to = MatrixGetElement(scc, A->rows[i], 0);
      if (from!=to) flags[A->cols[i]] = '0'; 
    }

  return flags;
}
