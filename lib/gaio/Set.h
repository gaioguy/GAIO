/*
 * Copyright (c) 1999 Oliver Junge, for details see COPYING
 * 
 * Set.h
 * 
 */

#ifndef _Set_h
#define _Set_h

#include <stdio.h>
#include <gaio/defs.h>
#include <gaio/Stack.h>

static int __Set_no = 0;

typedef struct _SetElement SetElement;

struct _SetElement {
  int no;
  void *ptr;
  void (*Free)(void**);
  void (*Info)(FILE *out, int, void *);
  SetElement *parent, *child[2];
};

typedef struct {
  int no_of_elements;
  SetElement *root;
} Set;

SetElement *SetElementNew(int no, void *ptr, void (*Free)(void**),
			  void (*Info)(FILE*, int, void*));
void SetElementFree(SetElement **s);
int SetElementInsert(SetElement *s, SetElement *node);
void SetElementPushToStack(SetElement *se, Stack *st);
SetElement *SetElementContains(SetElement *s, int no);
void SetElementInfo(FILE *out, int n, SetElement *sn);

Set *SetNew();
void SetFree(Set **set);
Stack *SetToStack(Set *s);
int SetInsert(Set *s, SetElement *node);
SetElement *SetContains(Set *s, int no);
void SetInfo(FILE *out, int n, Set *s);


#endif
