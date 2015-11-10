/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Stack.c
 * 
 */

#include <gaio/Stack.h>
#include <gaio/defs.h>
#include <assert.h>
#include <stdio.h>

StackElement *StackElementNew(void *contents) {
  StackElement *s;
  NEW1(s, StackElement*, 1);
  s->contents = contents;
  
  return s;
}

void StackElementFree(StackElement **s) {
  if (*s) {
    FREE(*s);
    *s = 0;
  }
}

void StackElementInfo(FILE *out, StackElement *s) {
  if (s) 
    fprintf(out, "StackElement at %p\n", s);
}
 
Stack *StackNew(void) {
  Stack *s;
  NEW1(s, Stack*, 1);
  s->top = 0;
  
  return s;
}

void StackFree(Stack **s) {
  StackElement *e;
  if (*s) {
    while ((*s)->top) {
      e = (*s)->top;
      (*s)->top = e->next;
      StackElementFree(&e);
    }
    FREE(*s);
    *s = 0;
  }
}

void *StackHead(Stack *s) {
  return s->top->contents;
}

void StackPush(Stack *s, void *contents) {
  StackElement *e;
  
  assert(s);
  e = StackElementNew(contents);
  e->next = s->top;
  s->top = e;
  s->n++;
}

void *StackPop(Stack *s) {
  void *c;
  StackElement *e;

  assert(s);
  assert(s->top);
  e = s->top;
  c = e->contents;
  s->top = e->next;
  s->n--;
  StackElementFree(&e);

  return c;
}
 
void StackInfo(FILE *out, Stack *s) {
  StackElement *e;
  if (s) {
    fprintf(out, "Stack at %p\n", s);
    e = s->top;
    while (e) {
      StackElementInfo(out, e);
      e = e->next;
    }
  }
}
