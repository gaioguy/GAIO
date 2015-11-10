/*
 * Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 *
 * Tree.c
 *
 */

#include <gaio/Tree.h>
#include <gaio/defs.h>
#include <gaio/Box.h>
#include <gaio/Rectangle.h>
#include <gaio/SparseVector.h>
#include <gaio/Vec.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

Tree *TreeNew(Rectangle *Q) {

  unsigned int i, dim;
  Tree *tree;

  assert(Q);
  dim = Q->dim;
  NEW1(tree, Tree*, 1);
  tree->Q = RectangleNew(Q->c, Q->r, dim);
  tree->root = BoxNew();
  NEW1(tree->refcount, unsigned int*, 1);
  *(tree->refcount) = 1;
  NEW1(tree->c, double*, dim);
  NEW1(tree->r, double*, dim);
  NEW1(tree->cStack, double*, DEPTH_MAX(dim));
  NEW1(tree->rStack, double*, DEPTH_MAX(dim));
  NEW1(tree->boxStack, Box**, DEPTH_MAX(dim));
  NEW1(tree->branchStack, unsigned int *, DEPTH_MAX(dim));
  NEW1(tree->sd, byte*, DEPTH_MAX(dim)+1);
  for (i=0; i<DEPTH_MAX(dim)+1; i++) tree->sd[i] = i % dim;
  TreeReset(tree);

  return tree;
}

void TreeFree(Tree **tree) {
  if (*tree) {
    FREE((*tree)->branchStack);
    FREE((*tree)->boxStack);
    FREE((*tree)->rStack);
    FREE((*tree)->cStack);
    FREE((*tree)->r);
    FREE((*tree)->c);
    *((*tree)->refcount) -= 1;
    if (*((*tree)->refcount)==0) {
      BoxFree(&((*tree)->root));
      RectangleFree(&((*tree)->Q));
      FREE((*tree)->sd);
    }
    FREE(*tree);
    *tree = 0;
  }
}

void TreeReset(Tree *tree) {
  assert(tree);

  tree->box = tree->root;
  VecCopy(tree->Q->c, tree->c, tree->Q->dim);
  VecCopy(tree->Q->r, tree->r, tree->Q->dim);
  tree->depth = 0;
}

void TreeChangeQ(Tree *tree, Rectangle *Q) {

  int dim;

  assert(tree);
  assert(Q);
  dim = Q->dim;

  RectangleInfo(stdout, tree->Q);

  FREE(tree->branchStack);
  FREE(tree->boxStack);
  FREE(tree->rStack);
  FREE(tree->cStack);
  FREE(tree->r);
  FREE(tree->c);
  RectangleFree(&(tree->Q));

  RectangleInfo(stdout, Q);

  tree->Q = RectangleNew(Q->c, Q->r, dim);
  NEW1(tree->c, double*, dim);
  NEW1(tree->r, double*, dim);
  NEW1(tree->cStack, double*, DEPTH_MAX(dim));
  NEW1(tree->rStack, double*, DEPTH_MAX(dim));
  NEW1(tree->boxStack, Box**, DEPTH_MAX(dim));
  NEW1(tree->branchStack, unsigned int*, DEPTH_MAX(dim));

  TreeReset(tree);
}

Tree *TreeCopy(Tree *tree) {
  Tree *copy;

  assert(tree);

  NEW1(copy, Tree*, 1);
  copy->Q = tree->Q;
  copy->root = tree->root;
  *(tree->refcount) += 1;
  copy->refcount = tree->refcount;
  NEW1(copy->c, double*, tree->Q->dim);
  NEW1(copy->r, double*, tree->Q->dim);
  NEW1(copy->cStack, double*, DEPTH_MAX(tree->Q->dim));
  NEW1(copy->rStack, double*, DEPTH_MAX(tree->Q->dim));
  NEW1(copy->boxStack, Box**, DEPTH_MAX(tree->Q->dim));
  NEW1(copy->branchStack, unsigned int*, DEPTH_MAX(tree->Q->dim));
  copy->sd = tree->sd;
  TreeReset(copy);

  return copy;
}

