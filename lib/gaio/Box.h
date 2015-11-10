/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Box.h
 * 
 */

#ifndef _Box_h
#define _Box_h

#include <stdio.h>
#include <gaio/defs.h>
#include <gaio/SparseVector.h>

#ifdef _NEIGHBOURS_
#include <gaio/Stack.h>
#endif

/* flags */
#define NONE               0x00
#define HIT                0x01
#define INSERTED           0x02
#define EXPAND             0x04
#define SUBDIVIDE          0x08
#define EQUILIBRIUM        0x10
#define ENTERREGION        0x20
#define EXITREGION         0x40

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Box Box;

struct _Box {

  byte sd;         /* coordinate in which box is/will be subdivided */
  byte flags;      /* space for various flags */    
  byte color;      /* the color of that box */
  Box *child[2];   /* pointer to the children */

#ifdef _MEASURE_
  int no;            /* number, needed for measure computation */
  SparseVector *img; /* sparse vector of transition probabilities */
#endif

#ifdef _NORMALS_
  char n[3];       /* normal, only for 3space */
  float l;         /* smallest eigenvalue of weight matrix */
#endif

#ifdef _NEIGHBOURS_
  Box *parent;     /* pointer to the parent box */
  char tau;        /* tau==i, if box==parent->son[i], i=0,1 */
#endif

};

/* creates and returns new box */
Box *BoxNew(void);

/* deletes box *box and all children, sets *b=0  */
void BoxFree(Box **box);

/* returns the volume of a d-dimensional box with radius r */
double BoxVolume(double *r, char d);

/* subdivides box, stores sd in their children */
int  BoxSubdivide(Box *box, byte sd); 

/* unsubdivides box */
int  BoxUnsubdivide(Box *box); 

/* returns the number of children */
unsigned int  BoxNoOfChildren(Box *box);    

/* flag handling functions */
void BoxSetFlag(Box *box, byte flag);
void BoxUnsetFlag(Box *box, byte flag);
void BoxChangeFlag(Box *box, byte from, byte to);
byte BoxGetFlags(Box *box);

/* returns the relative depth of the box: 
   -1, if it has no children (a "leaf"); 
   -2, if it is parent of a leaf;
   0   otherwise */
int  BoxDepth(Box *box);

/* prints some information about the box on the stream out */
void BoxInfo(FILE *out, Box *box);

/* saves the box to the stream out */
void BoxSave(FILE *out, Box *box);

/* loads a box from the stream in */
Box *BoxLoad(FILE *in);

#ifdef _NEIGHBOURS_
/* returns a pointer to the (i, sigma)-neighbour of b, i=0,...,MAXDIM-1, 
   sigma=0,1 or 0, if not exsistant */
Box *BoxNeighbour(Box *b, byte i, byte sigma);

/* returns a stack containing all neighbours of b */
Stack *BoxGetNeighbours(Box *b, byte dim);

void BoxExpandNeighbours(Box *ngb, byte i, byte sigma, Stack *s);
#endif /* _NEIGHBOURS_ */

int BoxCollapse(Box *box);

#ifdef __cplusplus
}
#endif

#endif
