
/*
 * vvector.h
 *
 * FUNCTION:
 * This file contains a number of utilities useful for handling
 * 3D vectors
 * 
 * HISTORY:
 * Written by Linas Vepstas, August 1991
 * Added 2D code, March 1993
 * Added Outer products, C++ proofed, Linas Vepstas October 1993
 */

#ifndef __GUTIL_VECTOR_H__
#define __GUTIL_VECTOR_H__

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


#include <math.h>
#include "port.h"

/* ========================================================== */
/* Zero out a 2D vector */

#define VEC_ZERO_2(a)				\
{						\
   (a)[0] = (a)[1] = 0.0;			\
}

/* ========================================================== */
/* Zero out a 3D vector */

#define VEC_ZERO(a)				\
{						\
   (a)[0] = (a)[1] = (a)[2] = 0.0;		\
}

/* ========================================================== */
/* Zero out a 4D vector */

#define VEC_ZERO_4(a)				\
{						\
   (a)[0] = (a)[1] = (a)[2] = (a)[3] = 0.0;	\
}

/* ========================================================== */
/* Vector copy */

#define VEC_COPY_2(b,a)				\
{						\
   (b)[0] = (a)[0];				\
   (b)[1] = (a)[1];				\
}

/* ========================================================== */
/* Copy 3D vector */

#define VEC_COPY(b,a)				\
{						\
   (b)[0] = (a)[0];				\
   (b)[1] = (a)[1];				\
   (b)[2] = (a)[2];				\
}

/* ========================================================== */
/* Copy 4D vector */

#define VEC_COPY_4(b,a)				\
{						\
   (b)[0] = (a)[0];				\
   (b)[1] = (a)[1];				\
   (b)[2] = (a)[2];				\
   (b)[3] = (a)[3];				\
}

/* ========================================================== */
/* Vector difference */

#define VEC_DIFF_2(v21,v2,v1)			\
{						\
   (v21)[0] = (v2)[0] - (v1)[0];		\
   (v21)[1] = (v2)[1] - (v1)[1];		\
}

/* ========================================================== */
/* Vector difference */

#define VEC_DIFF(v21,v2,v1)			\
{						\
   (v21)[0] = (v2)[0] - (v1)[0];		\
   (v21)[1] = (v2)[1] - (v1)[1];		\
   (v21)[2] = (v2)[2] - (v1)[2];		\
}

/* ========================================================== */
/* Vector difference */

#define VEC_DIFF_4(v21,v2,v1)			\
{						\
   (v21)[0] = (v2)[0] - (v1)[0];		\
   (v21)[1] = (v2)[1] - (v1)[1];		\
   (v21)[2] = (v2)[2] - (v1)[2];		\
   (v21)[3] = (v2)[3] - (v1)[3];		\
}

/* ========================================================== */
/* Vector sum */

#define VEC_SUM_2(v21,v2,v1)			\
{						\
   (v21)[0] = (v2)[0] + (v1)[0];		\
   (v21)[1] = (v2)[1] + (v1)[1];		\
}

/* ========================================================== */
/* Vector sum */

#define VEC_SUM(v21,v2,v1)			\
{						\
   (v21)[0] = (v2)[0] + (v1)[0];		\
   (v21)[1] = (v2)[1] + (v1)[1];		\
   (v21)[2] = (v2)[2] + (v1)[2];		\
}

/* ========================================================== */
/* Vector sum */

#define VEC_SUM_4(v21,v2,v1)			\
{						\
   (v21)[0] = (v2)[0] + (v1)[0];		\
   (v21)[1] = (v2)[1] + (v1)[1];		\
   (v21)[2] = (v2)[2] + (v1)[2];		\
   (v21)[3] = (v2)[3] + (v1)[3];		\
}

/* ========================================================== */
/* scalar times vector */

#define VEC_SCALE_2(c,a,b)			\
{						\
   (c)[0] = (a)*(b)[0];				\
   (c)[1] = (a)*(b)[1];				\
}

/* ========================================================== */
/* scalar times vector */

#define VEC_SCALE(c,a,b)			\
{						\
   (c)[0] = (a)*(b)[0];				\
   (c)[1] = (a)*(b)[1];				\
   (c)[2] = (a)*(b)[2];				\
}

/* ========================================================== */
/* scalar times vector */

#define VEC_SCALE_4(c,a,b)			\
{						\
   (c)[0] = (a)*(b)[0];				\
   (c)[1] = (a)*(b)[1];				\
   (c)[2] = (a)*(b)[2];				\
   (c)[3] = (a)*(b)[3];				\
}

/* ========================================================== */
/* accumulate scaled vector */

#define VEC_ACCUM_2(c,a,b)			\
{						\
   (c)[0] += (a)*(b)[0];			\
   (c)[1] += (a)*(b)[1];			\
}

/* ========================================================== */
/* accumulate scaled vector */

#define VEC_ACCUM(c,a,b)			\
{						\
   (c)[0] += (a)*(b)[0];			\
   (c)[1] += (a)*(b)[1];			\
   (c)[2] += (a)*(b)[2];			\
}

/* ========================================================== */
/* accumulate scaled vector */

#define VEC_ACCUM_4(c,a,b)			\
{						\
   (c)[0] += (a)*(b)[0];			\
   (c)[1] += (a)*(b)[1];			\
   (c)[2] += (a)*(b)[2];			\
   (c)[3] += (a)*(b)[3];			\
}