int TreeDown(Tree *tree, unsigned int i) {
  assert(tree);
  assert(tree->depth < (unsigned long int)DEPTH_MAX(tree->Q->dim));
  assert(i==0 || i==1);

  if (tree->box->child[i]) {
    unsigned int sd = tree->box->sd;
    tree->cStack[tree->depth] = tree->c[sd];
    tree->rStack[tree->depth] = tree->r[sd];
    tree->boxStack[tree->depth] = tree->box;
    tree->r[sd] /= 2;
    if (i==0) {
      tree->c[sd] -= tree->r[sd];
      tree->box = tree->box->child[0];
    } else {
      tree->c[sd] += tree->r[sd];
      tree->box = tree->box->child[1];
    }
    tree->branchStack[tree->depth] = i;
    tree->depth++;

    return 1;
  } else

    return 0;
}

int TreeUp(Tree *tree) {
  assert(tree);

  if (tree->depth==0) return 0;
  tree->depth--;
  tree->box = tree->boxStack[tree->depth];
  tree->c[tree->box->sd] = tree->cStack[tree->depth];
  tree->r[tree->box->sd] = tree->rStack[tree->depth];

  return 1;
}

int TreeNextBranch(Tree *tree) {
  int down = 0;

  do {
    if (!TreeUp(tree)) return 0;
    if (tree->branchStack[tree->depth]==0)
      down = TreeDown(tree, 1);
  } while (!down);

  return 1;
}

int TreeFirstBox(Tree *tree, int depth) {

  assert(tree);
  assert(depth==-1 || depth <= DEPTH_MAX(tree->Q->dim));

  TreeReset(tree);

  while (DEPTH(tree, depth)!=depth)
    if (!(TreeDown(tree, 0) || TreeDown(tree, 1)))
      if (!TreeNextBranch(tree)) return 0;

  return DEPTH(tree, depth)==depth;
}

int TreeNextBox(Tree *tree, int depth) {

  assert(tree);
  assert(depth==-1 || depth <= DEPTH_MAX(tree->Q->dim));

  if (!TreeNextBranch(tree)) return 0;

  while (DEPTH(tree, depth)!=depth)
    if (!(TreeDown(tree, 0) || TreeDown(tree, 1)))
      if (!TreeNextBranch(tree)) return 0;

  return DEPTH(tree, depth)==depth;
}

int TreeSearch(Tree *tree, double *x, int depth) {
  assert(tree);
  assert(depth==-1 || depth <= DEPTH_MAX(tree->Q->dim));

  if (RectangleContains(tree->Q, x)) {
    TreeReset(tree);
    while (DEPTH(tree, depth)!=depth) {
      int i = tree->c[tree->box->sd] < x[tree->box->sd];
      if (!TreeDown(tree, i)) break;
    }
    return DEPTH(tree, depth)==depth;
  }
  return -1;
}

#ifdef _MEASURE_
void TreeSearchBoxRec(Tree *tree, double *a, double *b, int depth,
		 SparseVector *nos) {
  int i;

  if (depth==DEPTH(tree, depth)) {
    SparseVectorAddEntry(nos, tree->box->no, 1.0);
    return;
  }
  i = tree->box->sd;
  if (interval_intersect(tree->c[i]-tree->r[i], tree->c[i], a[i], b[i])) {
    if (TreeDown(tree, 0)) {
      TreeSearchBoxRec(tree, a, b, depth, nos);
      TreeUp(tree);
    }
  }
  if (interval_intersect(tree->c[i], tree->c[i]+tree->r[i], a[i], b[i])) {
    if (TreeDown(tree, 1)) {
      TreeSearchBoxRec(tree, a, b, depth, nos);
      TreeUp(tree);
    }
  }
}

SparseVector *TreeSearchBox(Tree *tree, double *c, double *r, int depth) {
  SparseVector *nos;
  double a[MAXDIM], b[MAXDIM];
  int i, intsct, dim = tree->Q->dim;

  nos = SparseVectorNew(0, 10);
/*   SparseVectorInfo(stdout, nos); */
  TreeReset(tree);
  intsct = 1;
  for (i=0; i<dim; i++) {
    a[i] = c[i] - r[i];
    b[i] = c[i] + r[i];
    if (!interval_intersect(tree->c[i]-tree->r[i], tree->c[i]+tree->r[i],
			    a[i], b[i]))
      intsct = 0;
  }
/*   VecPrint(stdout, a, dim); */
/*   VecPrint(stdout, b, dim); */

  if (depth==0) {
    if (intsct) SparseVectorAddEntry(nos, 0, 1);
  } else
    TreeSearchBoxRec(tree, a, b, depth, nos);

  return nos;
}
#endif

