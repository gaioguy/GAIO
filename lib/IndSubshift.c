/*
 * Copyright (c) 1999 Oliver Junge, for details see COPYING
 * 
 * IndSubshift.c
 *
 */

#include <string.h>
#include <math.h>
#include <gaio/IndSubshift.h>

static int _no_of_newnodes = 0;

IndSubshift *IndSubshiftNew(SparseMatrix *A, unsigned char *letters) {
  int i;
  IndSubshift *s;

  NEW(s, 1);
  s->dim_sof = A->dim;
  s->dim_sub = 0;
  s->max_nodesize = 0;
  s->A = A;
  NEW(s->letters, A->dim);
  s->no_of_letters = letters[0]+1;
  for (i=1; i<A->dim; i++) {
/*    mexPrintf("letters[%d] = %d\n", i, letters[i]); */
    if (letters[i]+1 > s->no_of_letters) 
      s->no_of_letters = letters[i]+1;
  }
  memcpy(s->letters, letters, (A->dim));
/*  mexPrintf("IndSubshiftNew: no_of_letters = %d\n", s->no_of_letters); */
  s->root = NodeNew(0);
  s->root->sub = -1;

  return s;
}

void IndSubshiftFree(IndSubshift **s) {
  if (*s) {
    FREE((*s)->letters);
    NodeFree(&((*s)->root));
    FREE(*s);
    *s = 0;
  }
}

Node *IndSubshiftInsert(IndSubshift *s, char *node) {
  unsigned int i, flag;
  Node *current;
  
  current = s->root;
  for (i=0; i<s->dim_sof; i++) 
    if (node[i]) 
      current = NodeInsert(current, i+1);

  if (current->sub==0) 
    current->sub = ++s->dim_sub;

  return current;
}

int IndSubshiftNewnode(IndSubshift *s, char letter, char *node,
		     char *newnode) {
  int k, hits = 0;

  memset(newnode, 0, s->dim_sof);

  for (k=0; k<s->A->nz; k++) {
    int i = s->A->rows[k];
    int j = s->A->cols[k];
/*     printf("i=%d, j=%d, ent=%lf, node[j]=%d, let[j]=%d\n", i, j,  
 	   s->A->entries[k], node[j], s->letters[j]); */
    if (s->A->entries[k]!=0.0 && node[j] && s->letters[j]==letter) {
      hits = 1;
      newnode[i] = 1;
    }
  }
  return hits;
}

void NodePrint(char *n, int dim) {
  int i;
  
  for (i=0; i<dim; i++) mexPrintf("%d", n[i]);
  mexPrintf("\n");
}

/* void IndSubshiftPath(IndSubshift *s, SparseMatrix *A, int letter,  */
/* 		     char *_node) { */
/*   char *node, *newnode, *tmpnode; */
/*   int i, new, hits; */
/*   Node *n, *newn; */

/*   NEW(node, s->dim_sof); */
/*   memcpy(node, _node, s->dim_sof); */
/*   NEW(newnode, s->dim_sof); */
/*   n = IndSubshiftInsert(s, node); */
/*   hits = 1; */

/*   while (!(n->flags & (1 << letter)) && hits) { */
/*     hits = IndSubshiftNewnode(s, letter, node, newnode); */
/*     n->flags |= (1 << letter); */
/*     printf("letter = %d\n", letter); */
/*     NodePrint(stdout, node, s->dim_sof); */
/*     NodePrint(stdout, newnode, s->dim_sof); */
/*     if (hits) { */
/*       newn = IndSubshiftInsert(s, newnode); */
/*       SparseMatrixAddEntry(A, newn->sub-1, n->sub-1, 1.0); */
/*       n = newn; */
/*       tmpnode = node; node = newnode; newnode = tmpnode; */
/*     } */
/*   } */
  
/*   FREE(newnode); */
/*   FREE(node); */
/* } */
 
void IndSubshiftExtend(IndSubshift *s, SparseMatrix *A, char *node, Node *n) {

  long l, l1;    /* letter */
  int hits;      /* number of nodes in sofic shift hit by _n */ 
  char *newnode;
  Node *newn;

  s->max_nodesize = max(s->max_nodesize, sumchars(node, s->dim_sof));

  NEW(newnode, s->dim_sof);

  for (l=0; l < s->no_of_letters; l++) {
 /*    mexPrintf("letter = %d\n", l);  */
    if (!(n->flags & ((long)1 << l))) {
      hits = IndSubshiftNewnode(s, l, node, newnode);
      n->flags |= ((long)1 << l);
  /*    NodePrint(node, s->dim_sof); 
      NodePrint(newnode, s->dim_sof);  */
      if (hits) {
        newn = IndSubshiftInsert(s, newnode);
        SparseMatrixAddEntry(A, newn->sub-1, n->sub-1, 1.0);
        IndSubshiftExtend(s, A, newnode, newn); 
      } 
    }
  }
  
  FREE(newnode);
  
  return;
}

SparseMatrix *IndSubshiftGraph(IndSubshift *s, int start) {
  int i, max_nodesize;
  char *node;
  SparseMatrix *A;
  Node *n;

  A = SparseMatrixNew(1);

  NEW(node, s->dim_sof);

/*   i = 0; */
/*   memset(node, 0, s->dim_sof); */
/*   do { */
/*     node[i] = 1; */
/*     n = IndSubshiftInsert(s, node); */
/*     IndSubshiftExtend(s, A, node, n); */
/*     node[i] = 0; */
/*     i++; */
/*   } while (i < s->dim_sof); */
  memset(node, 0, s->dim_sof);
  node[start] = 1;
  n = IndSubshiftInsert(s, node);
  IndSubshiftExtend(s, A, node, n);
/*  mexPrintf("max_nodesize = %d\n", s->max_nodesize); */

  FREE(node);

  return A;
}

/* SparseMatrix *IndSubshiftGraph(IndSubshift *s) { */
/*   char *node; */
/*   SparseMatrix *A; */
/*   int letter, i; */

/*   NEW(node, s->dim_sof); */
/*   A = SparseMatrixNew(1); */
  
/*   for (letter=0; letter<s->no_of_letters; letter++) { */ 
/*     memset(node, 0, s->dim_sof); */ 
/*     i=0; */ 
/*     while (s->letters[i]!=letter) i++; */ 
/*     node[i] = 1; */ 
/*     IndSubshiftPath(s, A, letter, node); */ 
/*   } */ 

/*   i = 0; */
/*   memset(node, 0, s->dim_sof); */
/*   node[i] = 1; */
/*   do { */
/*     node[i] = 1; */
/*     IndSubshiftPath(s, A, s->letters[i], node); */
/*     node[i] = 0; */
/*     i++; */
/*   } while (i < s->dim_sof); */

/*   FREE(node); */

/*   return A; */
/* } */


void IndSubshiftInfo(FILE *out, IndSubshift *s) {
  fprintf(out, "IndSubshift at %p:\n", s);
  if (s) {
    fprintf(out, "  dim_sof = %d\n", s->dim_sof);
    fprintf(out, "  dim_sub = %d\n", s->dim_sub);
    NodeInfo(out, 0, s->root);
  }
}
