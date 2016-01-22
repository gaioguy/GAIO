/*
 * Copyright (c) 2007-2013 djs2
 */

#include <mex.h>
#include <gaio/mxHandling.h>
#include <gaio/Iter.h>

void mexFunction(
                 int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]
				)
{
  mxArray *null;
  Iter *iter = (Iter *)ptrFromMxArray(mxGetField(prhs[0], 0, "handle"));
  IterFree(&iter);
  null = mxCreateDoubleMatrix(1,1,mxREAL);
  *mxGetPr(null) = 0;
  mxSetField((mxArray *)prhs[0], 0, "handle", null);
}


