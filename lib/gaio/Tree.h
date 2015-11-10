/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Tree.h
 *
 */

#ifndef _Tree_h
#define _Tree_h

#define SD_MAX 500 /* we allow max. 500 sd's in each coordinate */
#define DEPTH_MAX(d) ((d)*SD_MAX) 
#define DEPTH(tree, depth) (depth < 0 ? BoxDepth(tree->box) : ((long int)tree->depth))

#include <stdio.h>
#include <gaio/defs.h>
#include <gaio/Box.h>
#include <gaio/Rectangle.h>
#include <gaio/SparseVector.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  
  Rectangle *Q;               /* region of interest in phase space */
  Box *root;                  /* root of the tree */
  unsigned int *refcount;     /* reference count for the root box */
  Box *box;                   /* current box */
  double *c, *r;              /* center and radius of the current box */
  double *cStack, *rStack;    /* stacks for c and r */
  Box **boxStack;             /* stack for current box */
  unsigned int *branchStack;  /* stack for last visited branch in the tree */
  unsigned long depth;        /* current depth, i.e. depth of *box */
  byte *sd;                   /* array of bisection directions */

} Tree;

/* creates and returns a new tree */
Tree *TreeNew(Rectangle *Q);

/* deletes the tree *tree and sets *tree=0 */
void TreeFree(Tree **tree);                  

/* sets box=root and c and r to the values of the root box */
void TreeReset(Tree *tree);

/* changes the outer box (and possibly the dimension! */
void TreeChangeQ(Tree *tree, Rectangle *Q);

/* creates and returns a copy of tree, sharing the boxtree at root */
Tree *TreeCopy(Tree *tree); 

/* goes to child i of box, computing the new c, r and depth */
int  TreeDown(Tree *tree, unsigned int i);

/* goes to the parent of box, computing the corresponding values of c, r and
   depth */
int  TreeUp(Tree *tree);

/* goes to the next branch */
int  TreeNextBranch(Tree *tree);   

/* first part of iterator over the boxes of a certain depth */
int  TreeFirstBox(Tree *tree, int depth);   

/* second part of this iterator */
int  TreeNextBox(Tree *tree, int depth);

/* looks for x in the tree, returning
   1, if x is found; in this case tree->box is the box containing x;
   0, if x is not found;
   -1, if x is not conatined in the root box */
int TreeSearch(Tree *tree, double *x, int depth); 

/* returns a sparse vector of the numbers of those boxes on the given
   depth which intersect the Box B(c,r) */
SparseVector *TreeSearchBox(Tree *tree, double *c, double *r, int depth);

/* inserts a box on depth 'depth' into the tree which contains x (if
   possible at all), returning
   1, if successful;
   0, if box has already been there;
   -1, if x is not contained in the root box,
   a inserted box gets the flag 'flag0', a found box the flag flag1,
   and the box is colored according to 'color'
*/
int  TreeInsert(Tree *tree, double *x, int depth, byte flag0, byte flag1,
		byte color);

/* inserts all boxes on depth 'depth' into the tree which intersect the
   box B=B(c,r), returning
   1, if some box has been inserted into the tree;
   0, if no box has been inserted;
   -1, if B(c,r) does not intersect the root box,
   a inserted box gets the flag 'flag0', a box which has been already
   present the flag flag1, and the box is colored according to 'color' */
int TreeInsertBox(Tree *tree, double *c, double *r, int depth,
		  byte flag0, byte flag1, byte color);

/* subdivides all leaves which have their flag 'flag' set in the next 
   coordinate direction, returns the number of subdivided boxes*/
int  TreeSubdivide(Tree *tree, byte flag);

/* unsubdivides all leaves which have their flag 'flag' set 
   returns the number of unsubdivided boxes*/
int  TreeUnsubdivide(Tree *tree, byte flag);

/* removes all boxes which don't have their flag 'flag' set */
int  TreeRemove(Box *box, byte flag);       

/* deletes the depth 'depth' and all deeper depths */
void TreeDeleteDepth(Tree *tree, int depth);

/* sets the flag 'flag' of all boxes on depth (choice==0) or of those 
   for which choice[box->no]==1 */
void TreeSetFlags(Tree *tree, int depth, char *choice, byte flag);

/* unsets the flag 'flag' of all boxes on 'depth' (choice==0) or of those 
   for which choice[box->no]==1*/
void TreeUnsetFlags(Tree *tree, int depth, char *choice, byte flag);

/* changes the flags of all boxes on 'depth' (choice==0) or of those 
   for which choice[box->no]==1*/
void TreeChangeFlags(Tree *tree, int depth, char *choice, byte from, byte to);

/* sets the values of the array 'flags' according to flags of all boxes 
   on 'depth' */
void TreeGetFlags(Tree *tree, int depth, byte *flags);

/* returns the number of boxes in the tree which starts at box */
int TreeCount(Box *box);

/* returns the number of boxes on depth 'depth' and enumerates the boxes */
int TreeCountDepth(Tree *tree, int depth);

/* returns the depth of the tree starting at box 'box' */
int TreeDepth(Box *box);

/* print some information about the tree */
void TreeInfo(FILE *out, Tree *tree);     

/* prints all boxes on depth 'depth' in ASCII format */
void TreePrint(FILE *out, Tree *tree, int depth); 
void TreePrintToMatrix(Tree *tree, int depth, double *boxes); 

/* saves the tree to stream out */
void TreeSave(FILE *out, Tree *tree);

/* saves the boxes in the subtree starting at box */
void TreeSaveBox(FILE *out, Box *box);

/* loads a tree from stream in */
Tree *TreeLoad(FILE *in);

/* loads a subtree starting at box from stream in */
Box *TreeLoadBox(FILE *in);

/* computes a vector of box volumes on depth 'depth' */
void TreeVolume(Tree *tree, double *v, int depth); 

/* computes a histogram h of the orbit x */
void TreeHistogram(Tree *tree, double *x, int length, int depth, double *h);

/* colors the boxes on depth 'depth' according to the values of the vector v,
   the length of v  must coincide with the number of boxes on depth 'depth' */
void TreeColor(Tree *tree, char *v, int depth);

int TreeCollapse(Tree *tree);
int TreeUncollapse(Tree *tree, int depth);

#ifdef __cplusplus
}
#endif


#endif
