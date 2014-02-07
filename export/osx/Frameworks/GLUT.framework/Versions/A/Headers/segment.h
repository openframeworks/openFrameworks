
/*
 * MODULE: segment.h
 *
 * FUNCTION:
 * Contains function prototypes for segment drawing subroutines.
 * These are used only internally, and are not to be exported to
 * the user.
 *
 * HISTORY:
 * Create by Linas Vepstas
 * Added tube.h include to define gleDouble, tad February 2002
 */

/* ============================================================ */

#include "tube.h"

extern void draw_segment_plain (int ncp,       /* number of contour points */
                           gleDouble front_contour[][3],
                           gleDouble back_contour[][3],
                           int inext, double len);

extern void draw_segment_color (int ncp,       /* number of contour points */
                           gleDouble front_contour[][3],
                           gleDouble back_contour[][3],
                           float color_last[3],
                           float color_next[3],
                           int inext, double len);

extern void draw_segment_edge_n (int ncp,      /* number of contour points */
                           gleDouble front_contour[][3],
                           gleDouble back_contour[][3],
                           double norm_cont[][3],
                           int inext, double len);

extern void draw_segment_c_and_edge_n (int ncp,   
                           gleDouble front_contour[][3],
                           gleDouble back_contour[][3],
                           double norm_cont[][3],
                           float color_last[3],
                           float color_next[3],
                           int inext, double len);

extern void draw_segment_facet_n (int ncp,     
                           gleDouble front_contour[][3],
                           gleDouble back_contour[][3],
                           double norm_cont[][3],
                           int inext, double len);

extern void draw_segment_c_and_facet_n (int ncp,    
                           gleDouble front_contour[][3],
                           gleDouble back_contour[][3],
                           double norm_cont[][3],
                           float color_last[3],
                           float color_next[3],
                           int inext, double len);

/* ============================================================ */

extern void draw_binorm_segment_edge_n (int ncp,  
                           double front_contour[][3],
                           double back_contour[][3],
                           double front_norm[][3],
                           double back_norm[][3],
                           int inext, double len);

extern void draw_binorm_segment_c_and_edge_n (int ncp,   
                           double front_contour[][3],
                           double back_contour[][3],
                           double front_norm[][3],
                           double back_norm[][3],
                           float color_last[3],
                           float color_next[3],
                           int inext, double len);

extern void draw_binorm_segment_facet_n (int ncp, 
                           double front_contour[][3],
                           double back_contour[][3],
                           double front_norm[][3],
                           double back_norm[][3],
                           int inext, double len);

extern void draw_binorm_segment_c_and_facet_n (int ncp,    
                           double front_contour[][3],
                           double back_contour[][3],
                           double front_norm[][3],
                           double back_norm[][3],
                           float color_last[3],
                           float color_next[3],
                           int inext, double len);

extern void draw_angle_style_back_cap (int ncp,        /* number of contour points */
                           gleDouble bi[3],             /* biscetor */
                           gleDouble point_array[][3]);  /* polyline */

/* -------------------------- end of file -------------------------------- */

