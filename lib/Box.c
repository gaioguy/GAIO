/*
* Copyright (c) 1998 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
*
* Box.c
*
*/

#include <gaio/Box.h>
#include <gaio/defs.h>
#include <gaio/SparseVector.h>
#include <stdio.h>

#ifdef _NEIGHBOURS_
#include <gaio/Stack.h>
#include <assert.h>
#endif

/* creates a NEW Box */
Box *BoxNew(void) {
	Box *box;
	NEW1(box, Box*, 1);
	box->sd = 0;
	box->flags = 0;
	box->color = 1;    /* 1 and not 0 since 0 means 'no box here' */
	box->child[0] = 0;
	box->child[1] = 0;

#ifdef _MEASURE_
	box->no = -1;
	box->img = 0;
#endif

#ifdef _NORMALS_
	box->n[0] = box->n[1] = box->n[2] = 0;
	box->l = 0;
#endif

	return box;
}

/* deletes Box b and recursively all children of it, sets *b=0 */
void BoxFree(Box **box) {
	if (*box) {

#ifdef _MEASURE_
		if ((*box)->img) SparseVectorFree(&((*box)->img));
#endif

		BoxFree(&((*box)->child[0]));
			BoxFree(&((*box)->child[1]));
		FREE(*box);
		*box = 0;
	}
}

double BoxVolume(double *r, char d) {
	int i;
	double v = 1;
	for (i=0; i<d; i++) v *= 2*r[i];
	return v;
}

int BoxSubdivide(Box *box, unsigned char sd) {
	if (BoxNoOfChildren(box)==0) {
		box->child[0] = BoxNew();
		box->child[1] = BoxNew();
		box->sd = sd;
#ifdef _NEIGHBOURS_
		box->child[0]->parent = box;
		box->child[1]->parent = box;
		box->child[0]->tau = 0;
		box->child[1]->tau = 1;
#endif
		return 1;
	}
	fprintf(stderr, "warning: libgaio: BoxSubdivide(): box is no leaf.\n");
	return 0;
}

int BoxUnsubdivide(Box *box) {
	if (BoxNoOfChildren(box)!=0) {
		BoxFree(&(box->child[0]));
		BoxFree(&(box->child[1]));
		return 1;
	}
	return 0;
}

void BoxInfo(FILE *out, Box *box) {
	unsigned int i;
	fprintf(out, "Box at %p:\n", box);
	if (box) {
		fprintf(out, "  sd = %u\n", box->sd);
		fprintf(out, "  flags = 0x%.2x\n", box->flags);
		fprintf(out, "  color = %d\n", box->color);
		fprintf(out, "  children:");
		for (i=0; i<2; i++)
			fprintf(out, " %p", box->child[i]);
#ifdef _MEASURE_
		fprintf(out, "  no = %d", box->no);
#endif
#ifdef _NEIGHBOURS_
		fprintf(out, "  parent = %p", box->parent);
		fprintf(out, "  tau = %d", box->tau);
#endif
	}
	fprintf(out, "\n");
}

unsigned int BoxNoOfChildren(Box *box) {
	if (box) {
		if (box->child[0] && box->child[1]) return 2;
		if (box->child[0] || box->child[1]) return 1;
	}
	return 0; /* if b==0, it has no children, too */
}

void BoxSetFlag(Box *box, byte flag) {
	if (box) box->flags |= flag;
}

void BoxUnsetFlag(Box *box, byte flag) {
	if (box) box->flags &= ~flag;
}

void BoxChangeFlag(Box *box, byte from, byte to) {
	if (box) {
		if (box->flags & from) {
			box->flags &= ~from;
			box->flags |= to;
		}
	}
}

byte BoxGetFlags(Box *box) {
	if (box)
		return box->flags;
	else {
		fprintf(stderr, "error: libgaio: BoxGetFlags called with invalid argument.");
		return 0;
	}
}

int BoxDepth(Box *box) {
	if (box) {
		if (BoxNoOfChildren(box)==0) return -1;
		if (BoxNoOfChildren(box->child[0])==0 &&
			    BoxNoOfChildren(box->child[1])==0) return -2;
	}
	return 0;
}

