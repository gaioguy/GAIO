/*
 * Copyright (c) 1999 Oliver Junge, for details see COPYING
 * 
 * Node.c
 *
 */

#include <assert.h>
#include <gaio/Node.h>

/* creates a NEW Node */
Node *NodeNew(int sof) {
  Node *node;
  NEW1(node, Node*, 1);

  node->sof = sof;
  node->sub = 0;
  node->flags = 0;
  node->childs = SetNew();
  __Node_no++;

  return node;
}

/* deletes Node *node and recursively all children of it, sets *node=0 */
void NodeFree(Node **node) {
  int i;
  if (*node) {
    SetFree(&((*node)->childs));
    FREE(*node);
    *node = 0;
    __Node_no--;
  }
}

Node *NodeInsert(Node *parent, int sof) {
  Node *n;
  SetElement *s;

  s = SetContains(parent->childs, sof);
  if (!s) {
    n = NodeNew(sof);
    s = SetElementNew(sof, (void *)n, (void (*)(void**))NodeFree, 
		      (void (*)(FILE*, int, void*))NodeInfo);
    SetInsert(parent->childs, s);
  } else
    n = (Node *)s->ptr;
 
  return n;
}

void NodeInfo(FILE *out, int n, Node *node) {
  if (node) {
    fputns(' ', n, out);
    fprintf(out, "(%d,%d,%lu)\n", node->sof, node->sub, node->flags);
    SetInfo(out, n+7, node->childs);
  }     
}

void NodeSave(FILE *out, Node *node) {
  if (node) {
  }
}

Node *NodeLoad(FILE *in) {
  Node *node;

  return node;
}