/* ========================================================== */
/* Vector dot product */

#define VEC_DOT_PRODUCT_2(c,a,b)			\
{							\
   c = (a)[0]*(b)[0] + (a)[1]*(b)[1];			\
}

/* ========================================================== */
/* Vector dot product */

#define VEC_DOT_PRODUCT(c,a,b)				\
{							\
   c = (a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2];	\
}

/* ========================================================== */
/* Vector dot product */

#define VEC_DOT_PRODUCT_4(c,a,b)			\
{							\
   c = (a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2] + (a)[3]*(b)[3] ;	\
}

/* ========================================================== */
/* vector impact parameter (squared) */

#define VEC_IMPACT_SQ(bsq,direction,position)		\
{							\
   gleDouble vlen, llel;					\
   VEC_DOT_PRODUCT (vlen, position, position);		\
   VEC_DOT_PRODUCT (llel, direction, position);		\
   bsq = vlen - llel*llel;				\
}

/* ========================================================== */
/* vector impact parameter */

#define VEC_IMPACT(bsq,direction,position)		\
{							\
   VEC_IMPACT_SQ(bsq,direction,position);		\
   bsq = sqrt (bsq);					\
}

/* ========================================================== */
/* Vector length */

#define VEC_LENGTH_2(vlen,a)			\
{						\
   vlen = a[0]*a[0] + a[1]*a[1];			\
   vlen = sqrt (vlen);				\
}

/* ========================================================== */
/* Vector length */

#define VEC_LENGTH(vlen,a)			\
{						\
   vlen = (a)[0]*(a)[0] + (a)[1]*(a)[1];		\
   vlen += (a)[2]*(a)[2];			\
   vlen = sqrt (vlen);				\
}

/* ========================================================== */
/* Vector length */

#define VEC_LENGTH_4(vlen,a)			\
{						\
   vlen = (a)[0]*(a)[0] + (a)[1]*(a)[1];		\
   vlen += (a)[2]*(a)[2];			\
   vlen += (a)[3] * (a)[3];			\
   vlen = sqrt (vlen);				\
}

/* ========================================================== */
/* distance between two points */

#define VEC_DISTANCE(vlen,va,vb)			\
{						\
    gleDouble tmp[4];				\
    VEC_DIFF (tmp, vb, va);			\
    VEC_LENGTH (vlen, tmp);			\
}

/* ========================================================== */
/* Vector length */

#define VEC_CONJUGATE_LENGTH(vlen,a)		\
{						\
   vlen = 1.0 - a[0]*a[0] - a[1]*a[1] - a[2]*a[2];\
   vlen = sqrt (vlen);				\
}

/* ========================================================== */
/* Vector length */

#define VEC_NORMALIZE(a)			\
{						\
   double vlen;					\
   VEC_LENGTH (vlen,a);				\
   if (vlen != 0.0) {				\
      vlen = 1.0 / vlen;				\
      a[0] *= vlen;				\
      a[1] *= vlen;				\
      a[2] *= vlen;				\
   }						\
}

/* ========================================================== */
/* Vector length */

#define VEC_RENORMALIZE(a,newlen)		\
{						\
   double vlen;					\
   VEC_LENGTH (vlen,a);				\
   if (vlen != 0.0) {				\
      vlen = newlen / vlen;				\
      a[0] *= vlen;				\
      a[1] *= vlen;				\
      a[2] *= vlen;				\
   }						\
}

/* ========================================================== */
/* 3D Vector cross product yeilding vector */

#define VEC_CROSS_PRODUCT(c,a,b)		\
{						\
   c[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1];	\
   c[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2];	\
   c[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0];	\
}

/* ========================================================== */
/* Vector perp -- assumes that n is of unit length 
 * accepts vector v, subtracts out any component parallel to n */

#define VEC_PERP(vp,v,n)			\
{						\
   double vdot;					\
						\
   VEC_DOT_PRODUCT (vdot, v, n);			\
   vp[0] = (v)[0] - (vdot) * (n)[0];		\
   vp[1] = (v)[1] - (vdot) * (n)[1];		\
   vp[2] = (v)[2] - (vdot) * (n)[2];		\
}

/* ========================================================== */
/* Vector parallel -- assumes that n is of unit length 
 * accepts vector v, subtracts out any component perpendicular to n */

#define VEC_PARALLEL(vp,v,n)			\
{						\
   double vdot;					\
						\
   VEC_DOT_PRODUCT (vdot, v, n);			\
   vp[0] = (vdot) * (n)[0];			\
   vp[1] = (vdot) * (n)[1];			\
   vp[2] = (vdot) * (n)[2];			\
}

/* ========================================================== */
/* Vector reflection -- assumes n is of unit length */
/* Takes vector v, reflects it against reflector n, and returns vr */

#define VEC_REFLECT(vr,v,n)			\
{						\
   double vdot;					\
						\
   VEC_DOT_PRODUCT (vdot, v, n);			\
   vr[0] = (v)[0] - 2.0 * (vdot) * (n)[0];	\
   vr[1] = (v)[1] - 2.0 * (vdot) * (n)[1];	\
   vr[2] = (v)[2] - 2.0 * (vdot) * (n)[2];	\
}

/* ========================================================== */
/* Vector blending */
/* Takes two vectors a, b, blends them together */ 

