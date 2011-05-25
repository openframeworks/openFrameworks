
/*
 * port.h 
 *
 * FUNCTION:
 * This file contains defines for porting the tubing toolkit from GL to
 * OpenGL to some callback scheme.
 *
 * HISTORY:
 * Created by Linas Vepstas --  February 1993
 * Added auto texture coord generation hacks, Linas April 1994 
 */

#ifndef __GLE_PORT_H__
#define __GLE_PORT_H__


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ====================================================== */
/* Some compilers can't handle multiply-subscripted array's */

#ifdef FUNKY_C
typedef gleDouble gleVector;
#define AVAL(arr,n,i,j)  arr(6*n+3*i+j)
#define VVAL(arr,n,i)  arr(3*n+i)

#else  /* FUNKY_C */
typedef double gleVector[3];
typedef double glePoint[2];
#define AVAL(arr,n,i,j)  arr[n][i][j]
#define VVAL(arr,n,i)  arr[n][i];

#endif /* FUNKY_C */

/* ====================================================== */
/* These are used to convey info about topography to the 
 * texture mapping routines */

#define FRONT 		1
#define BACK 		2
#define FRONT_CAP 	3
#define BACK_CAP	4
#define FILLET		5

/* ====================================================== */

#define __GLE_DOUBLE

/* ====================================================== */

#ifdef __GLE_DOUBLE
#ifndef gleDouble
    #define gleDouble 		double
#endif
#define urot_axis(a,b,c) 	urot_axis_d(a,b,c)
#define uview_direction(a,b,c) 	uview_direction_d(a,b,c)
#define uviewpoint(a,b,c,d) 	uviewpoint_d(a,b,c,d)
#define MULTMATRIX(m)		MULTMATRIX_D(m)
#define LOADMATRIX(m)		LOADMATRIX_D(m)
#define V3F(x,j,id)		V3F_D(x,j,id)
#define N3F(x)			N3F_D(x)
#define T2F(x,y)		T2F_D(x,y)
#else 
#define gleDouble 		float
#define urot_axis(a,b,c) 	urot_axis_f(a,b,c)
#define uview_direction(a,b,c) 	uview_direction_f(a,b,c)
#define uviewpoint(a,b,c,d) 	uviewpoint_f(a,b,c,d)
#define MULTMATRIX(m)		MULTMATRIX_F(m)
#define LOADMATRIX(m)		LOADMATRIX_F(m)
#define V3F(x,j,id)		V3F_F(x,j,id)
#define N3F(x)			N3F_F(x)
#define T2F(x,y)		T2F_F(x,y)
#endif 

/* ====================================================== */

#if (defined DEBUG_GL_32 || DEBUG_OPENGL_10)
#undef GL_32
#undef OPENGL_10

#define BGNTMESH(i,len)	printf ("bgntmesh() \n");
#define ENDTMESH()      printf ("endtmesh() \n");
#define BGNPOLYGON()	printf ("bgnpolygon() \n");
#define ENDPOLYGON()	printf ("endpolygon() \n");
#define V3F_F(x,j,id)	printf ("v3f(x)		%f %f %f \n", x[0], x[1], x[2]);
#define V3F_D(x,j,id)	printf ("v3d(x) 	%f %f %f \n", x[0], x[1], x[2]);
#define	N3F_F(x)	printf ("n3f(x) 	%f %f %f \n", x[0], x[1], x[2]);
#define	N3F_D(x)	printf ("n3d(x)		%f %f %f \n", x[0], x[1], x[2]);
#define	C3F(x)		printf ("c3f(x) 	%f %f %f \n", x[0], x[1], x[2]);

#define	POPMATRIX()	printf ("popmatrix () \n");
#define	PUSHMATRIX()	printf ("pushmatrix() \n");
#define	MULTMATRIX_F(x)	MULTMATRIX_D(x)
#define	LOADMATRIX_F(x)	LOADMATRIX_D(x)


#define LOADMATRIX_D(x) {				\
   int i, j; 						\
   printf ("loadmatrix (x) \n");			\
   for (i=0; i<4; i++) {				\
      for (j=0; j<4; j++) {				\
         printf ( "%f ", x[i][j]);			\
      }							\
      printf (" \n");					\
   }							\
}

#define MULTMATRIX_D(x) {				\
   int i, j; 						\
   printf ("multmatrix (x) \n");			\
   for (i=0; i<4; i++) {				\
      for (j=0; j<4; j++) {				\
         printf ( "%f ", x[i][j]);			\
      }							\
      printf (" \n");					\
   }							\
}

#define __IS_LIGHTING_ON  (1)

#endif

/* ====================================================== */

#ifdef GL_32

#include <gl/gl.h>

#define BGNTMESH(i,len)	bgntmesh()
#define ENDTMESH()	endtmesh()
#define BGNPOLYGON()	bgnpolygon()
#define ENDPOLYGON()	endpolygon()
#define V3F_F(x,j,id)	v3f(x)
#define V3F_D(x,j,id)	v3d(x)
#define	N3F_F(x)	n3f(x)
#define	T2F_F(x,y)
#define	T2F_D(x,y)
#define	C3F(x)		c3f(x)

