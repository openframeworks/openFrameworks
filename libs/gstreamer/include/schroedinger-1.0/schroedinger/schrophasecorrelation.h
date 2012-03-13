
#ifndef __SCHRO_PHASECORRELATION_H__
#define __SCHRO_PHASECORRELATION_H__

#include <schroedinger/schroencoder.h>
#include <schroedinger/schromotionest.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

typedef struct _SchroPhaseCorr SchroPhaseCorr;
typedef struct _SchroPhaseCorrLevel SchroPhaseCorrLevel;

struct _SchroPhaseCorrLevel {
  int hshift;
  int vshift;
  int width;
  int height;
  
  int num_x;
  int num_y;
  int *vecs_dx;
  int *vecs_dy;
  int *vecs2_dx;
  int *vecs2_dy;
};

struct _SchroPhaseCorr {
  SchroEncoderFrame *frame;
  SchroEncoderFrame *ref;

  int shift;
  int n;
  int picture_shift;

  int n_levels;
  SchroPhaseCorrLevel levels[8];

  /* static tables */
  float *s, *c;
  float *zero;
  float *weight;

  float *image1;
  float *image2;
  float *ft1r;
  float *ft1i;
  float *ft2r;
  float *ft2i;
  float *conv_r, *conv_i;
  float *resr, *resi;

};

SchroPhaseCorr * schro_phasecorr_new (SchroEncoderFrame *frame,
    SchroEncoderFrame *ref);
void schro_encoder_phasecorr_estimation (SchroPhaseCorr *pc);
void schro_phasecorr_free (SchroPhaseCorr *pc);


#endif

SCHRO_END_DECLS

#endif

