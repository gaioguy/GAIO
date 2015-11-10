/*
 * Copyright (c) 2007-2009 djs² GmbH
 *
 * mexHandling.h
 *
 * suggested by Christian Ludwig
 *
 */

#ifndef _mexHandling_h
#define _mexHandling_h

#include <mex.h>
#include <string.h>

static mxArray *ptrToMxArray(const void *ptr) {
  mxArray *mx;
  mwSize dims[2];
  void *mxMem;

  dims[0] = 1;
  dims[1] = sizeof(void*);
  mx = mxCreateCharArray(2,dims);
  mxAssert(mx!=NULL,"Out of memory");
  mxMem = mxGetData(mx);
  memcpy(mxMem,&ptr,sizeof(void*));

  return mx;
}

static void *ptrFromMxArray(const mxArray *mx) {
  void *ptr;
  mxAssert(mx!=NULL,"ptrFromMxArray: NULL pointer");

  if (!((mxIsChar(mx) && (mxGetNumberOfDimensions(mx)==2) &&
           (mxGetM(mx)==1) && (mxGetN(mx)==sizeof(void*))))) {
    mexErrMsgTxt("not a valid object pointer.");
  }
  memcpy(&ptr,mxGetData(mx),sizeof(void*));
  return ptr;
}

#endif