#define VEC_BLEND(vr,sa,a,sb,b)			\
{						\
						\
   vr[0] = (sa) * (a)[0] + (sb) * (b)[0];	\
   vr[1] = (sa) * (a)[1] + (sb) * (b)[1];	\
   vr[2] = (sa) * (a)[2] + (sb) * (b)[2];	\
}

/* ========================================================== */
/* Vector print */

#define VEC_PRINT_2(a)					\
{							\
   double vlen;						\
   VEC_LENGTH_2 (vlen, a);					\
   printf (" a is %f %f length of a is %f \n", a[0], a[1], vlen); \
}

/* ========================================================== */
/* Vector print */

#define VEC_PRINT(a)					\
{							\
   double vlen;						\
   VEC_LENGTH (vlen, (a));				\
   printf (" a is %f %f %f length of a is %f \n", (a)[0], (a)[1], (a)[2], vlen); \
}

/* ========================================================== */
/* Vector print */

#define VEC_PRINT_4(a)					\
{							\
   double vlen;						\
   VEC_LENGTH_4 (vlen, (a));				\
   printf (" a is %f %f %f %f length of a is %f \n",	\
       (a)[0], (a)[1], (a)[2], (a)[3], vlen);		\
}

/* ========================================================== */
/* print matrix */

#define MAT_PRINT_4X4(mmm) {				\
   int i,j;						\
   printf ("matrix mmm is \n");				\
   if (mmm == NULL) {					\
      printf (" Null \n");				\
   } else {						\
      for (i=0; i<4; i++) {				\
         for (j=0; j<4; j++) {				\
            printf ("%f ", mmm[i][j]);			\
         }						\
         printf (" \n");				\
      }							\
   }							\
}

/* ========================================================== */
/* print matrix */

#define MAT_PRINT_3X3(mmm) {				\
   int i,j;						\
   printf ("matrix mmm is \n");				\
   if (mmm == NULL) {					\
      printf (" Null \n");				\
   } else {						\
      for (i=0; i<3; i++) {				\
         for (j=0; j<3; j++) {				\
            printf ("%f ", mmm[i][j]);			\
         }						\
         printf (" \n");				\
      }							\
   }							\
}

/* ========================================================== */
/* print matrix */

#define MAT_PRINT_2X3(mmm) {				\
   int i,j;						\
   printf ("matrix mmm is \n");				\
   if (mmm == NULL) {					\
      printf (" Null \n");				\
   } else {						\
      for (i=0; i<2; i++) {				\
         for (j=0; j<3; j++) {				\
            printf ("%f ", mmm[i][j]);			\
         }						\
         printf (" \n");				\
      }							\
   }							\
}

/* ========================================================== */
/* initialize matrix */

#define IDENTIFY_MATRIX_3X3(m)			\
{						\
   m[0][0] = 1.0;				\
   m[0][1] = 0.0;				\
   m[0][2] = 0.0;				\
						\
   m[1][0] = 0.0;				\
   m[1][1] = 1.0;				\
   m[1][2] = 0.0;				\
						\
   m[2][0] = 0.0;				\
   m[2][1] = 0.0;				\
   m[2][2] = 1.0;				\
}

/* ========================================================== */
/* initialize matrix */

#define IDENTIFY_MATRIX_4X4(m)			\
{						\
   m[0][0] = 1.0;				\
   m[0][1] = 0.0;				\
   m[0][2] = 0.0;				\
   m[0][3] = 0.0;				\
						\
   m[1][0] = 0.0;				\
   m[1][1] = 1.0;				\
   m[1][2] = 0.0;				\
   m[1][3] = 0.0;				\
						\
   m[2][0] = 0.0;				\
   m[2][1] = 0.0;				\
   m[2][2] = 1.0;				\
   m[2][3] = 0.0;				\
						\
   m[3][0] = 0.0;				\
   m[3][1] = 0.0;				\
   m[3][2] = 0.0;				\
   m[3][3] = 1.0;				\
}

/* ========================================================== */
/* matrix copy */

#define COPY_MATRIX_2X2(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[0][1];		\
				\
   b[1][0] = a[1][0];		\
   b[1][1] = a[1][1];		\
				\
}

/* ========================================================== */
/* matrix copy */

#define COPY_MATRIX_2X3(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[0][1];		\
   b[0][2] = a[0][2];		\
				\
   b[1][0] = a[1][0];		\
   b[1][1] = a[1][1];		\
   b[1][2] = a[1][2];		\
}

/* ========================================================== */
/* matrix copy */

#define COPY_MATRIX_3X3(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[0][1];		\
   b[0][2] = a[0][2];		\
				\
   b[1][0] = a[1][0];		\
   b[1][1] = a[1][1];		\
   b[1][2] = a[1][2];		\
				\
   b[2][0] = a[2][0];		\
   b[2][1] = a[2][1];		\
   b[2][2] = a[2][2];		\
}

/* ========================================================== */
/* matrix copy */

#define COPY_MATRIX_4X4(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[0][1];		\
   b[0][2] = a[0][2];		\
   b[0][3] = a[0][3];		\
				\
   b[1][0] = a[1][0];		\
   b[1][1] = a[1][1];		\
   b[1][2] = a[1][2];		\
   b[1][3] = a[1][3];		\
				\
   b[2][0] = a[2][0];		\
   b[2][1] = a[2][1];		\
   b[2][2] = a[2][2];		\
   b[2][3] = a[2][3];		\
				\
   b[3][0] = a[3][0];		\
   b[3][1] = a[3][1];		\
   b[3][2] = a[3][2];		\
   b[3][3] = a[3][3];		\
}

