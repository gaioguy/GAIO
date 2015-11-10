#include <math.h>
#include <mex.h>
#include "Bias2.h"
#include "types.h"
#include <IntervalVector.h>

double a = 1.4, b = 0.3;

void rhs(int dim, INTERVAL_VECTOR& x, INTERVAL_VECTOR& y) {
  y(1) = a - x(1)*x(1) + b*x(2);
  y(2) = x(1);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  BIASINTERVAL *pR;
  BIASINTERVAL *pA;
  double *pa;
  double *pr;
  unsigned int Arows, Acols;
  int i, j, Aclass;

  BiasInit();

  Arows = mxGetM(prhs[0])/2;
  if (Arows!=2) mexErrMsgTxt("input interval matrix must be 2 x n.");
  Acols = mxGetN(prhs[0]);

  pA = (BIASINTERVAL *) mxGetPr(prhs[0]);

  //for (i=0; i<Arows*Acols; i++)
  //     printf("a= [%lg, %lg]\n", (*(pA + i)).inf, (*(pA+i)).sup);

  plhs[0] = mxCreateDoubleMatrix(Arows * 2, Acols, mxREAL);
  pR = (BIASINTERVAL *) mxGetPr(plhs[0]);

  for (i=0; i<Acols; i++) {
    INTERVAL_VECTOR v(Arows, pA + i*Arows);
    INTERVAL_VECTOR w(Arows, pR + i*Arows);
    rhs(Arows, v, w);
  }

  return;
}
