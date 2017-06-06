#include <math.h>
#include <mex.h>

#define VERY_LARGE 100000

struct GRAPH {
	int n;
	mwIndex *ep;
	mwIndex *edge;
	double *ew;
	};

struct BHEAP {
        int n;
        int space;
        int *ele;
        int *pos;
        };

int bheapNew (struct BHEAP *bheap, int n)
{ int i;

  bheap->n = bheap->space = n;
 
  if(!(bheap->ele = (int*)mxCalloc((unsigned)n,sizeof(int))) ||
     !(bheap->pos = (int*)mxCalloc((unsigned)n,sizeof(int))) )
  { printf("ERROR...calloc!\n"); return 1; }
  for (i=0; i<n; i++)
    bheap->ele[i] = bheap->pos[i] = i;
  return 0;
}

void bheapFree (struct BHEAP bheap)
{ mxFree(bheap.ele);
  mxFree(bheap.pos);
}

int bheap_size (struct BHEAP *bheap)
{
  return bheap->n;
}
 
int bheap_min (struct BHEAP *bheap)
{
  return bheap->ele[0];
}   


void bheap_check (struct BHEAP bheap, double *d)
{ int i,left,right;

  for (i=0; i<bheap.n; i++)
  { left = 2*i+1;
    right = 2*i+2;
    if ((left<bheap.n && d[bheap.ele[left]]<d[bheap.ele[i]])  ||  (right<bheap.n && d[bheap.ele[right]]<d[bheap.ele[i]]))
    { printf("ERROR...no bheap property!\n"); exit(0); }
  }
}

void heapify (struct BHEAP bheap, int root, double *d)
{ int	left=root*2+1, right=root*2+2, smallest=root, tmp; 

  if ((left < bheap.n) && (d[bheap.ele[left]] < d[bheap.ele[smallest]]))
    smallest = left;
  if((right < bheap.n) && (d[bheap.ele[right]] < d[bheap.ele[smallest]]))
    smallest = right;
  if (smallest != root)
  { bheap.pos[bheap.ele[root]] = smallest;
    bheap.pos[bheap.ele[smallest]] = root;
    tmp = bheap.ele[root];
    bheap.ele[root] = bheap.ele[smallest];
    bheap.ele[smallest] = tmp;
    heapify(bheap,smallest,d);
  }
}

void bheap_decrease_key (struct BHEAP bheap, int element, double *d)
{ int tmp, position=bheap.pos[element], father=(position-1)/2;

  while (position>0 && d[element]<d[bheap.ele[father]])
  { bheap.pos[bheap.ele[position]] = father;
    bheap.pos[bheap.ele[father]] = position;
    tmp = bheap.ele[father];
    bheap.ele[father] = bheap.ele[position];
    bheap.ele[position] = tmp;
    position = father;
    father = (father-1)/2;
  } 
}

int bheap_extract_min (struct BHEAP *bheap, double *d)
{ int min = bheap->ele[0];
  bheap->ele[0] = bheap->ele[--(bheap->n)];
  bheap->pos[bheap->ele[0]] = 0;
  heapify(*bheap,0,d);
  return min;
}

int dijkstra(struct GRAPH graph, int sour, double *d, int *pre)
{ int		i, j;
  struct BHEAP	bheap;

  for (i=0; i<graph.n; i++)
    d[i] = VERY_LARGE;

  if (bheapNew (&bheap,graph.n))
  { printf("ERROR...bheapNew!\n"); return 1; }

  d[sour] = 0.0;
  bheap_decrease_key(bheap,sour,d);
  bheap_extract_min(&bheap,d);
  for (j=graph.ep[sour]; j<graph.ep[sour+1]; j++) {
    if (d[graph.edge[j]] > d[sour] + (graph.ew[j]))
      { d[graph.edge[j]] = d[sour] + (graph.ew[j]);
        pre[graph.edge[j]] = sour;
        bheap_decrease_key(bheap,graph.edge[j],d);
      }
  }

  do { 
    i = bheap_extract_min(&bheap,d);
    for (j=graph.ep[i]; j<graph.ep[i+1]; j++) {
       if (d[graph.edge[j]] > d[i] + (graph.ew[j])) { 
	d[graph.edge[j]] = d[i] + (graph.ew[j]);
        pre[graph.edge[j]] = i;
        bheap_decrease_key(bheap,graph.edge[j],d);
      }
    }
  } while (bheap_size(&bheap)>0 && d[bheap_min(&bheap)] < VERY_LARGE);

  bheapFree (bheap);
  return 0;
}


/* Das Array 'd' zeigt die Distanz von dem jeweilgen Knoten zum
   'dest'-Knoten an.
   Das Array 'next' zeigt fuer jeden Knoten an, zu welchem naechsten Knoten
   es weiter gehen muss um zu dem 'dest'-Knoten zu kommen.
*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  
  int i, e, dest, *next;
  struct GRAPH graph,fgraph;
  double *pr, *d;
  
  if (nrhs < 2)
    mexErrMsgTxt("usage: dijkstra_all(matrix, destination).");
  if (!mxIsSparse(prhs[0]))
    mexErrMsgTxt("first parameter must be sparse.");

  graph.n = mxGetM(prhs[0]);
  graph.ep = mxGetJc(prhs[0]);
  graph.edge = mxGetIr(prhs[0]);
  graph.ew = mxGetPr(prhs[0]);

  dest = (int)*mxGetPr(prhs[1]) - 1;
  if (dest < 0 || dest >= graph.n)
    mexErrMsgTxt("destination node out of range.");
/*   printf("dest = %d\n", dest); fflush(stdout); */
    
  if (!(d   = (double*)mxCalloc((unsigned)graph.n,sizeof(double))) ||
      !(next= (int*)   mxCalloc((unsigned)graph.n,sizeof(int)))     )
  { printf("ERROR...calloc!\n"); return; }

  if (dijkstra(graph, dest, d, next))
    mexErrMsgTxt("allocation error in New.");

  plhs[0] = mxCreateDoubleMatrix(1, graph.n, mxREAL);
  pr = mxGetPr(plhs[0]);
  for (i=0; i<graph.n; i++) pr[i] = ( d[i]==VERY_LARGE ? mxGetInf() : d[i]);
  if (nrhs > 1) {
    plhs[1] = mxCreateDoubleMatrix(1, graph.n, mxREAL);
    pr = mxGetPr(plhs[1]);
    for (i=0; i<graph.n; i++) pr[i] = next[i]+1;
  }
  
  mxFree(d);
  mxFree(next);

  return;
}
