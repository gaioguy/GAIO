#include <mex.h>
#include <stdio.h>
#include <stdlib.h>
    
struct GRAPH {
	int n;
	mwIndex *ep;
	mwIndex *edge;
	double *ew;
};


static void visit (struct GRAPH graph, int vertex, int *taken, int *finished, int *n_finished)
{ int j;

  taken[vertex] = 1;
  
  for (j=graph.ep[vertex]; j<graph.ep[vertex+1]; j++)
    if (taken[graph.edge[j]] == 0)
      visit (graph,graph.edge[j],taken,finished,n_finished);
  finished[(*n_finished)++] = vertex;
}

static void visit_again (struct GRAPH graphT, int vertex, int *taken, int n_scc, int *scc)
{ int j;

  taken[vertex] = 0;
  scc[vertex] = n_scc;

  for (j=graphT.ep[vertex]; j<graphT.ep[vertex+1]; j++)
    if (taken[graphT.edge[j]] == 1)
      visit_again (graphT,graphT.edge[j],taken,n_scc,scc);
}

/* graph     der gerichtete Graph
   graphT    derselbe Graph nur mit umgekehrter Richtung der Kanten
   n_scc     Zeiger auf einen Integer, der die Anzahl der SCC angibt
   scc       Array mit SCC-nummer fuer jeden Knoten
*/
static int SCC (struct GRAPH graph, struct GRAPH graphT, int *n_scc, int *scc)
{ int i, j, *taken, *finished, n_finished=0;

  if (!(taken    = (int*) mxCalloc((unsigned)graph.n,sizeof(int)))   ||
      !(finished = (int*) mxCalloc((unsigned)graph.n,sizeof(int)))  )
  { printf("ERROR...calloc!\n"); return 1; }

  for (i=0; i<graph.n; i++)
    taken[i] = 0;

  for (i=0; i<graph.n; i++)
    if (taken[i] == 0)
      visit (graph,i,taken,finished,&n_finished);

  if (n_finished != graph.n)
  { printf("%d und %d muessten eigentlich gleich sein...\n",n_finished,graph.n);
    exit(0);
  }

  (*n_scc) = 0;
  for (i=n_finished-1; i>=0; i--)
    if (taken[finished[i]] == 1)
    { int new_scc=0;
      for (j=graphT.ep[finished[i]]; j<graphT.ep[finished[i]+1]; j++)
        if (taken[graphT.edge[j]] == 1)
        { visit_again (graphT,graphT.edge[j],taken, *n_scc,scc);
          new_scc = 1; 
        }
      taken[finished[i]] = 0;
      if (new_scc)
      { scc[finished[i]] = *n_scc;
        (*n_scc)++;
      }
      else
        scc[finished[i]] = -1;
    }

  mxFree(taken);
  mxFree(finished);
  return 0;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  
  int n_scc;
  mwSize dims[2];
  struct GRAPH graph, graphT;
  int *pr;

  if (nrhs != 2)
    mexErrMsgTxt("usage: scc(A, A^T).");
  if (!mxIsSparse(prhs[0]))
    mexErrMsgTxt("first parameter must be sparse.");
  if (!mxIsSparse(prhs[1]))
    mexErrMsgTxt("first parameter must be sparse.");

  graph.n = mxGetM(prhs[0]);
  graph.ep = mxGetJc(prhs[0]);
  graph.edge = mxGetIr(prhs[0]);
  graph.ew = mxGetPr(prhs[0]);

  graphT.n = mxGetM(prhs[1]);
  graphT.ep = mxGetJc(prhs[1]);
  graphT.edge = mxGetIr(prhs[1]);
  graphT.ew = mxGetPr(prhs[1]);

  dims[0] = 1;
  dims[1] = graph.n;
  plhs[0] = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
  pr = (int *)mxGetData(plhs[0]);

  if (SCC (graph, graphT, &n_scc, pr))
    mexErrMsgTxt("allocation error in SCC.");

  return;
}

