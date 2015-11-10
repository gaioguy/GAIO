/*
 * Copyright (c) 2007-2009 djs² GmbH
 */

#include <mex.h>
#include <gaio/mxHandling.h>
#include <gaio/Iter.h>
#include <string.h>

void mexFunction(
                 int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]
		 )
{
  char *type, *attr;
  Iter *iter;
  mxArray *ptr;
  double *r, *c;
  unsigned int dim, k;
  
  ptr = mxGetField(prhs[0], 0, "handle");
  iter = (Iter *)ptrFromMxArray(ptr);
  if (iter==NULL) mexErrMsgTxt("Tree: tree is empty.");
  dim = (unsigned int)(iter->tree->Q->dim);

  ptr = mxGetField(prhs[1], 0, "type");
  type = mxArrayToString(ptr);
  
  if (!strcmp(type,".")) {
    ptr = mxGetField(prhs[1], 0, "subs");
    attr = mxArrayToString(ptr);

    if (!strcmp(attr, "radius")) {
      if (mxGetNumberOfElements(prhs[2]) == dim) {
        r = mxGetPr(prhs[2]);
        for (k=0; k<dim; k++) {
/* 	printf("%f\n", r[k]); */
          iter->tree->Q->r[k] = r[k];
        }
      } else {
	mexErrMsgTxt("Tree.radius: right hand side has wrong dimension");
      }
    } else if (!strcmp(attr, "center")) {
      if (mxGetNumberOfElements(prhs[2]) == dim) {
        c = mxGetPr(prhs[2]);
        for (k=0; k<dim; k++) iter->tree->Q->c[k] = c[k];
      } else {
	mexErrMsgTxt("Tree.center: right hand side has wrong dimension");
      }
    } else if (!strcmp(attr, "verbose")) {
      iter->verbose = (int)*mxGetPr(prhs[2]);
    } else if (!strcmp(attr, "sd")) {
      c = mxGetPr(prhs[2]);
      for (k=0; k<mxGetNumberOfElements(prhs[2]); k++) {
/* 	printf("%f\n", c[k]); */
        iter->tree->sd[k] = c[k];
      }
    }
    mxFree(attr);
  }

  mxFree(type);
  
  plhs[0] = (mxArray*) prhs[0];

  return;
}
