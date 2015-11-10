/*
 * Copyright (c) 1998,99 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Iter.c   
 *
 */

#include <gaio/Iter.h>
#include <gaio/defs.h>
#include <gaio/Box.h>
#include <gaio/Rectangle.h>
#include <gaio/Tree.h>
#include <assert.h>
#include <stdio.h>

Iter *IterNew(Rectangle *Q) {

  Iter *iter;
  NEW1(iter, Iter*, 1);
  iter->tree = TreeNew(Q);
  iter->searchTree = TreeCopy(iter->tree);
  iter->verbose = 1;

  return iter;
}

void IterFree(Iter **iter) {
  if (*iter) {
    TreeFree(&((*iter)->searchTree));
    TreeFree(&((*iter)->tree));
    FREE(*iter);
    *iter = NULL;
  }
}

void IterInfo(FILE *out, Iter *iter) {
  fprintf(out, "Iter at %p:\n", iter);
  if (iter) {
    fprintf(out, "  tree at %p:\n", iter->tree);
    fprintf(out, "  searchTree at %p:\n", iter->tree);
    fprintf(out, "  verbose = %d\n", iter->verbose);
  }
}

void IterSave(FILE *out, Iter *iter) {
  if (iter) {
    TreeSave(out, iter->tree);
  }
}

Iter *IterLoad(FILE *in) {

  Iter *iter;
  Tree *tree;

  tree = TreeLoad(in);
  if (!tree) {
    return 0;
  }

  NEW1(iter, Iter*, 1);
  iter->tree = tree;
  iter->searchTree = TreeCopy(iter->tree);

  return iter;
}