/* ========================================================== */
/* matrix transpose */

#define TRANSPOSE_MATRIX_2X2(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[1][0];		\
				\
   b[1][0] = a[0][1];		\
   b[1][1] = a[1][1];		\
}

/* ========================================================== */
/* matrix transpose */

#define TRANSPOSE_MATRIX_3X3(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[1][0];		\
   b[0][2] = a[2][0];		\
				\
   b[1][0] = a[0][1];		\
   b[1][1] = a[1][1];		\
   b[1][2] = a[2][1];		\
				\
   b[2][0] = a[0][2];		\
   b[2][1] = a[1][2];		\
   b[2][2] = a[2][2];		\
}

/* ========================================================== */
/* matrix transpose */

#define TRANSPOSE_MATRIX_4X4(b,a)	\
{				\
   b[0][0] = a[0][0];		\
   b[0][1] = a[1][0];		\
   b[0][2] = a[2][0];		\
   b[0][3] = a[3][0];		\
				\
   b[1][0] = a[0][1];		\
   b[1][1] = a[1][1];		\
   b[1][2] = a[2][1];		\
   b[1][3] = a[3][1];		\
				\
   b[2][0] = a[0][2];		\
   b[2][1] = a[1][2];		\
   b[2][2] = a[2][2];		\
   b[2][3] = a[3][2];		\
				\
   b[3][0] = a[0][3];		\
   b[3][1] = a[1][3];		\
   b[3][2] = a[2][3];		\
   b[3][3] = a[3][3];		\
}

/* ========================================================== */
/* multiply matrix by scalar */

#define SCALE_MATRIX_2X2(b,s,a)		\
{					\
   b[0][0] = (s) * a[0][0];		\
   b[0][1] = (s) * a[0][1];		\
					\
   b[1][0] = (s) * a[1][0];		\
   b[1][1] = (s) * a[1][1];		\
}

/* ========================================================== */
/* multiply matrix by scalar */

#define SCALE_MATRIX_3X3(b,s,a)		\
{					\
   b[0][0] = (s) * a[0][0];		\
   b[0][1] = (s) * a[0][1];		\
   b[0][2] = (s) * a[0][2];		\
					\
   b[1][0] = (s) * a[1][0];		\
   b[1][1] = (s) * a[1][1];		\
   b[1][2] = (s) * a[1][2];		\
					\
   b[2][0] = (s) * a[2][0];		\
   b[2][1] = (s) * a[2][1];		\
   b[2][2] = (s) * a[2][2];		\
}

/* ========================================================== */
/* multiply matrix by scalar */

#define SCALE_MATRIX_4X4(b,s,a)		\
{					\
   b[0][0] = (s) * a[0][0];		\
   b[0][1] = (s) * a[0][1];		\
   b[0][2] = (s) * a[0][2];		\
   b[0][3] = (s) * a[0][3];		\
					\
   b[1][0] = (s) * a[1][0];		\
   b[1][1] = (s) * a[1][1];		\
   b[1][2] = (s) * a[1][2];		\
   b[1][3] = (s) * a[1][3];		\
					\
   b[2][0] = (s) * a[2][0];		\
   b[2][1] = (s) * a[2][1];		\
   b[2][2] = (s) * a[2][2];		\
   b[2][3] = (s) * a[2][3];		\
					\
   b[3][0] = s * a[3][0];		\
   b[3][1] = s * a[3][1];		\
   b[3][2] = s * a[3][2];		\
   b[3][3] = s * a[3][3];		\
}

/* ========================================================== */
/* multiply matrix by scalar */

#define ACCUM_SCALE_MATRIX_2X2(b,s,a)		\
{					\
   b[0][0] += (s) * a[0][0];		\
   b[0][1] += (s) * a[0][1];		\
					\
   b[1][0] += (s) * a[1][0];		\
   b[1][1] += (s) * a[1][1];		\
}

/* +========================================================== */
/* multiply matrix by scalar */

#define ACCUM_SCALE_MATRIX_3X3(b,s,a)		\
{					\
   b[0][0] += (s) * a[0][0];		\
   b[0][1] += (s) * a[0][1];		\
   b[0][2] += (s) * a[0][2];		\
					\
   b[1][0] += (s) * a[1][0];		\
   b[1][1] += (s) * a[1][1];		\
   b[1][2] += (s) * a[1][2];		\
					\
   b[2][0] += (s) * a[2][0];		\
   b[2][1] += (s) * a[2][1];		\
   b[2][2] += (s) * a[2][2];		\
}

/* +========================================================== */
/* multiply matrix by scalar */

#define ACCUM_SCALE_MATRIX_4X4(b,s,a)		\
{					\
   b[0][0] += (s) * a[0][0];		\
   b[0][1] += (s) * a[0][1];		\
   b[0][2] += (s) * a[0][2];		\
   b[0][3] += (s) * a[0][3];		\
					\
   b[1][0] += (s) * a[1][0];		\
   b[1][1] += (s) * a[1][1];		\
   b[1][2] += (s) * a[1][2];		\
   b[1][3] += (s) * a[1][3];		\
					\
   b[2][0] += (s) * a[2][0];		\
   b[2][1] += (s) * a[2][1];		\
   b[2][2] += (s) * a[2][2];		\
   b[2][3] += (s) * a[2][3];		\
					\
   b[3][0] += (s) * a[3][0];		\
   b[3][1] += (s) * a[3][1];		\
   b[3][2] += (s) * a[3][2];		\
   b[3][3] += (s) * a[3][3];		\
}

