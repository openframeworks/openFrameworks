
/*
 * extrude.h
 *
 * FUNCTION:
 * prototypes for privately used subroutines for the tubing library
 *
 * HISTORY:
 * Linas Vepstas 1991
 */
 
#include "port.h" /* for gleDouble */

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

/* ============================================================ */
/* 
 * Provides choice of calling subroutine, vs. invoking macro.
 * Basically, inlines the source, or not.
 * Trades performance for executable size.
 */

#define INLINE_INTERSECT
#ifdef INLINE_INTERSECT
#define INNERSECT(sect,p,n,v1,v2) { INTERSECT(sect,p,n,v1,v2); }
#else
#define INNERSECT(sect,p,n,v1,v2) intersect(sect,p,n,v1,v2)
#endif /* INLINE_INTERSECT */

/* ============================================================ */
/* The folowing defines give a kludgy way of accessing the qmesh primitive */

/*
#define bgntmesh _emu_qmesh_bgnqmesh
#define endtmesh _emu_qmesh_endqmesh
#define c3f _emu_qmesh_c3f
#define n3f _emu_qmesh_n3f
#define v3f _emu_qmesh_v3f
*/

/* ============================================================ */

extern void up_sanity_check (gleDouble up[3],      /* up vector for contour */
                      int npoints,              /* numpoints in poly-line */
                      gleDouble point_array[][3]);   /* polyline */


extern void draw_raw_style_end_cap (int ncp,     /* number of contour points */
                             gleDouble contour[][2],     /* 2D contour */
                             gleDouble zval,             /* where to draw cap */
                             int frontwards);    /* front or back cap */

extern void draw_round_style_cap_callback (int iloop,
                                  double cap[][3],
                                  float face_color[3],
                                  gleDouble cut_vector[3],
                                  gleDouble bisect_vector[3],
                                  double norms[][3],
                                  int frontwards);

extern void draw_angle_style_front_cap (int ncp,
                           gleDouble bi[3],
                           gleDouble point_array[][3]);

extern void extrusion_raw_join (int ncp,        /* number of contour points */
                         gleDouble contour[][2],    /* 2D contour */
                         gleDouble cont_normal[][2],/* 2D contour normal vecs */
                         gleDouble up[3],           /* up vector for contour */
                         int npoints,           /* numpoints in poly-line */
                         gleDouble point_array[][3],        /* polyline */
                         float color_array[][3],        /* color of polyline */
                         gleDouble xform_array[][2][3]);  /* 2D contour xforms */


extern void extrusion_round_or_cut_join (int ncp, /* number of contour points */
                         gleDouble contour[][2],    /* 2D contour */
                         gleDouble cont_normal[][2],/* 2D contour normal vecs */
                           gleDouble up[3],         /* up vector for contour */
                           int npoints,         /* numpoints in poly-line */
                           gleDouble point_array[][3],      /* polyline */
                           float color_array[][3],      /* color of polyline */
                           gleDouble xform_array[][2][3]);  /* 2D contour xforms */


extern void extrusion_angle_join (int ncp,      /* number of contour points */
                         gleDouble contour[][2],    /* 2D contour */
                         gleDouble cont_normal[][2],/* 2D contour normal vecs */
                           gleDouble up[3],         /* up vector for contour */
                           int npoints,         /* numpoints in poly-line */
                           gleDouble point_array[][3],      /* polyline */
                           float color_array[][3],      /* color of polyline */
                           gleDouble xform_array[][2][3]);  /* 2D contour xforms */

/* -------------------------- end of file -------------------------------- */
