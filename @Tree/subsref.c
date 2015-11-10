/*
 * Copyright 2007-2012 djs2
 */  

#include <mex.h>
#include <gaio/mxHandling.h>
#include <gaio/Box.h>
#include <gaio/Iter.h>
#include <gaio/Rectangle.h>
#include <gaio/Tree.h>
#include <gaio/Vec.h>
#include <stdio.h>

void mexFunction(
	int nlhs,       mxArray *plhs[],
	int nrhs, const mxArray *prhs[]
	)
{
	char *type, *attr;
	mxArray *ptr;
	double *pr, *pl;
	int k, dim;
	Iter *iter;

	ptr = mxGetField(prhs[0], 0, "handle");
    iter = (Iter *)ptrFromMxArray(ptr);
	if (iter==NULL) mexErrMsgTxt("Tree: tree is empty.");
    
	dim = iter->tree->Q->dim;

	ptr = mxGetField(prhs[1], 0, "type");
	type = mxArrayToString(ptr);

	ptr = mxGetField(prhs[1], 0, "subs");
	attr = mxArrayToString(ptr);

	/* attributes */
	if (mxGetNumberOfElements(prhs[1])==1 && !strcmp(type,".")) {

		if (!strcmp(attr, "dim")) {
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = dim;
		}
		else if (!strcmp(attr, "radius")) {
			plhs[0] = mxCreateDoubleMatrix(1, dim, mxREAL);
			pr = mxGetPr(plhs[0]);
			for (k=0; k < dim; k++) pr[k] = iter->tree->Q->r[k];
		}
		else if (!strcmp(attr, "center")) {
			plhs[0] = mxCreateDoubleMatrix(1, dim, mxREAL);
			pr = mxGetPr(plhs[0]);
			for (k=0; k <dim; k++) pr[k] = iter->tree->Q->c[k];
		}
		else if (!strcmp(attr, "info")) {
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			IterInfo(stdout, iter);
			TreeInfo(stdout, iter->tree);
		}
		else if (!strcmp(attr, "verbose")) {
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = iter->verbose;
		}
		else if (!strcmp(attr, "depth")) {
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeDepth(iter->tree->root);
		}
		else if (!strcmp(attr, "subdivide")) {
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeSubdivide(iter->tree, SUBDIVIDE);
		}
		else if (!strcmp(attr, "unsubdivide")) {
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeUnsubdivide(iter->tree, SUBDIVIDE);
		}
		else if (!strcmp(attr, "unsubdivide_current_box")) {
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = BoxUnsubdivide(iter->tree->box);
		}
		else if (!strcmp(attr, "sd")) {
			int i;
			plhs[0] = mxCreateDoubleMatrix(DEPTH_MAX(dim)+1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			for (i=0; i<DEPTH_MAX(dim)+1; i++) pr[i] = iter->tree->sd[i];
		}
		else if (!strcmp(attr, "collapse")) {             /* collapse */
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeCollapse(iter->tree);
		}
		else {
			mexErrMsgTxt("Tree: unknown field.");
		}
	}

	/* methods */
	if (mxGetNumberOfElements(prhs[1])==2 && !strcmp(type,".")) {
		int nargin, dim0, dim1 = 0;
		mxArray *cell0, *cell1 = 0;

		ptr = mxGetField(prhs[1], 1, "subs");
		nargin = mxGetNumberOfElements(ptr);
                if (nargin == 0) {
                  char errMsg[128];
                  sprintf(errMsg, "Tree.%.100s: missing arguments", attr);
                  mexErrMsgTxt(errMsg);
                }

		cell0 = mxGetCell(ptr, 0);
		dim0 = mxGetNumberOfElements(cell0);
		if (nargin > 1) {
			cell1 = mxGetCell(ptr, 1);
			dim1 = mxGetNumberOfElements(cell1);
		}

		if (!strcmp(attr, "color")) {               /* color */
			char *colors;
			int depth = -1;
			colors = mxArrayToString(cell0);
			if (nargin > 1) depth = (int)*mxGetPr(mxGetCell(ptr, 1));
			TreeColor(iter->searchTree, colors, depth);
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
		}
		else if (!strcmp(attr, "change_Q")) {        /* change_Q */
			int m_c, m_r;
			double *pr_c, *pr_r;
			Rectangle *Q;
			if (nargin < 2)
				mexErrMsgTxt("Tree.change_Q: radius is missing.");
			pr_c = mxGetPr(cell0);
			m_c = mxGetNumberOfElements(cell0);
			pr_r = mxGetPr(cell1);
			m_r = mxGetNumberOfElements(cell1);
			VecPrint(stdout, pr_c, m_c);
			VecPrint(stdout, pr_r, m_r);
			if (m_c != m_r)
				mexErrMsgTxt("Tree.change_Q: dim(center)!=dim(radius).");
			Q = RectangleNew(pr_c, pr_r, m_c);
			TreeFree(&(iter->searchTree));
			TreeChangeQ(iter->tree, Q);
			iter->searchTree = TreeCopy(iter->tree);
			plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
		}
		else if (!strcmp(attr, "count")) {        /* count */
			if (dim0 > 1) mexErrMsgTxt("Tree.count: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeCountDepth(iter->searchTree, (int)*mxGetPr(cell0));
		}
		else if (!strcmp(attr, "delete")) {       /* delete */
			if (dim0 > 1) mexErrMsgTxt("Tree.delete: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
			pr = mxGetPr(plhs[0]);
			TreeDeleteDepth(iter->tree, (int)*mxGetPr(cell0));
		}
		else if (!strcmp(attr, "first_box")) {       /* first_box */
			if (dim0 > 1) mexErrMsgTxt("Tree.first_box: scalar expected.");
			if (TreeFirstBox(iter->tree, (int)*mxGetPr(cell0))) {
				plhs[0] = mxCreateDoubleMatrix(2*dim+2, 1, mxREAL);
				VecCopy(iter->tree->c, mxGetPr(plhs[0]), dim);
				VecCopy(iter->tree->r, mxGetPr(plhs[0]) + dim, dim);
				*(mxGetPr(plhs[0]) + 2*dim) = (double)iter->tree->box->flags;
#ifdef _MEASURE_
				*(mxGetPr(plhs[0]) + 2*dim + 1) = (double)iter->tree->box->no + 1;
#endif
			}
			else {
				plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
			}
		}
		else if (!strcmp(attr, "next_box")) {       /* next_box */
			if (dim0 > 1) mexErrMsgTxt("Tree.next_box: scalar expected.");
			if (TreeNextBox(iter->tree, (int)*mxGetPr(cell0))) {
				plhs[0] = mxCreateDoubleMatrix(2*dim+2, 1, mxREAL);
				VecCopy(iter->tree->c, mxGetPr(plhs[0]), dim);
				VecCopy(iter->tree->r, mxGetPr(plhs[0]) + dim, dim);
				*(mxGetPr(plhs[0]) + 2*dim) = (double)iter->tree->box->flags;
#ifdef _MEASURE_
				*(mxGetPr(plhs[0]) + 2*dim + 1) = (double)iter->tree->box->no + 1;
#endif
			}
			else {
				plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
			}
		}
		else if (!strcmp(attr, "insert")) {       /* insert */

			double *color;
			int i, m, n, flag_inserted = INSERTED, flag_found = NONE;
			if (nargin < 2)
				mexErrMsgTxt("usage: Tree.insert(x, depth).");
			m = mxGetM(cell0);
			if (m != dim)
				mexErrMsgTxt("Tree.insert: matrix has wrong dimension.");
			n = mxGetN(cell0);
			plhs[0] = mxCreateDoubleMatrix(n, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			if (nargin > 2)
				flag_inserted = (int)*mxGetPr(mxGetCell(ptr, 2));
			if (nargin > 3)
				flag_found = (int)*mxGetPr(mxGetCell(ptr, 3));
			if (nargin > 4) {
				color = mxGetPr(mxGetCell(ptr, 4));
				for (i=0; i<n; i++)
					pr[i] = TreeInsert(iter->searchTree,
						mxGetPr(cell0) + i*dim, (int)*mxGetPr(cell1),
						flag_inserted, flag_found, (byte)*(color + i));
			}
			else
			    for (i=0; i<n; i++)
				pr[i] = TreeInsert(iter->searchTree,
					mxGetPr(cell0) + i*dim, (int)*mxGetPr(cell1),
					flag_inserted, flag_found, 1);
		}
		else if (!strcmp(attr, "insert_box")) {          /* insert_box */
			double *pr1;
			byte color = 1, flag_inserted = INSERTED, flag_found = NONE;
			int inserted, n0, m0, n1, m1, depth = -1;
                        if (nargin < 2) {
                                mexErrMsgTxt("usage: Tree.insert_box(center, radius).");
                        }
			m0 = mxGetM(cell0);
			n0 = mxGetN(cell0);
			if (m0!=dim || n0!=1)
				mexErrMsgTxt("Tree.insert_box: center has wrong dimension.");
			pr = mxGetPr(cell0);
			m1 = mxGetM(cell1);
			n1 = mxGetN(cell1);
			if (m1!=dim || n1!=1)
				mexErrMsgTxt("Tree.insert_box: radius has wrong dimension.");
			pr1 = mxGetPr(cell1);
			if (nargin > 2) depth = (int)*mxGetPr(mxGetCell(ptr, 2));
			if (nargin > 3) flag_inserted = (byte)*mxGetPr(mxGetCell(ptr, 3));
			if (nargin > 4) flag_found = (byte)*mxGetPr(mxGetCell(ptr, 4));
			if (nargin > 5) color = (byte)*mxGetPr(mxGetCell(ptr, 5));
			/*       VecPrint(stdout, pr, dim); */
			/*       VecPrint(stdout, pr1, dim); */
			/*       printf("depth = %d\n", depth); */

			inserted = TreeInsertBox(iter->searchTree, pr, pr1, depth,
				flag_inserted, flag_found, color);

			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pl = mxGetPr(plhs[0]);
			pl[0] = inserted;
		}
		else if (!strcmp(attr, "boxes")) {        /* boxes */
			int dims[2];
			if (dim0 > 1) mexErrMsgTxt("Tree.boxes: scalar expected.");
			dims[0] = TreeCountDepth(iter->searchTree, (int)*mxGetPr(cell0));
			dims[1] = 2*iter->tree->Q->dim + 2;
			plhs[0] = mxCreateDoubleMatrix(dims[1],dims[0],mxREAL);
			pr = mxGetPr(plhs[0]);
			TreePrintToMatrix(iter->searchTree, (int)*mxGetPr(cell0), pr);
		}
		else if (!strcmp(attr, "get_flags")) {    /* get_flags */
			char *choice;
			int depth = -1;
			if (nargin < 2)
				mexErrMsgTxt("Tree.get_flags: two arguments required.");
			depth = (int)*mxGetPr(cell1);
			if (mxIsChar(cell0)) {
				int no = TreeCountDepth(iter->tree, depth);
				mwSize dims[2];
				dims[0] = no;
				dims[1] = 1;
				plhs[0] = mxCreateNumericArray(2, dims, mxUINT8_CLASS, mxREAL);
				choice = mxArrayToString(cell0);
				if (strcmp(choice,"all"))
					mexErrMsgTxt("Tree.get_flags: either 'all' or a point expected.");
				TreeGetFlags(iter->tree, depth, (byte*)mxGetData(plhs[0]));
				mxFree(choice);
			}
			else {
				byte *ptr;
				mwSize dims[1];
				dims[0] = 1;
				if (dim0 != dim)
					mexErrMsgTxt("Tree.get_flags: point has wrong dimension.");
				plhs[0] = mxCreateNumericArray(1, dims, mxUINT8_CLASS, mxREAL);
				ptr = (byte*)mxGetData(plhs[0]);
				if (TreeSearch(iter->tree, mxGetPr(cell0), depth) > 0) {
					ptr[0] = BoxGetFlags(iter->tree->box);
				}
			}
		}
		else if (!strcmp(attr, "set_flags")) {    /* set_flags */

			char *choice;
			byte flag;
			int depth = -1;
			if (nargin < 2)
				mexErrMsgTxt("Tree.set_flags: at least two arguments expected.");
			flag = (byte)*mxGetPr(cell1);
			if (nargin > 2) depth = (int)*mxGetPr(mxGetCell(ptr, 2));
			if (mxIsChar(cell0)) {
				choice = mxArrayToString(cell0);
				TreeSetFlags(iter->tree, depth, choice, flag);
				mxFree(choice);
				plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
			}
			else {
				double *ptr;
				int i, m, n, found = 0;
				m = mxGetM(cell0);
				if (m!=dim)
					mexErrMsgTxt("Tree.set_flags: input matrix has wrong dimension.");
				n = mxGetN(cell0);
				pr = mxGetPr(cell0);
				for (i=0; i<n; i++)
					if (TreeSearch(iter->searchTree, pr + i*dim, depth) > 0) {
						BoxSetFlag(iter->searchTree->box, flag);
						found = 1;
					}
				plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
				ptr = mxGetPr(plhs[0]);
				ptr[0] = found;
			}
		}
		else if (!strcmp(attr, "unset_flags")) {    /* unset_flags */
			char *choice;
			byte flag;
			int depth = -1;
			if (nargin < 2)
				mexErrMsgTxt("Tree.unset_flags: at least two arguments expected.");
			flag = (byte)*mxGetPr(cell1);
			if (nargin > 2) depth = (int)*mxGetPr(mxGetCell(ptr, 2));
			if (mxIsChar(cell0)) {
				choice = mxArrayToString(cell0);
				TreeUnsetFlags(iter->tree, depth, choice, flag);
				mxFree(choice);
			}
			else {
				int i, m, n;
				m = mxGetM(cell0);
				if (m!=dim)
					mexErrMsgTxt("Tree.unset_flags: input matrix has wrong dimension.");
				n = mxGetN(cell0);
				pr = mxGetPr(cell0);
				for (i=0; i<n; i++)
					if (TreeSearch(iter->searchTree, pr + i*dim, depth) > 0)
						BoxUnsetFlag(iter->searchTree->box, flag);
			}
			plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
		}
		else if (!strcmp(attr, "change_flags")) {    /* change_flags */
			char *choice;
			byte from, to;
			int depth = -1;
			if (nargin < 3)
				mexErrMsgTxt("Tree.change_flags: at least three arguments expected.");
			from = (byte)*mxGetPr(cell1);
			to = (byte)*mxGetPr(mxGetCell(ptr, 2));
			if (nargin > 3) depth = (int)*mxGetPr(mxGetCell(ptr, 3));
			if (mxIsChar(cell0)) {
				choice = mxArrayToString(cell0);
				TreeChangeFlags(iter->tree, depth, choice, from, to);
				mxFree(choice);
			}
			else {
				int i, m, n;
				m = mxGetM(cell0);
				if (m!=dim)
					mexErrMsgTxt("Tree.change_flags: input matrix has wrong dimension.");
				n = mxGetN(cell0);
				pr = mxGetPr(cell0);
				for (i=0; i<n; i++)
					if (TreeSearch(iter->searchTree, pr + i*dim, depth) > 0)
						BoxChangeFlag(iter->searchTree->box, from, to);
			}
			plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
		}
#ifdef _NORMALS_
		else if (!strcmp(attr, "normals")) {         /* normals */
			if (nargin<2)
				mexErrMsgTxt("Tree.normals: two input arguments expected.");
			if (dim0 != dim)
				mexErrMsgTxt("Tree.normals: radius has wrong dimension.");
			if (dim1 > 1)
				mexErrMsgTxt("Tree.normals: scalar as second arg expected.");
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			IterNormals(iter, (int)*mxGetPr(cell1), mxGetPr(cell0));
		}
#endif
		else if (!strcmp(attr, "remove")) {          /* remove */
			if (dim0 > 1) mexErrMsgTxt("Tree.remove: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeRemove(iter->tree->root, (byte)*mxGetPr(cell0));
		}
		else if (!strcmp(attr, "search")) {          /* search */
		  int i, n, m, depth = -1, found;
		  double *pl1 = NULL;
		  m = mxGetM(cell0);
		  if (m!=dim)
		    mexErrMsgTxt("Tree.search: input matrix has wrong dimension.");
		  n = mxGetN(cell0);
		  pr = mxGetPr(cell0);
		  plhs[0] = mxCreateDoubleMatrix(n, 1, mxREAL);
		  pl = mxGetPr(plhs[0]);
		  if (nlhs > 1) {
#ifdef __x86_64
                    mexWarnMsgTxt("Tree.search: second output argument ignored on this architecture.");
                    pl1 = NULL;
#else
		    mwSize dims[2];
		    dims[0] = n;
		    dims[1] = 1;
		    plhs[1] = mxCreateNumericArray(2, dims, mxUINT32_CLASS, mxREAL);
		    pl1 = mxGetPr(plhs[1]);
#endif
		  }
		  if (nargin > 1) depth = (int)*mxGetPr(mxGetCell(ptr, 1));
		  for (i=0; i < n; i++) {
		    found = TreeSearch(iter->searchTree, pr + i*dim, depth);
#ifdef _MEASURE_
		    pl[i] = (found > 0 ? iter->searchTree->box->no + 1: -1);
#else
		    pl[i] = found;
#endif
#ifndef __x86_64
		    if (nlhs > 1)
		      pl1[i] = (unsigned int)iter->searchTree->box; /* this does not work on x86_64! */
#endif
		  }
		}
#ifdef _MEASURE_
		else if (!strcmp(attr, "search_box")) {          /* search_box */
			double *pr1;
			SparseVector *nos;
			int i, n0, m0, n1, m1, depth = -1;
                        if (nargin<2) {
                                mexErrMsgTxt("usage: Tree.search_box(center, radius).");
                        }
			m0 = mxGetM(cell0);
			n0 = mxGetN(cell0);
			if (m0!=dim || n0!=1)
				mexErrMsgTxt("Tree.search_box: center has wrong dimension.");
			pr = mxGetPr(cell0);
			m1 = mxGetM(cell1);
			n1 = mxGetN(cell1);
			if (m1!=dim || n1!=1)
				mexErrMsgTxt("Tree.search_box: radius has wrong dimension.");
			pr1 = mxGetPr(cell1);

			if (nargin > 2) depth = (int)*mxGetPr(mxGetCell(ptr, 2));
			/*       VecPrint(stdout, pr, dim); */
			/*       VecPrint(stdout, pr1, dim); */
			/*       printf("depth = %d\n", depth); */

			nos = TreeSearchBox(iter->searchTree, pr, pr1, depth);

			/*       SparseVectorInfo(stdout, nos); */
			plhs[0] = mxCreateDoubleMatrix(nos->nz, 1, mxREAL);
			pl = mxGetPr(plhs[0]);
			for (i=0; i<nos->nz; i++) pl[i] = nos->rows[i]+1;
			SparseVectorFree(&nos);

		}
#endif
		else if (!strcmp(attr, "save")) {            /* save */
			FILE *out;
			char *filename;
			if (!mxIsChar(cell0)) mexErrMsgTxt("Tree.save: file name expected.");
			filename = mxArrayToString(cell0);
			out = fopen(filename, "w");
			if (!out) mexErrMsgTxt("Tree.save: could not open file for writing.");
			IterSave(out, iter);
			fclose(out);
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			mxFree(filename);
		}
		else if (!strcmp(attr, "subdivide")) {       /* subdivide */
			if (dim0 > 1) mexErrMsgTxt("Tree.subdivide: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeSubdivide(iter->tree, (byte)*mxGetPr(cell0));
		}
		else if (!strcmp(attr, "subdivide_current_box")) {
			if (dim0 > 1) mexErrMsgTxt("Tree.subdivide: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = BoxSubdivide(iter->tree->box, (byte)*mxGetPr(cell0));
		}
		else if (!strcmp(attr, "unsubdivide")) {     /* unsubdivide */
			if (dim0 > 1) mexErrMsgTxt("Tree.unsubdivide: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeUnsubdivide(iter->tree, (byte)*mxGetPr(cell0));
		}
#ifdef _MEASURE_
		else if (!strcmp(attr, "volume")) {           /* volume */
			int n, depth = (int)*mxGetPr(cell0);
			if (dim0 > 1) mexErrMsgTxt("Tree.volume: scalar expected.");
			n = TreeCountDepth(iter->tree, depth);
			plhs[0] = mxCreateDoubleMatrix(n, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			TreeVolume(iter->tree, pr, depth);
		}
#endif
		else if (!strcmp(attr, "uncollapse")) {       /* uncollapse */
			if (dim0 > 1) mexErrMsgTxt("Tree.uncollapse: scalar expected.");
			plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
			pr = mxGetPr(plhs[0]);
			pr[0] = TreeUncollapse(iter->tree, (int)*mxGetPr(cell0));
		}
		else {
			mexErrMsgTxt("Tree: unknown method.");
		}
	}
	mxFree(attr);
	mxFree(type);

	return;
}