/* +========================================================== */
/* matrix product */
/* c[x][y] = a[x][0]*b[0][y]+a[x][1]*b[1][y]+a[x][2]*b[2][y]+a[x][3]*b[3][y];*/

#define MATRIX_PRODUCT_2X2(c,a,b)		\
{						\
   c[0][0] = a[0][0]*b[0][0]+a[0][1]*b[1][0];	\
   c[0][1] = a[0][0]*b[0][1]+a[0][1]*b[1][1];	\
						\
   c[1][0] = a[1][0]*b[0][0]+a[1][1]*b[1][0];	\
   c[1][1] = a[1][0]*b[0][1]+a[1][1]*b[1][1];	\
						\
}

/* ========================================================== */
/* matrix product */
/* c[x][y] = a[x][0]*b[0][y]+a[x][1]*b[1][y]+a[x][2]*b[2][y]+a[x][3]*b[3][y];*/

#define MATRIX_PRODUCT_3X3(c,a,b)				\
{								\
   c[0][0] = a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0];	\
   c[0][1] = a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1];	\
   c[0][2] = a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2];	\
								\
   c[1][0] = a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0];	\
   c[1][1] = a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1];	\
   c[1][2] = a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2];	\
								\
   c[2][0] = a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0];	\
   c[2][1] = a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1];	\
   c[2][2] = a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2];	\
}

/* ========================================================== */
/* matrix product */
/* c[x][y] = a[x][0]*b[0][y]+a[x][1]*b[1][y]+a[x][2]*b[2][y]+a[x][3]*b[3][y];*/

#define MATRIX_PRODUCT_4X4(c,a,b)		\
{						\
   c[0][0] = a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0]+a[0][3]*b[3][0];\
   c[0][1] = a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1]+a[0][3]*b[3][1];\
   c[0][2] = a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2]+a[0][3]*b[3][2];\
   c[0][3] = a[0][0]*b[0][3]+a[0][1]*b[1][3]+a[0][2]*b[2][3]+a[0][3]*b[3][3];\
						\
   c[1][0] = a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0]+a[1][3]*b[3][0];\
   c[1][1] = a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1]+a[1][3]*b[3][1];\
   c[1][2] = a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2]+a[1][3]*b[3][2];\
   c[1][3] = a[1][0]*b[0][3]+a[1][1]*b[1][3]+a[1][2]*b[2][3]+a[1][3]*b[3][3];\
						\
   c[2][0] = a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0]+a[2][3]*b[3][0];\
   c[2][1] = a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1]+a[2][3]*b[3][1];\
   c[2][2] = a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2]+a[2][3]*b[3][2];\
   c[2][3] = a[2][0]*b[0][3]+a[2][1]*b[1][3]+a[2][2]*b[2][3]+a[2][3]*b[3][3];\
						\
   c[3][0] = a[3][0]*b[0][0]+a[3][1]*b[1][0]+a[3][2]*b[2][0]+a[3][3]*b[3][0];\
   c[3][1] = a[3][0]*b[0][1]+a[3][1]*b[1][1]+a[3][2]*b[2][1]+a[3][3]*b[3][1];\
   c[3][2] = a[3][0]*b[0][2]+a[3][1]*b[1][2]+a[3][2]*b[2][2]+a[3][3]*b[3][2];\
   c[3][3] = a[3][0]*b[0][3]+a[3][1]*b[1][3]+a[3][2]*b[2][3]+a[3][3]*b[3][3];\
}

/* ========================================================== */
/* matrix times vector */

#define MAT_DOT_VEC_2X2(p,m,v)					\
{								\
   p[0] = m[0][0]*v[0] + m[0][1]*v[1];				\
   p[1] = m[1][0]*v[0] + m[1][1]*v[1];				\
}

/* ========================================================== */
/* matrix times vector */

#define MAT_DOT_VEC_3X3(p,m,v)					\
{								\
   p[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2];		\
   p[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2];		\
   p[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2];		\
}

/* ========================================================== */
/* matrix times vector */

#define MAT_DOT_VEC_4X4(p,m,v)					\
{								\
   p[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]*v[3];	\
   p[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]*v[3];	\
   p[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]*v[3];	\
   p[3] = m[3][0]*v[0] + m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]*v[3];	\
}

/* ========================================================== */
/* vector transpose times matrix */
/* p[j] = v[0]*m[0][j] + v[1]*m[1][j] + v[2]*m[2][j]; */

#define VEC_DOT_MAT_3X3(p,v,m)					\
{								\
   p[0] = v[0]*m[0][0] + v[1]*m[1][0] + v[2]*m[2][0];		\
   p[1] = v[0]*m[0][1] + v[1]*m[1][1] + v[2]*m[2][1];		\
   p[2] = v[0]*m[0][2] + v[1]*m[1][2] + v[2]*m[2][2];		\
}

/* ========================================================== */
/* affine matrix times vector */
/* The matrix is assumed to be an affine matrix, with last two 
 * entries representing a translation */