int TreeInsert(Tree *tree, double *x, int depth, byte flag0, byte flag1,
	       byte color) {

  assert(tree);
  assert(depth==-1 || depth<=DEPTH_MAX(tree->Q->dim));

  if (RectangleContains(tree->Q, x)) {
    int inserted = 0;
    TreeReset(tree);
    while (DEPTH(tree, depth)!=depth) {
      int i;
      if (!tree->box->child[0] && !tree->box->child[1])
	tree->box->sd = tree->sd[tree->depth];
      i = ((tree->c)[tree->box->sd] < x[tree->box->sd]);
      if (!TreeDown(tree, i)) {
	tree->box->child[i] = BoxNew();
	BoxSetFlag(tree->box->child[i], flag0);
	TreeDown(tree, i);
	inserted = 1;
      } else {
	BoxSetFlag(tree->box, flag1);
      }
      tree->box->color = color;
    }
    return inserted;
  }
  return -1;
}

void TreeInsertBoxRec(Tree *tree, double *a, double *b, int depth,
		     byte flag0, byte flag1, byte color, int *inserted) {
  int i;

/*   printf("depth = %d, DEPTH = %d, tree->depth = %d\n",  */
/* 	 depth, DEPTH(tree, depth), tree->depth); */
  if (depth==DEPTH(tree, depth)) return;

  if (!tree->box->child[0] && !tree->box->child[1])
    tree->box->sd = tree->sd[tree->depth];
  i = tree->box->sd;
  if (interval_intersect(tree->c[i]-tree->r[i], tree->c[i], a[i], b[i])) {
/*     printf("0: [%lg, %lg], [%lg, %lg]\n",tree->c[i]-tree->r[i], tree->c[i], a[i], b[i]); */
    if (!TreeDown(tree, 0)) {
      tree->box->child[0] = BoxNew();
      BoxSetFlag(tree->box->child[0], flag0);
      TreeDown(tree, 0);
      *inserted = 1;
    } else {
      BoxSetFlag(tree->box, flag1);
    }
    tree->box->color = color;
    TreeInsertBoxRec(tree, a, b, depth, flag0, flag1, color, inserted);
    TreeUp(tree);
  }
  if (interval_intersect(tree->c[i], tree->c[i]+tree->r[i], a[i], b[i])) {
/*     printf("1: [%lg, %lg], [%lg, %lg]\n",tree->c[i], tree->c[i]+tree->r[i], a[i], b[i]); */
    if (!TreeDown(tree, 1)) {
      tree->box->child[1] = BoxNew();
      BoxSetFlag(tree->box->child[1], flag0);
      TreeDown(tree, 1);
      *inserted = 1;
    } else {
      BoxSetFlag(tree->box, flag1);
    }
    tree->box->color = color;
    TreeInsertBoxRec(tree, a, b, depth, flag0, flag1, color, inserted);
    TreeUp(tree);
  }
}

int TreeInsertBox(Tree *tree, double *c, double *r, int depth,
			    byte flag0, byte flag1, byte color) {
  double a[MAXDIM], b[MAXDIM];
  int i, inserted, intersects, dim = tree->Q->dim;

  assert(depth>0);
  assert(depth<=DEPTH_MAX(tree->Q->dim));

  TreeReset(tree);

  intersects = 1;
  for (i=0; i<dim; i++) {
    a[i] = c[i] - r[i];
    b[i] = c[i] + r[i];
    if (!interval_intersect(tree->c[i]-tree->r[i], tree->c[i]+tree->r[i],
			    a[i], b[i]))
      intersects = 0;
  }

  inserted = intersects ? 0 : -1;
  if (intersects)
    TreeInsertBoxRec(tree, a, b, depth, flag0, flag1, color, &inserted);

  return inserted;
}

int TreeSubdivide(Tree *tree, byte flag) {
  int no = 0;

  assert(tree);

  if (TreeFirstBox(tree, -1)) do {
    if (tree->box->flags & flag) {
      BoxSubdivide(tree->box, tree->sd[tree->depth]);
      no++;
    }
  } while (TreeNextBox(tree, -1));

  return no;
}

int TreeUnsubdivide(Tree *tree, byte flag) {
  int no = 0;

  assert(tree);

  if (TreeFirstBox(tree, -2)) do {
    if (tree->box->flags & flag) {
      BoxUnsubdivide(tree->box);
      no++;
    }
  } while (TreeNextBox(tree, -2));

  return no;
}

