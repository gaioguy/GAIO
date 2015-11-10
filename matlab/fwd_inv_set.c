#include <math.h>
#include <mex.h>

double *D, *I;

struct GRAPH {
	int n;
	mwIndex *ep;
	mwIndex *edge;
	double *ew;
} graph;

int fis_rec(int n) {
  int j;

  if (I[n] > 0) return 1;
  if (D[n] > 0) return 0;

  I[n] = 1;
  for (j=(graph.ep)[n]; j<(graph.ep)[n+1]; j++) 
    if (fis_rec((graph.edge)[j])) return 1;

  I[n] = 0;
  D[n] = 1;

  return 0;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  
  int n;
  
  if (!mxIsSparse(prhs[0]))
    mexErrMsgTxt("first parameter must be sparse.");

  graph.n = mxGetM(prhs[0]);
  graph.ep = mxGetJc(prhs[0]);
  graph.edge = mxGetIr(prhs[0]);

  plhs[0] = mxCreateDoubleMatrix(1, graph.n, mxREAL);
  I = mxGetPr(plhs[0]);
  D = (double *)mxCalloc((unsigned)(graph.n),sizeof(double));

  for (n=0; n<graph.n; n++) fis_rec(n);

  mxFree(D);
  
  return;
}
