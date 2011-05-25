#ifndef __glutstroke_h__
#define __glutstroke_h__

/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#if defined(_WIN32)
#pragma warning (disable:4244)  /* disable bogus conversion warnings */
#pragma warning (disable:4305)  /* VC++ 5.0 version of above warning. */
#endif

typedef struct {
  float x;
  float y;
} CoordRec, *CoordPtr;

typedef struct {
  int num_coords;
  const CoordRec *coord;
} StrokeRec, *StrokePtr;

typedef struct {
  int num_strokes;
  const StrokeRec *stroke;
  float center;
  float right;
} StrokeCharRec, *StrokeCharPtr;

typedef struct {
  const char *name;
  int num_chars;
  const StrokeCharRec *ch;
  float top;
  float bottom;
} StrokeFontRec, *StrokeFontPtr;

typedef void *GLUTstrokeFont;

#endif /* __glutstroke_h__ */
