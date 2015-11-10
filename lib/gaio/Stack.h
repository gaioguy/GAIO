/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Stack.h
 * 
 */

#ifndef _Stack_h
#define _Stack_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _StackElement StackElement;

struct _StackElement {
  void *contents;
  StackElement *next;
};

typedef struct {
  unsigned int n;     /* no of elements in stack */
  StackElement *top;  /* upmost element of stack */
} Stack;

/* con- and destructor */
StackElement *StackElementNew(void *contents);
void StackElementFree(StackElement **s);

/* prints some information about the stack element on the stream out */
void StackElementInfo(FILE *out, StackElement *s);

Stack *StackNew(void);
void StackFree(Stack **s);

void *StackHead(Stack *s);
void StackPush(Stack *s, void *contents);
void *StackPop(Stack *s);

void StackInfo(FILE *out, Stack *s);

#ifdef __cplusplus
}
#endif


#endif
