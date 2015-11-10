/*
 * Copyright (c) 1998,99 Oliver Junge, Stefan Sertl, 
 *               2003-2009 djs² GmbH, for details see COPYING
 * 
 * Matrix.h
 * 
 */

#ifndef _Matrix_h
#define _Matrix_h

#include <stdio.h>
#include <gaio/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

  int rows, cols;       /* number of rows and columns of matrix */
  int memFlag;          /* boolean: matrix uses own memory (true/false) */
  double *d;            /* memory for matrix-elements */

} Matrix;

Matrix *MatrixNew(int rows, int cols);
Matrix *MatrixRef(int rows, int cols, double *d); /* use given memory */

void MatrixFree(Matrix **A);
void MatrixSetElement(Matrix *A, int i, int j, double value);
double MatrixGetElement(Matrix *A, int i, int j);
void MatrixAddToElement(Matrix *A, int i, int j, double value);
int  MatrixSameRange(Matrix *A, Matrix *B);
void MatrixCopy(Matrix *A, Matrix *B);
void MatrixIdentity(Matrix *I);
Matrix *MatrixTranspose(Matrix *A);

/* copys the matrix into a double vector */
void MatrixCopyTo(Matrix *A, double *a);
/* C = A*B */
void MatrixMult(Matrix *A, Matrix *B, Matrix *C);
void MatrixMultTranspose(Matrix *A, Matrix *BT, Matrix *C);
void MatrixTransposeMult(Matrix *AT, Matrix *B, Matrix *C);
/* returns the p´th power of A */
Matrix *MatrixPow(Matrix *A, unsigned int p);

void MatrixApply(Matrix *A, DoubleFunc f);
Matrix *MatrixAbs(Matrix *A);
double MatrixMax(Matrix *A);
double MatrixMin(Matrix *A);
double MatrixSum(Matrix *A);

Matrix *MatrixColumn(Matrix *A, int j);
char *MatrixFindGeq(Matrix *A, double v);

/* prints A to the stream file */
void MatrixPrint(FILE *out, Matrix *A);

#ifdef __cplusplus
}
#endif

#endif
