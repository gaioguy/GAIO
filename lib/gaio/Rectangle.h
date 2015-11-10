/*
 * Copyright (c) 1999 Oliver Junge, 2003-2009 djs² GmbH, for details see COPYING
 *
 * Rectangle.h
 *
 */

#ifndef _Rectangle_h
#define _Rectangle_h

#include <stdio.h>

#ifdef WIN32
#define Rectangle GRectangle
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int dim;
  double *c, *r;
} Rectangle;

Rectangle *RectangleNew(double *c, double *r, unsigned char dim);
void RectangleFree(Rectangle **r);

/* returns a bisected rectangle, bisection coordinate i, which is
   indicating which half of rect is returned */
Rectangle *RectangleHalf(Rectangle *rect, unsigned char i, int which);

double RectangleVolume(Rectangle *rect);

/* checks whether x \in rect */
int RectangleContains(Rectangle *rect, double *x);

void RectangleSave(FILE *out, Rectangle *rect);
Rectangle *RectangleLoad(FILE *in);

void RectangleInfo(FILE *out, Rectangle *r);

#ifdef __cplusplus
}
#endif

#endif
