/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * SparseVector.h
 * 
 */

#ifndef _SparseVector_h
#define _SparseVector_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  
  int dim;              /* dimension of the vector */
  int nz;               /* number of nonzeros of the vector */
  int size;             /* size of rows and entries */
  int *rows;
  double *entries;

} SparseVector;
  
SparseVector *SparseVectorNew(int dim, int size);
void SparseVectorFree(SparseVector **A);
void SparseVectorAddEntry(SparseVector *A, int row, double entry);

void SparseVectorInfo(FILE *out, SparseVector *A);
void SparseVectorPrint(FILE *out, SparseVector *A);

#ifdef __cplusplus
}
#endif

#endif
