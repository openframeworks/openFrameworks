
#ifndef __SCHRO_ANALYSIS_H__
#define __SCHRO_ANALYSIS_H__

#include <schroedinger/schroencoder.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

void schro_encoder_frame_analyse (SchroEncoderFrame *frame);
void schro_encoder_frame_downsample (SchroEncoderFrame *frame);
void schro_encoder_frame_upsample (SchroEncoderFrame* frame);
void schro_frame_mean_squared_error (SchroFrame *a, SchroFrame *b, double *mse);

#endif

SCHRO_END_DECLS

#endif