int TreeRemove(Box *box, byte flag) {
  int i;

  for (i=0; i<2; i++) {
    if (box->child[i]) {
      if (TreeRemove(box->child[i], flag))
	BoxFree(&(box->child[i]));
      if (BoxNoOfChildren(box)==0)
	box->flags &= ~flag;
    }
  }

  return (BoxNoOfChildren(box)==0 && !((box->flags & flag)==flag));
}

void TreeDeleteDepth(Tree *tree, int depth) {
  if (depth==0) {
    BoxFree(&(tree->root->child[0]));
    BoxFree(&(tree->root->child[1]));
  } else
    if (TreeFirstBox(tree, depth-1)) do {
      BoxFree(&(tree->box->child[0]));
      BoxFree(&(tree->box->child[1]));
    } while (TreeNextBox(tree, depth-1));
}

void TreeSetFlags(Tree *tree, int depth, char *choice, byte flag) {
  int no = 0;

  if (TreeFirstBox(tree, depth)) do {
    if (!strcmp(choice, "all") || choice[no++]=='1')
      BoxSetFlag(tree->box, flag);
  } while (TreeNextBox(tree, depth));
}

void TreeUnsetFlags(Tree *tree, int depth, char *choice, byte flag) {
  int no = 0;

  if (TreeFirstBox(tree, depth)) do {
    if (!strcmp(choice, "all") || choice[no++]=='1')
      BoxUnsetFlag(tree->box, flag);
  } while (TreeNextBox(tree, depth));
}

void TreeChangeFlags(Tree *tree, int depth, char *choice, byte from, byte to) {
  int no = 0;

  if (TreeFirstBox(tree, depth)) do {
    if (!strcmp(choice, "all") || choice[no++]=='1')
      BoxChangeFlag(tree->box, from, to);
  } while (TreeNextBox(tree, depth));
}

void TreeGetFlags(Tree *tree, int depth, byte *flags) {
  int no =0;

  if (TreeFirstBox(tree, depth)) do
    flags[no++] = BoxGetFlags(tree->box);
  while (TreeNextBox(tree, depth));
}

int TreeCount(Box *box) {
  int no0, no1;
  if (box) {
    no0 = TreeCount(box->child[0]);
    no1 = TreeCount(box->child[1]);
    return (no0 + no1 + 1);
  } else
    return 0;
}

int TreeCountDepth(Tree *tree, int depth) {
  int no = 0;

  assert(tree);
  assert(depth==-1 || depth <= DEPTH_MAX(tree->Q->dim));

  if (TreeFirstBox(tree, depth)) do {
#ifdef _MEASURE_
    tree->box->no = no++;
#else
    no++;
#endif
  } while (TreeNextBox(tree, depth));

  return no;
}

int TreeDepth(Box *box) {
  int l, r;
  if (box) {
    l = TreeDepth(box->child[0]) + 1;
    r = TreeDepth(box->child[1]) + 1;
    return max(l, r);
  }
  return -1;
}

void TreePrint(FILE *out, Tree *tree, int depth) {
  int i;

  assert(tree);
  assert(depth==-1 || depth <= DEPTH_MAX(tree->Q->dim));

  if (TreeFirstBox(tree, depth)) do {
    for (i=0; i<tree->Q->dim; i++) fprintf(out,"%g ",tree->c[i]);
    for (i=0; i<tree->Q->dim; i++) fprintf(out,"%g ",tree->r[i]);
    fprintf(out,"%u", tree->box->flags);
    fprintf(out, "\n");
  } while (TreeNextBox(tree, depth));
}

void TreePrintToMatrix(Tree *tree, int depth, double *boxes) {
  int dim, i, j = 0;

  assert(tree);
  dim = tree->Q->dim;
  assert(depth==-1 || depth <= DEPTH_MAX(dim));

  if (TreeFirstBox(tree, depth)) do {
    for (i=0; i<dim; i++) boxes[j*(2*dim+2) + i] = tree->c[i];
    for (i=0; i<dim; i++) boxes[j*(2*dim+2) + dim + i] = tree->r[i];
    boxes[j*(2*dim+2) + 2*dim] = tree->box->flags;
    boxes[j*(2*dim+2) + 2*dim + 1] = tree->box->color;
    j++;
  } while (TreeNextBox(tree, depth));

}

