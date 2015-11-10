/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * SparseVector.c
 * 
 */

#include <gaio/SparseVector.h>
#include <gaio/defs.h>
#include <stdio.h>
#include <stdlib.h>

SparseVector *SparseVectorNew(int dim, int size) {
  SparseVector *v;

  NEW1(v, SparseVector*, 1);
  v->dim = dim;
  v->nz = 0;
  v->size = size;
  NEW1(v->rows, int*, size);
  NEW1(v->entries, double*, size);

  return v;
}

void SparseVectorFree(SparseVector **v) {
  if (*v) {
    FREE((*v)->entries);
    FREE((*v)->rows);
    FREE(*v);
    *v = 0;
  }
}

void SparseVectorAddEntry(SparseVector *v, int row, double entry) {
  if (v->nz >= v->size) {
    v->size *= 2;
    v->rows = (int*)realloc(v->rows, v->size*sizeof(int));
    v->entries = (double*)realloc(v->entries, v->size*sizeof(double));
  }
  v->rows[v->nz] = row;
  v->entries[v->nz] = entry;
  v->nz++;
}

void SparseVectorInfo(FILE *out, SparseVector *v) {
  fprintf(out, "sparse vector at %p:\n", v);
  if (v) {
    int i;
    fprintf(out, "  dim = %d\n", v->dim);
    fprintf(out, "  nonzeros = %d\n", v->nz);
    fprintf(out, "  size = %d\n", v->size);
    for (i=0; i < v->nz; i++) 
      fprintf(out, "  v(%d) = %g\n", v->rows[i], v->entries[i]);
  }
}

void SparseVectorPrint(FILE *out, SparseVector *v) {
  if (v) {
    int i;
    for (i=0; i < v->nz; i++) 
      fprintf(out, "%d %g\n", v->rows[i]+1, v->entries[i]);
  }
}

