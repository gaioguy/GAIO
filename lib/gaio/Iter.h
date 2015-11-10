/*
 * Copyright (c) 1998,99 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Iter.h   
 *
 */

#ifndef _Iter_h
#define _Iter_h

#include <stdio.h>
#include <gaio/defs.h>
#include <gaio/Rectangle.h>
#include <gaio/Tree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

  Tree *tree;                 /* tree of boxes */
  Tree *searchTree;           /* copy of tree for searching */
  int verbose;                /* output debug information, if == 1 */

} Iter;

/* creates and returns a new iter structure */
Iter *IterNew(Rectangle *Q);

/* deletes **iter and set *iter=0 */
void IterFree(Iter **iter);

/* prints some information about the iter structure */
void IterInfo(FILE *out, Iter *iter);

/* saves the iter structure including the tree ti file out */
void IterSave(FILE *out, Iter *iter);

/* loads an iter structure including the tree from file in */
Iter *IterLoad(FILE *in); 

#ifdef __cplusplus
}
#endif

#endif