#ifdef _MEASURE_
void TreeVolume(Tree *tree, double *v, int depth) {
  int dim;

  assert(tree);
  dim = tree->Q->dim;

  if (TreeFirstBox(tree, depth)) do {
    int i = tree->box->no;
    v[i] = BoxVolume(tree->r, dim);
  } while (TreeNextBox(tree, depth));
}


void TreeHistogram(Tree *tree, double *x, int length, int depth, double *h) {
  int dim, i;

  assert(tree);
  dim = tree->Q->dim;

  for (i=0; i<length; i++) {
    if (TreeSearch(tree, x + i*dim, depth)>0)
      h[tree->box->no] += 1;
  }
}
#endif

void TreeColor(Tree *tree, char *v, int depth) {

  int i = 0;

  if (TreeFirstBox(tree, depth)) do {
    tree->box->color = v[i++];
  } while (TreeNextBox(tree, depth));
}

void TreeSave(FILE *out, Tree *tree) {
  fprintf(out, "Tree at %p:\n", tree);
  if (tree) {
#ifdef _NORMALS_
    float one = 1;
#endif
    RectangleSave(out, tree->Q);
    fprintf(out, "  no of boxes = %d\n", TreeCount(tree->root));
#ifdef _NORMALS_
    fprintf(out, "  normals saved, 1 = ");
    fwrite(&one, sizeof(float), 1,out);
    fprintf(out, "\n");
#endif
    TreeSaveBox(out, tree->root);
  }
}

void TreeSaveBox(FILE *out, Box *box) {
  BoxSave(out, box);
  if (box) {
    TreeSaveBox(out, box->child[0]);
    TreeSaveBox(out, box->child[1]);
  }
}

Tree *TreeLoad(FILE *in) {
  Rectangle *Q;
  Tree *tree;
  int n;
#ifdef _NORMALS_
  float one;
#endif
  if (!fscanf(in, "Tree at %p:\n", &tree)) return 0;
  if (tree) {
    if (!(Q = RectangleLoad(in))) return 0;
    if (!fscanf(in, "  no of boxes = %d\n", &n)) return 0;
#ifdef _NORMALS_
    fscanf(in, "  normals saved, 1 = ");
    fread(&one, sizeof(float), 1, in);
    fscanf(in, "\n");
#endif
    if (!(tree = TreeNew(Q))) return 0;
    if (!(tree->root = TreeLoadBox(in))) return 0;

    /* reconstructing the sd vector */
    TreeReset(tree);
    do {
      tree->sd[tree->depth] = tree->box->sd;
    } while(TreeDown(tree, 0) || TreeDown(tree,1));

    RectangleFree(&Q);
  }
  return tree;
}

Box *TreeLoadBox(FILE *in) {
  Box *box;
  box = BoxLoad(in);
  if (box) {
    box->child[0] = TreeLoadBox(in);
    box->child[1] = TreeLoadBox(in);
  }
  return box;
}

void TreeInfo(FILE *out, Tree *tree) {
  fprintf(out, "Tree at %p:\n", tree);
  if (tree) {
    RectangleInfo(out, tree->Q);
    fprintf(out, "  root: ");   BoxInfo(out, tree->root);
    fprintf(out, "  refcount = %u\n", *(tree->refcount));
    fprintf(out, "  box: ");    BoxInfo(out, tree->box);
    fprintf(out, "  DEPTH_MAX = %u\n", DEPTH_MAX(tree->Q->dim));
  }
}

int  TreeCollapse(Tree *tree) {
  assert(tree);

  BoxCollapse(tree->root);
  return TreeCountDepth(tree, -1);
}

void TreeUncollapseRec(Tree *tree, int depth) {
  if ((int)tree->depth < depth) {
    if (tree->box->child[0] == NULL && tree->box->child[1] == NULL) {
      BoxSubdivide(tree->box, tree->sd[tree->depth]);
    }

    if (TreeDown(tree, 0)) {
      TreeUncollapseRec(tree, depth);
      TreeUp(tree);
    }
    if (TreeDown(tree, 1)) {
      TreeUncollapseRec(tree, depth);
      TreeUp(tree);
    }
  }
}

int TreeUncollapse(Tree *tree, int depth) {
  assert(tree);

  if (depth < 0) {
    depth = TreeDepth(tree->root);
  }

  TreeReset(tree);
  TreeUncollapseRec(tree, depth);

  return TreeCountDepth(tree, -1);
}

