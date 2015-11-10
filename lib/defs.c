/*
 * Copyright (c) 2003-2009 djs² GmbH
 *
 * defs.c
 *
 */

#include <gaio/defs.h>
#include <stdio.h>
#include <string.h>

void fputns(int c, int n, FILE *stream) {
  int i;
  for (i=0; i<n; i++) fputc(c, stream);
}

long fsize(FILE *f) {
  long size;

  fseek(f, 0L, SEEK_END);
  size = ftell(f);
  rewind(f);

  return size;
}

long fnewline(FILE *f) {
  int c;
  long i = 0;

  rewind(f);
  do {
    c = fgetc(f);
    i++;
  } while (c!=EOF && ((char)c)!='\n');

  rewind(f);

  return i;
}

int strcnttok(char *s, const char *delim) {
  int i = 0;
  char *tok;

  tok = strtok(s, delim);
  while (tok) {
    i++;
    tok = strtok(0, delim);
  }

  return i;
}

int sumchars(char *s, int length) {
  int i, sum = 0;

  for (i=0; i<length; i++) sum += s[i];

  return sum;
}

