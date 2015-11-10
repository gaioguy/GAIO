/*
 * Copyright (c) 1999 Oliver Junge, for details see COPYING
 * 
 * Set.c
 *
 */

#include <assert.h>
#include <gaio/Set.h>

SetElement *SetElementNew(int no, void *ptr, void (*Free)(void**),
			  void (*Info)(FILE*, int, void*)) {
  SetElement *s;
  
  NEW1(s, SetElement*, 1);
  s->no = no;
  s->ptr = ptr;
  s->Free = Free;
  s->Info = Info;
  s->parent = 0;
  s->child[0] = 0;
  s->child[1] = 0;

  return s;
}

void SetElementFree(SetElement **s) {
  if (*s) {
    if ((*s)->Free) (*s)->Free(&((*s)->ptr));
    SetElementFree(&((*s)->child[0]));
    SetElementFree(&((*s)->child[1]));
    FREE(*s);
    *s = 0;
  }
}

int SetElementInsert(SetElement *s, SetElement *node) {
  if (node->no < s->no) {
    if (!s->child[0]) {
      s->child[0] = node;
      node->parent = s;
      return 1;
    } else
      return SetElementInsert(s->child[0], node);
  } else if (node->no > s->no) {
    if (!s->child[1]) {
      s->child[1] = node;
      node->parent = s;
      return 1;
    } else
      return SetElementInsert(s->child[1], node);
  } 
  return 0;
}

void SetElementPushToStack(SetElement *se, Stack *st) {
  if (se) {
    StackPush(st, (void *)se);
    SetElementPushToStack(se->child[0], st);
    SetElementPushToStack(se->child[1], st);
  }
}
   
SetElement *SetElementContains(SetElement *s, int no) {
  if (!s)
    return 0;
  if (s->no==no)
    return s;
  else {
    if (no < s->no)
      return SetElementContains(s->child[0], no);
    else
      return SetElementContains(s->child[1], no);
  }
}

void SetElementInfo(FILE *out, int n, SetElement *sn) {
  if (sn) {
    if (sn->Info) sn->Info(out, n, sn->ptr);
    if (!sn->child[0] && !sn->child[1])
      fprintf(out, "\n");
    else {
      SetElementInfo(out, n, sn->child[0]);
      SetElementInfo(out, n, sn->child[1]);
    }
  }
}

Set *SetNew() {
  Set *set;
  
  NEW1(set, Set*, 1);
  set->no_of_elements = 0;
  set->root = 0;
  __Set_no++;

  return set;
}

void SetFree(Set **s) {
  if (*s) {
    SetElementFree(&((*s)->root));
    FREE(*s);
    *s = 0;
    __Set_no--;
  }
}

Stack *SetToStack(Set *s) {
  Stack *st;
 
  st = StackNew();
  SetElementPushToStack(s->root, st);

  return st;
}

int SetInsert(Set *s, SetElement *node) {
  int r;

  if (!s->root) {
    s->root = node;
    r = 1;
  } else 
    r = SetElementInsert(s->root, node);

  s->no_of_elements += r;
    
  return r;
}

SetElement *SetContains(Set *s, int no) {
  return SetElementContains(s->root, no);
}

void SetInfo(FILE *out, int n, Set *s) {
  if (s) {
    SetElementInfo(out, n, s->root);
  }
}

