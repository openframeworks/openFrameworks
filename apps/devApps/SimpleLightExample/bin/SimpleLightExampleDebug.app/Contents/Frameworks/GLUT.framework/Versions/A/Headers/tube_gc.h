
/*
 * tube_gc.h
 *
 * FUNCTION:
 * This file allows for easy changes to changes in the way the extrusion
 * library handles state info (i.e. context).
 *
 * HISTORY:
 * Linas Vepstas --- February 1993
 * Added auto texture coord generation hacks, Linas April 1994
 * Added tube.h include to define gleDouble, tad February 2002
 */

#include "tube.h"
#include "port.h" /* for gleVector */

typedef float gleColor[3];
typedef double gleTwoVec[2];

typedef struct {

   /* public methods */
   void (*bgn_gen_texture) (int, double);
   void (*n3f_gen_texture) (float *);
   void (*n3d_gen_texture) (double *);
   void (*v3f_gen_texture) (float *, int, int);
   void (*v3d_gen_texture) (double *, int, int);
   void (*end_gen_texture) (void);

   /* protected members -- "general knowledge" stuff */
   int join_style;

   /* arguments passed into extrusion code */ 
   int ncp;     /* number of contour points */
   gleTwoVec *contour;    /* 2D contour */
   gleTwoVec *cont_normal;  /* 2D contour normals */
   gleDouble *up;               /* up vector */
   int npoints;  /* number of points in polyline */
   gleVector *point_array;     /* path */
   gleColor *color_array;         /* path colors */
   gleAffine *xform_array;  /* contour xforms */

   /* private members, used by texturing code */
   int num_vert;
   int segment_number;
   double segment_length;
   double accum_seg_len;
   double prev_x;
   double prev_y;

   void (*save_bgn_gen_texture) (int, double);
   void (*save_n3f_gen_texture) (float *);
   void (*save_n3d_gen_texture) (double *);
   void (*save_v3f_gen_texture) (float *, int, int);
   void (*save_v3d_gen_texture) (double *, int, int);
   void (*save_end_gen_texture) (void);

} gleGC;

extern gleGC *_gle_gc;
extern gleGC * gleCreateGC (void);

#define INIT_GC() {if (!_gle_gc) _gle_gc = gleCreateGC(); }
#define extrusion_join_style (_gle_gc->join_style)

#define __TUBE_CLOSE_CONTOUR (extrusion_join_style & TUBE_CONTOUR_CLOSED)
#define __TUBE_DRAW_CAP (extrusion_join_style & TUBE_JN_CAP)
#define __TUBE_DRAW_FACET_NORMALS (extrusion_join_style & TUBE_NORM_FACET)
#define __TUBE_DRAW_PATH_EDGE_NORMALS (extrusion_join_style & TUBE_NORM_PATH_EDGE)

#define __TUBE_STYLE (extrusion_join_style & TUBE_JN_MASK)
#define __TUBE_RAW_JOIN (extrusion_join_style & TUBE_JN_RAW)
#define __TUBE_CUT_JOIN (extrusion_join_style & TUBE_JN_CUT)
#define __TUBE_ANGLE_JOIN (extrusion_join_style & TUBE_JN_ANGLE)
#define __TUBE_ROUND_JOIN (extrusion_join_style & TUBE_JN_ROUND)

/* ======================= END OF FILE ========================== */