#define MAT_DOT_VEC_2X3(p,m,v)					\
{								\
   p[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2];		\
   p[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2];		\
}

/* ========================================================== */
/* inverse transpose of matrix times vector
 *
 * This macro computes inverse transpose of matrix m, 
 * and multiplies vector v into it, to yeild vector p
 *
 * DANGER !!! Do Not use this on normal vectors!!!
 * It will leave normals the wrong length !!!
 * See macro below for use on normals.
 */

#define INV_TRANSP_MAT_DOT_VEC_2X2(p,m,v)			\
{								\
   gleDouble det;						\
								\
   det = m[0][0]*m[1][1] - m[0][1]*m[1][0];			\
   p[0] = m[1][1]*v[0] - m[1][0]*v[1];				\
   p[1] = - m[0][1]*v[0] + m[0][0]*v[1];			\
								\
   /* if matrix not singular, and not orthonormal, then renormalize */ \
   if ((det!=1.0) && (det != 0.0)) {				\
      det = 1.0 / det;						\
      p[0] *= det;						\
      p[1] *= det;						\
   }								\
}

/* ========================================================== */
/* transform normal vector by inverse transpose of matrix 
 * and then renormalize the vector 
 *
 * This macro computes inverse transpose of matrix m, 
 * and multiplies vector v into it, to yeild vector p
 * Vector p is then normalized.
 */


#define NORM_XFORM_2X2(p,m,v)					\
{								\
   double mlen;							\
								\
   /* do nothing if off-diagonals are zero and diagonals are 	\
    * equal */							\
   if ((m[0][1] != 0.0) || (m[1][0] != 0.0) || (m[0][0] != m[1][1])) { \
      p[0] = m[1][1]*v[0] - m[1][0]*v[1];			\
      p[1] = - m[0][1]*v[0] + m[0][0]*v[1];			\
								\
      mlen = p[0]*p[0] + p[1]*p[1];				\
      mlen = 1.0 / sqrt (mlen);					\
      p[0] *= mlen;						\
      p[1] *= mlen;						\
   } else {							\
      VEC_COPY_2 (p, v);					\
   }								\
}

/* ========================================================== */
/* outer product of vector times vector transpose 
 *
 * The outer product of vector v and vector transpose t yeilds 
 * dyadic matrix m.
 */

#define OUTER_PRODUCT_2X2(m,v,t)				\
{								\
   m[0][0] = v[0] * t[0];					\
   m[0][1] = v[0] * t[1];					\
								\
   m[1][0] = v[1] * t[0];					\
   m[1][1] = v[1] * t[1];					\
}

/* ========================================================== */
/* outer product of vector times vector transpose 
 *
 * The outer product of vector v and vector transpose t yeilds 
 * dyadic matrix m.
 */

#define OUTER_PRODUCT_3X3(m,v,t)				\
{								\
   m[0][0] = v[0] * t[0];					\
   m[0][1] = v[0] * t[1];					\
   m[0][2] = v[0] * t[2];					\
								\
   m[1][0] = v[1] * t[0];					\
   m[1][1] = v[1] * t[1];					\
   m[1][2] = v[1] * t[2];					\
								\
   m[2][0] = v[2] * t[0];					\
   m[2][1] = v[2] * t[1];					\
   m[2][2] = v[2] * t[2];					\
}

/* ========================================================== */
/* outer product of vector times vector transpose 
 *
 * The outer product of vector v and vector transpose t yeilds 
 * dyadic matrix m.
 */

#define OUTER_PRODUCT_4X4(m,v,t)				\
{								\
   m[0][0] = v[0] * t[0];					\
   m[0][1] = v[0] * t[1];					\
   m[0][2] = v[0] * t[2];					\
   m[0][3] = v[0] * t[3];					\
								\
   m[1][0] = v[1] * t[0];					\
   m[1][1] = v[1] * t[1];					\
   m[1][2] = v[1] * t[2];					\
   m[1][3] = v[1] * t[3];					\
								\
   m[2][0] = v[2] * t[0];					\
   m[2][1] = v[2] * t[1];					\
   m[2][2] = v[2] * t[2];					\
   m[2][3] = v[2] * t[3];					\
								\
   m[3][0] = v[3] * t[0];					\
   m[3][1] = v[3] * t[1];					\
   m[3][2] = v[3] * t[2];					\
   m[3][3] = v[3] * t[3];					\
}

/* +========================================================== */
/* outer product of vector times vector transpose 
 *
 * The outer product of vector v and vector transpose t yeilds 
 * dyadic matrix m.
 */

#define ACCUM_OUTER_PRODUCT_2X2(m,v,t)				\
{								\
   m[0][0] += v[0] * t[0];					\
   m[0][1] += v[0] * t[1];					\
								\
   m[1][0] += v[1] * t[0];					\
   m[1][1] += v[1] * t[1];					\
}

/* +========================================================== */
/* outer product of vector times vector transpose 
 *
 * The outer product of vector v and vector transpose t yeilds 
 * dyadic matrix m.
 */

#define ACCUM_OUTER_PRODUCT_3X3(m,v,t)				\
{								\
   m[0][0] += v[0] * t[0];					\
   m[0][1] += v[0] * t[1];					\
   m[0][2] += v[0] * t[2];					\
								\
   m[1][0] += v[1] * t[0];					\
   m[1][1] += v[1] * t[1];					\
   m[1][2] += v[1] * t[2];					\
								\
   m[2][0] += v[2] * t[0];					\
   m[2][1] += v[2] * t[1];					\
   m[2][2] += v[2] * t[2];					\
}

