/* 
 * Copyright (c) 2007-2009 djs² GmbH
 */

#include <mex.h>
#include <gaio/mxHandling.h>
#include <gaio/Iter.h>
#include <gaio/Rectangle.h>
#include <stdio.h>

void mexFunction(
                 int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]
				 )
{
	Rectangle *Q = 0;
	double *ptr, *ptr0, *ptr1;
	unsigned char dim;
	
	if (nrhs > 1) { 		/* new tree */
		if (nrhs == 2) {
			ptr0 = mxGetPr(prhs[0]);
			ptr1 = mxGetPr(prhs[1]);
			dim = mxGetNumberOfElements(prhs[0]);
		} else {
			ptr = mxGetPr(prhs[0]);
			dim = (unsigned char)*ptr;
			ptr0 = mxGetPr(prhs[1]);
			ptr1 = mxGetPr(prhs[2]);
		} 
		Q = RectangleNew(ptr0, ptr1, dim);
		plhs[0] = ptrToMxArray(IterNew(Q));
	} else {       		/* load tree from file */
		char *filename = mxArrayToString(prhs[0]);
		Iter *handle;
		FILE *in;
		in = fopen(filename, "r");
		if (in==NULL) {
                        mexErrMsgTxt("Tree: could not open file.");
		}

		handle = IterLoad(in);
		fclose(in);
		mxFree(filename);

		if (!handle) {
		  mexErrMsgTxt("failed to load tree.");
		}
		plhs[0] = ptrToMxArray(handle);
	}
return;
}


