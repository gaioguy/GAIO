/*
 * Copyright (c) 1999 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 * 
 * Rectangle.c
 * 
 */

#include <gaio/Rectangle.h>
#include <gaio/Vec.h>
#include <math.h>

Rectangle *RectangleNew(double *c, double *r, unsigned char dim) {
  Rectangle *rec;
  NEW1(rec, Rectangle*, 1);
  rec->dim = dim;
  NEW1(rec->c, double*, dim);
  NEW1(rec->r, double*, dim);
  VecCopy(c, rec->c, dim);
  VecCopy(r, rec->r, dim);

  return rec;
}

void RectangleFree(Rectangle **rec) {
  if (*rec) {
    FREE((*rec)->r);
    FREE((*rec)->c);
    FREE(*rec);
    *rec = 0;
  }
}

Rectangle *RectangleHalf(Rectangle *rect, unsigned char i, int which) {
  Rectangle *half;
  half = RectangleNew(rect->c, rect->r, rect->dim);
  half->r[i] /= 2;
  if (which==0)
    half->c[i] -= half->r[i];
  else
    half->c[i] += half->r[i];

  return half;
}

double RectangleVolume(Rectangle *rect) {
  unsigned char i;
  double v = 1;
  for (i=0; i<rect->dim; i++) v *= 2*rect->r[i];
  
  return v;
}

int RectangleContains(Rectangle *rect, double *x) {
  int i, in = 1;
  for (i=0; in && i<rect->dim; i++) 
    in = (fabs(x[i] - rect->c[i]) <= rect->r[i]);
  return in;
}

void RectangleSave(FILE *out, Rectangle *r) {
  fprintf(out, "Rectangle at %p:\n", r);
  if (r) {  
    fprintf(out, "  dim = %d\n", r->dim);
    fprintf(out, "  center = "); VecPrint(out, r->c, r->dim);
    fprintf(out, "  radius = "); VecPrint(out, r->r, r->dim);
  }
}
  
Rectangle *RectangleLoad(FILE *in) {
  int dim;
  double *c, *r;
  Rectangle *rect;

  fscanf(in, "Rectangle at %p:\n", &rect);
  if (rect) {
    fscanf(in, "  dim = %d\n", &dim);
    fscanf(in, "  center = "); c = VecRead(in, dim);
    fscanf(in, "  radius = "); r = VecRead(in, dim);
    rect = RectangleNew(c, r, dim);
    FREE(c);
    FREE(r);
  }
  return rect;
}

void RectangleInfo(FILE *out, Rectangle *r) {
  fprintf(out, "Rectangle at %p:\n", r);
  if (r) {
    fprintf(out, "  dim = %d\n", r->dim);
    fprintf(out, "  center = "); VecPrint(out, r->c, r->dim);
    fprintf(out, "  radius = "); VecPrint(out, r->r, r->dim);
  }
}
    
