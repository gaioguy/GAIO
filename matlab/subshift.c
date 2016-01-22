#include <mex.h>
#include <stdio.h>
#include <stdlib.h>
#include <gaio/SparseMatrix.h>
#include <gaio/IndSubshift.h>

static SparseMatrix *mxSparse_to_SparseMatrix(const mxArray *A) {
    
  SparseMatrix *SA;
  double  *pr;
  mwIndex  *ir, *jc, row_start, row_stop, row;
  mwSize   col, total=0, n;
  
  n = mxGetN(A);
  SA = SparseMatrixNew(n);

  pr = mxGetPr(A); ir = mxGetIr(A); jc = mxGetJc(A);
  
  for (col=0; col<n; col++)  { 
    row_start = jc[col]; row_stop = jc[col+1]; 
    if (row_start == row_stop) continue;
    else 
      for (row = row_start; row < row_stop; row++)  
       SparseMatrixAddEntry(SA, ir[row], col, pr[total++]);
    }  
  
  return SA;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  
    SparseMatrix *A, *HA;
    IndSubshift *s;
    unsigned char *letters;
    double *pr;
    int i, start;
    
    if (nrhs < 3) mexErrMsgTxt("subshift: two arguments expected.");
    if (!mxIsSparse(prhs[0])) mexErrMsgTxt("subshift: first parameter must be sparse.");
    A = mxSparse_to_SparseMatrix(prhs[0]);
    letters = mxGetData(prhs[1]);
    pr = mxGetData(prhs[2]);
    start = (int)pr[0];

/*    mexPrintf("start = %d\n",start);
    mexPrintf("A->dim = %d\n",A->dim); */
    s = IndSubshiftNew(A, letters);
    HA = IndSubshiftGraph(s, start);
/*    SparseMatrixInfo(stdout,HA);   */
    plhs[0] = mxCreateDoubleMatrix(HA->nz,1,mxREAL);
    pr = mxGetPr(plhs[0]);
    for (i=0; i<HA->nz; i++) pr[i] = HA->rows[i]+1;
    plhs[1] = mxCreateDoubleMatrix(HA->nz,1,mxREAL);
    pr = mxGetPr(plhs[1]);
    for (i=0; i<HA->nz; i++) pr[i] = HA->cols[i]+1;
    plhs[2] = mxCreateDoubleMatrix(HA->nz,1,mxREAL);
    pr = mxGetPr(plhs[2]);
    for (i=0; i<HA->nz; i++) pr[i] = HA->entries[i];    
 
    IndSubshiftFree(&s);
    SparseMatrixFree(&HA);
    
    return;
}