void BoxSave(FILE *out, Box *box) {
	if (box) {
		fwrite((void *)&box->color, 1, 1, out);
		fwrite((void *)&box->sd, 1, 1, out);
		fwrite((void *)&box->flags, 1, 1, out);
#ifdef _NORMALS_
		fwrite((void *)box->n, 1, 3, out);
		fwrite((void *)&box->l, 4, 1, out);
#endif
	}
	else
	    fprintf(out, "%c", 0);
	return;
}

Box *BoxLoad(FILE *in) {
	int C;
	Box *box;

	C = fgetc(in);
	if (C==EOF || C==0) return 0;

	box = BoxNew();
	box->color = (unsigned char) C;
	fread((void *)&box->sd, 1, 1, in);
	fread((void *)&box->flags, 1, 1, in);
#ifdef _NORMALS_
	fread((void *)box->n, 1, 3, in);
	fread((void *)&box->l, 4, 1, in);
#endif


	return box;
}

#ifdef _NEIGHBOURS_

Box *BoxNeighbour(Box *b, byte i, byte sigma) {
	Box *ngb, *child, *parent;
	byte tau = b->tau;
	if (!b) return 0;
	parent = b->parent;
	if (!parent) return 0;
	if (i==parent->sd) {
		/*     printf("i==sd, "); */
		if (sigma!=tau) {
			/*       printf("sigma!=tau, "); */
			/*       printf("parent->children=%d, ", BoxNoOfChildren(parent)); */
			return parent->child[(tau + 1) % 2];
		}
		else {
			ngb = BoxNeighbour(parent, i, sigma);
			if (ngb==0) return 0;
			else {
				child = ngb->child[(tau + 1) % 2];
				return (BoxNoOfChildren(ngb)!=0 ? child : ngb);
			}
		}
	}
	else { /* (i!=sd) */
		ngb = BoxNeighbour(parent, i, sigma);
		if (ngb==0) return 0;
		else {
			child = ngb->child[tau];
			return (BoxNoOfChildren(ngb)!=0 ? child : ngb);
		}
	}
}

Stack *BoxGetNeighbours(Box *b, byte dim) {
	byte i, sigma;
	Box *ngb;
	Stack *s;
	if (!b) return 0;
	s = StackNew();
	/*   printf("tau=%d, ", b->tau); */
	for (i=0; i<dim; i++) {
		for (sigma=0; sigma<2; sigma++) {
			/*       printf("sigma=%d, ", sigma); */
			ngb = BoxNeighbour(b, i, sigma);
			/*       if (ngb) printf("ngb=%d, ", ngb->no); */
			/*       if (ngb) printf("ngb->tau=%d, ", ngb->tau); */
			/*       if (ngb) printf("ngb->children=%d, ", BoxNoOfChildren(ngb)); */
			BoxExpandNeighbours(ngb, i, sigma, s);
		}
	}
	return s;
}

void BoxExpandNeighbours(Box *ngb, byte i, byte sigma, Stack *s) {
	byte tau;
	assert(s);
	if (!ngb) return;
	if (BoxNoOfChildren(ngb)==0) StackPush(s, ngb);
	if (ngb->sd==i) {
		BoxExpandNeighbours(ngb->child[(sigma + 1) % 2], i, sigma, s);
	}
	else
		for (tau=0; tau<2; tau++)
			BoxExpandNeighbours(ngb->child[tau], i, sigma, s);
}

#endif

int BoxCollapse(Box *box) {
  int collapsed = 0;

  if (box->child[0] == NULL && box->child[1] == NULL) {
    return 1;
  }

  if (box->child[0]) {
    collapsed += BoxCollapse(box->child[0]);
  }
  if (box->child[1]) {
    collapsed += BoxCollapse(box->child[1]);
  }

  if (collapsed == 2) {
    BoxFree(&(box->child[0]));
    BoxFree(&(box->child[1]));
    return 1;
  }

  return 0;
}