/* +========================================================== */
/* outer product of vector times vector transpose 
 *
 * The outer product of vector v and vector transpose t yeilds 
 * dyadic matrix m.
 */

#define ACCUM_OUTER_PRODUCT_4X4(m,v,t)				\
{								\
   m[0][0] += v[0] * t[0];					\
   m[0][1] += v[0] * t[1];					\
   m[0][2] += v[0] * t[2];					\
   m[0][3] += v[0] * t[3];					\
								\
   m[1][0] += v[1] * t[0];					\
   m[1][1] += v[1] * t[1];					\
   m[1][2] += v[1] * t[2];					\
   m[1][3] += v[1] * t[3];					\
								\
   m[2][0] += v[2] * t[0];					\
   m[2][1] += v[2] * t[1];					\
   m[2][2] += v[2] * t[2];					\
   m[2][3] += v[2] * t[3];					\
								\
   m[3][0] += v[3] * t[0];					\
   m[3][1] += v[3] * t[1];					\
   m[3][2] += v[3] * t[2];					\
   m[3][3] += v[3] * t[3];					\
}

/* +========================================================== */
/* determinant of matrix
 *
 * Computes determinant of matrix m, returning d
 */

#define DETERMINANT_2X2(d,m)					\
{								\
   d = m[0][0] * m[1][1] - m[0][1] * m[1][0];			\
}

/* ========================================================== */
/* determinant of matrix
 *
 * Computes determinant of matrix m, returning d
 */

#define DETERMINANT_3X3(d,m)					\
{								\
   d = m[0][0] * (m[1][1]*m[2][2] - m[1][2] * m[2][1]);		\
   d -= m[0][1] * (m[1][0]*m[2][2] - m[1][2] * m[2][0]);	\
   d += m[0][2] * (m[1][0]*m[2][1] - m[1][1] * m[2][0]);	\
}

/* ========================================================== */
/* i,j,th cofactor of a 4x4 matrix
 *
 */

#define COFACTOR_4X4_IJ(fac,m,i,j) 				\
{								\
   int ii[4], jj[4], k;						\
								\
   /* compute which row, columnt to skip */			\
   for (k=0; k<i; k++) ii[k] = k;				\
   for (k=i; k<3; k++) ii[k] = k+1;				\
   for (k=0; k<j; k++) jj[k] = k;				\
   for (k=j; k<3; k++) jj[k] = k+1;				\
								\
   (fac) = m[ii[0]][jj[0]] * (m[ii[1]][jj[1]]*m[ii[2]][jj[2]] 	\
                            - m[ii[1]][jj[2]]*m[ii[2]][jj[1]]); \
   (fac) -= m[ii[0]][jj[1]] * (m[ii[1]][jj[0]]*m[ii[2]][jj[2]]	\
                             - m[ii[1]][jj[2]]*m[ii[2]][jj[0]]);\
   (fac) += m[ii[0]][jj[2]] * (m[ii[1]][jj[0]]*m[ii[2]][jj[1]]	\
                             - m[ii[1]][jj[1]]*m[ii[2]][jj[0]]);\
								\
   /* compute sign */						\
   k = i+j;							\
   if ( k != (k/2)*2) {						\
      (fac) = -(fac);						\
   }								\
}

/* ========================================================== */
/* determinant of matrix
 *
 * Computes determinant of matrix m, returning d
 */

#define DETERMINANT_4X4(d,m)					\
{								\
   double cofac;						\
   COFACTOR_4X4_IJ (cofac, m, 0, 0);				\
   d = m[0][0] * cofac;						\
   COFACTOR_4X4_IJ (cofac, m, 0, 1);				\
   d += m[0][1] * cofac;					\
   COFACTOR_4X4_IJ (cofac, m, 0, 2);				\
   d += m[0][2] * cofac;					\
   COFACTOR_4X4_IJ (cofac, m, 0, 3);				\
   d += m[0][3] * cofac;					\
}

/* ========================================================== */
/* cofactor of matrix
 *
 * Computes cofactor of matrix m, returning a
 */

#define COFACTOR_2X2(a,m)					\
{								\
   a[0][0] = (m)[1][1];						\
   a[0][1] = - (m)[1][0];						\
   a[1][0] = - (m)[0][1];						\
   a[1][1] = (m)[0][0];						\
}

/* ========================================================== */
/* cofactor of matrix
 *
 * Computes cofactor of matrix m, returning a
 */

#define COFACTOR_3X3(a,m)					\
{								\
   a[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];			\
   a[0][1] = - (m[1][0]*m[2][2] - m[2][0]*m[1][2]);		\
   a[0][2] = m[1][0]*m[2][1] - m[1][1]*m[2][0];			\
   a[1][0] = - (m[0][1]*m[2][2] - m[0][2]*m[2][1]);		\
   a[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];			\
   a[1][2] = - (m[0][0]*m[2][1] - m[0][1]*m[2][0]);		\
   a[2][0] = m[0][1]*m[1][2] - m[0][2]*m[1][1];			\
   a[2][1] = - (m[0][0]*m[1][2] - m[0][2]*m[1][0]);		\
   a[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0]);		\
}

