/*
 * Copyright (c) 1999 Oliver Junge, for details see COPYING
 * 
 * IndSubshift.h
 * 
 */

#ifndef _IndSubshift_h
#define _IndSubshift_h

#include <gaio/SparseMatrix.h>
#include <gaio/Node.h>

typedef struct {
  int dim_sof;        /* dimension of the sofic shift */
  int dim_sub;        /* dimension of the induced subshift */
  int max_nodesize;   /* maximal size of a hypernode */
  SparseMatrix *A;    /* transition matrix of the sofic shift */
  unsigned char *letters;       /* letters of nodes in the sofic shift */
  int no_of_letters;  /* power of alphabet */
  Node *root;         /* of the graph */
  Node *current;

} IndSubshift;

IndSubshift *IndSubshiftNew(SparseMatrix *A, unsigned char *letters);
void IndSubshiftFree(IndSubshift **s);

/* inserts (or finds) a new hypernode in the induced subshift and
   returns it */
Node *IndSubshiftInsert(IndSubshift *s, char *node);

/* computes a new ("hyper")node of the induced subshift */
int IndSubshiftNewnode(IndSubshift *s, char letter, char *node,
		     char *newnode);

/* computes the induced graph starting from node */
void IndSubshiftPath(IndSubshift *s, SparseMatrix *A, int letter,
		     char *_node);

void IndSubshiftExtend(IndSubshift *s, SparseMatrix *A, char *node, Node *n);

SparseMatrix *IndSubshiftGraph(IndSubshift *s, int start);

void IndSubshiftInfo(FILE *out, IndSubshift *s);

#endif