#define	POPMATRIX()	popmatrix ()
#define	PUSHMATRIX()	pushmatrix()
#define	MULTMATRIX_F(x)	multmatrix (x)
#define	LOADMATRIX_F(x)	loadmatrix (x)

#define	N3F_D(x) {					\
   float nnn[3];					\
   nnn[0] = (float) x[0]; 				\
   nnn[1] = (float) x[1]; 				\
   nnn[2] = (float) x[2]; 				\
   n3f (nnn);						\
}

#define LOADMATRIX_D(x) {				\
   int i, j; 						\
   float mmm[4][4];					\
   for (i=0; i<4; i++) {				\
      for (j=0; j<4; j++) {				\
         mmm[i][j] = (float) x[i][j];			\
      }							\
   }							\
   loadmatrix(mmm);					\
}

#define MULTMATRIX_D(x) {				\
   int i, j; 						\
   float mmm[4][4];					\
   for (i=0; i<4; i++) {				\
      for (j=0; j<4; j++) {				\
         mmm[i][j] = (float) x[i][j];			\
      }							\
   }							\
   multmatrix(mmm);					\
}

/* #define __IS_LIGHTING_ON  (MSINGLE == getmmode()) */
#define __IS_LIGHTING_ON  (extrusion_join_style & TUBE_LIGHTING_ON)

#endif /* GL_32 */

/* ====================================================== */
#ifdef OPENGL_10

#if defined(_WIN32)
#include <windows.h>
#pragma warning (disable:4244)          /* disable bogus conversion warnings */
#endif
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

/*
#define	N3F_F(x) {					\
   float nnn[3];					\
   nnn[0] = - (float) x[0]; 				\
   nnn[1] = - (float) x[1]; 				\
   nnn[2] = - (float) x[2]; 				\
   glNormal3fv (nnn);						\
}
#define	N3F_D(x) {					\
   float nnn[3];					\
   nnn[0] = - (float) x[0]; 				\
   nnn[1] = - (float) x[1]; 				\
   nnn[2] = - (float) x[2]; 				\
   glNormal3fv (nnn);						\
}
*/

#define	C3F(x)		glColor3fv(x)
#define	T2F_F(x,y)	glTexCoord2f(x,y)
#define	T2F_D(x,y)	glTexCoord2d(x,y)

#define	POPMATRIX()	glPopMatrix()
#define	PUSHMATRIX()	glPushMatrix()

#define	MULTMATRIX_F(x)	glMultMatrixf ((const GLfloat *)x)
#define	LOADMATRIX_F(x)	glLoadMatrixf ((const GLfloat *)x)

#define	MULTMATRIX_D(x)	glMultMatrixd ((const GLdouble *)x)
#define	LOADMATRIX_D(x)	glLoadMatrixd ((const GLdouble *)x)

#define __IS_LIGHTING_ON  (glIsEnabled(GL_LIGHTING))

/* ====================================================== */
#ifdef AUTO_TEXTURE

#define BGNTMESH(i,len) { 					\
	if(_gle_gc -> bgn_gen_texture) (*(_gle_gc -> bgn_gen_texture))(i,len);\
	glBegin (GL_TRIANGLE_STRIP); 			\
}

#define BGNPOLYGON() { 					\
	if(_gle_gc -> bgn_gen_texture) (*(_gle_gc -> bgn_gen_texture))();\
	glBegin (GL_POLYGON);				\
}

#define N3F_F(x) { 					\
	if(_gle_gc -> n3f_gen_texture) (*(_gle_gc -> n3f_gen_texture))(x); \
	glNormal3fv(x); 				\
}

#define N3F_D(x) { 					\
	if(_gle_gc -> n3d_gen_texture) (*(_gle_gc -> n3d_gen_texture))(x); \
	glNormal3dv(x); 				\
}

#define V3F_F(x,j,id) { 					\
	if(_gle_gc -> v3f_gen_texture) (*(_gle_gc -> v3f_gen_texture))(x,j,id);\
	glVertex3fv(x); 				\
}

#define V3F_D(x,j,id) { 					\
	if(_gle_gc -> v3d_gen_texture) (*(_gle_gc -> v3d_gen_texture))(x,j,id); \
	glVertex3dv(x); 				\
}

#define ENDTMESH() {					\
	if(_gle_gc -> end_gen_texture) (*(_gle_gc -> end_gen_texture))(); \
	glEnd ();					\
}

#define ENDPOLYGON() {					\
	if(_gle_gc -> end_gen_texture) (*(_gle_gc -> end_gen_texture))(); \
	glEnd ();					\
}

/* ====================================================== */
#else /* AUTO_TEXTURE */

#define BGNTMESH(i,len)	glBegin (GL_TRIANGLE_STRIP);
#define BGNPOLYGON() 	glBegin (GL_POLYGON);

#define	N3F_F(x)	glNormal3fv(x)
#define	N3F_D(x)	glNormal3dv(x)
#define V3F_F(x,j,id)	glVertex3fv(x);
#define V3F_D(x,j,id)	glVertex3dv(x);

#define ENDTMESH()	glEnd ()
#define ENDPOLYGON()	glEnd()

#endif /* AUTO_TEXTURE */

#endif /* OPENGL_10 */

/* ====================================================== */


#endif /* __GLE_PORT_H__ */
/* ================== END OF FILE ======================= */
