/*
 * Copyright (c) 1998 Oliver Junge, for details see COPYING
 * 
 * SparseMatrix.h
 * 
 */

#ifndef _SparseMatrix_h
#define _SparseMatrix_h

#include <gaio/defs.h>
#include <gaio/Matrix.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  
  int dim;              /* dimension of the matrix */
  int nz;               /* number of nonzeros of the matrix */
  int *rows;
  int *cols;
  double *entries;
  int size;             /* length of the vectors rows, cols and entries */

} SparseMatrix;
  
SparseMatrix *SparseMatrixNew(int dim);
void SparseMatrixFree(SparseMatrix **A);
void SparseMatrixAddEntry(SparseMatrix *A, int row, int col, double entry);

/* adds a to the diagonal of A */
void SparseMatrixAddDiag(SparseMatrix *A, double a);

/* returns a char vector c such that c[i]=(A[i,i]!=0) */
char *SparseMatrixEquilibria(SparseMatrix *A);

/* returns a char vector c s.t. 
   which = 1:  c[i]=1, if A[i,j]>0 and v[i]*v[j]<0,
   which = -1: c[j]=1, if A[i,j]>0 and v[i]*v[j]<0 */
char *SparseMatrixFindSignChange(SparseMatrix *A, Matrix *v, int which);

/* y=Ax */
void SparseMatrixMult(SparseMatrix *A, double *x, double *y);

/* y=transpose(A)x */
void SparseMatrixTMult(SparseMatrix *A, double *x, double *y);

/* Norm(A,which), which==-1: inf-Norm, which==1: 1-Norm */
double SparseMatrixNorm(SparseMatrix *A, int which);

void SparseMatrixInfo(FILE *out, SparseMatrix *A);
void SparseMatrixPrint(FILE *out, SparseMatrix *A);
SparseMatrix *SparseMatrixLoad(FILE *in);

/* the vector scc assinging the number of the strongly connected
   component to each column of A this function returns a vector of flags, 
   indictating which column corresponds to a sink cell */
char *SparseMatrixSinks(SparseMatrix *A, Matrix *scc);

#ifdef __cplusplus
}
#endif

#endif
