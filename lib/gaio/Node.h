/*
 * Copyright (c) 1999 Oliver Junge, for details see COPYING
 * 
 * Node.h
 * 
 */

#ifndef _Node_h
#define _Node_h

#include <stdio.h>
#include <gaio/defs.h>
#include <gaio/Set.h>

static int __Node_no = 0;

typedef struct _Node Node;

struct _Node {
  int sof;               /* number of node of sofic graph */
  int sub;               /* number within the induced subshift */
  unsigned long flags;    /* indicating, which letters have been parsed */
  Set *childs;
};

/* creates and returns new node */
Node *NodeNew(int sof);

/* deletes node *node and all children, sets *node=0  */
void NodeFree(Node **node);

/* inserts (or finds) a (new) node with number sof in the childs of
   parent, returns the inserted (or found) node */
Node *NodeInsert(Node *parent, int sof);

/* prints some information about the node on the stream out */
void NodeInfo(FILE *out, int n, Node *node);

/* saves the node to the stream out */
void NodeSave(FILE *out, Node *node);

/* loads a node from the stream in */
Node *NodeLoad(FILE *in);

#endif