/* ========================================================== */
/* cofactor of matrix
 *
 * Computes cofactor of matrix m, returning a
 */

#define COFACTOR_4X4(a,m)					\
{								\
   int i,j;							\
								\
   for (i=0; i<4; i++) {					\
      for (j=0; j<4; j++) {					\
         COFACTOR_4X4_IJ (a[i][j], m, i, j);			\
      }								\
   }								\
}

/* ========================================================== */
/* adjoint of matrix
 *
 * Computes adjoint of matrix m, returning a
 * (Note that adjoint is just the transpose of the cofactor matrix)
 */

#define ADJOINT_2X2(a,m)					\
{								\
   a[0][0] = (m)[1][1];						\
   a[1][0] = - (m)[1][0];						\
   a[0][1] = - (m)[0][1];						\
   a[1][1] = (m)[0][0];						\
}

/* ========================================================== */
/* adjoint of matrix
 *
 * Computes adjoint of matrix m, returning a
 * (Note that adjoint is just the transpose of the cofactor matrix)
 */


#define ADJOINT_3X3(a,m)					\
{								\
   a[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];			\
   a[1][0] = - (m[1][0]*m[2][2] - m[2][0]*m[1][2]);		\
   a[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];			\
   a[0][1] = - (m[0][1]*m[2][2] - m[0][2]*m[2][1]);		\
   a[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];			\
   a[2][1] = - (m[0][0]*m[2][1] - m[0][1]*m[2][0]);		\
   a[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];			\
   a[1][2] = - (m[0][0]*m[1][2] - m[0][2]*m[1][0]);		\
   a[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0]);		\
}

/* ========================================================== */
/* adjoint of matrix
 *
 * Computes adjoint of matrix m, returning a
 * (Note that adjoint is just the transpose of the cofactor matrix)
 */

#define ADJOINT_4X4(a,m)					\
{								\
   int i,j;							\
								\
   for (i=0; i<4; i++) {					\
      for (j=0; j<4; j++) {					\
         COFACTOR_4X4_IJ (a[j][i], m, i, j);			\
      }								\
   }								\
}

/* ========================================================== */
/* compute adjoint of matrix and scale
 *
 * Computes adjoint of matrix m, scales it by s, returning a
 */

#define SCALE_ADJOINT_2X2(a,s,m)				\
{								\
   a[0][0] = (s) * m[1][1];					\
   a[1][0] = - (s) * m[1][0];					\
   a[0][1] = - (s) * m[0][1];					\
   a[1][1] = (s) * m[0][0];					\
}

/* ========================================================== */
/* compute adjoint of matrix and scale
 *
 * Computes adjoint of matrix m, scales it by s, returning a
 */

#define SCALE_ADJOINT_3X3(a,s,m)				\
{								\
   a[0][0] = (s) * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);	\
   a[1][0] = (s) * (m[1][2] * m[2][0] - m[1][0] * m[2][2]);	\
   a[2][0] = (s) * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);	\
								\
   a[0][1] = (s) * (m[0][2] * m[2][1] - m[0][1] * m[2][2]);	\
   a[1][1] = (s) * (m[0][0] * m[2][2] - m[0][2] * m[2][0]);	\
   a[2][1] = (s) * (m[0][1] * m[2][0] - m[0][0] * m[2][1]);	\
								\
   a[0][2] = (s) * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);	\
   a[1][2] = (s) * (m[0][2] * m[1][0] - m[0][0] * m[1][2]);	\
   a[2][2] = (s) * (m[0][0] * m[1][1] - m[0][1] * m[1][0]);	\
}

/* ========================================================== */
/* compute adjoint of matrix and scale
 *
 * Computes adjoint of matrix m, scales it by s, returning a
 */

#define SCALE_ADJOINT_4X4(a,s,m)				\
{								\
   int i,j;							\
								\
   for (i=0; i<4; i++) {					\
      for (j=0; j<4; j++) {					\
         COFACTOR_4X4_IJ (a[j][i], m, i, j);			\
         a[j][i] *= s;						\
      }								\
   }								\
}

/* ========================================================== */
/* inverse of matrix 
 *
 * Compute inverse of matrix a, returning determinant m and 
 * inverse b
 */

#define INVERT_2X2(b,det,a)			\
{						\
   double tmp;					\
   DETERMINANT_2X2 (det, a);			\
   tmp = 1.0 / (det);				\
   SCALE_ADJOINT_2X2 (b, tmp, a);		\
}

/* ========================================================== */
/* inverse of matrix 
 *
 * Compute inverse of matrix a, returning determinant m and 
 * inverse b
 */

#define INVERT_3X3(b,det,a)			\
{						\
   double tmp;					\
   DETERMINANT_3X3 (det, a);			\
   tmp = 1.0 / (det);				\
   SCALE_ADJOINT_3X3 (b, tmp, a);		\
}

/* ========================================================== */
/* inverse of matrix 
 *
 * Compute inverse of matrix a, returning determinant m and 
 * inverse b
 */

#define INVERT_4X4(b,det,a)			\
{						\
   double tmp;					\
   DETERMINANT_4X4 (det, a);			\
   tmp = 1.0 / (det);				\
   SCALE_ADJOINT_4X4 (b, tmp, a);		\
}

/* ========================================================== */
#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* __GUTIL_VECTOR_H__ */
/* ===================== END OF FILE ======================== */
