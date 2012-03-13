
#ifndef __SCHRO_FILTER_H__
#define __SCHRO_FILTER_H__

#include <schroedinger/schroframe.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

void schro_frame_filter_lowpass (SchroFrame * frame, int tap);
void schro_frame_filter_lowpass2 (SchroFrame *frame, double sigma);
void schro_frame_filter_lowpass_16 (SchroFrame *frame);
void schro_frame_filter_cwm7 (SchroFrame *frame);
void schro_frame_filter_cwmN (SchroFrame *frame, int weight);
void schro_frame_filter_cwmN_ref (SchroFrame *frame, int weight);
void schro_frame_filter_wavelet (SchroFrame *frame);
void schro_frame_filter_addnoise (SchroFrame *frame, double sigma);
void schro_frame_filter_adaptive_lowpass (SchroFrame *frame);

void schro_filter_cwmN (uint8_t *d, uint8_t *s1, uint8_t *s2, uint8_t *s3, int n, int weight);
void schro_filter_cwmN_ref (uint8_t *d, uint8_t *s1, uint8_t *s2, uint8_t *s3, int n, int weight);
void schro_filter_cwm7 (uint8_t *d, uint8_t *s1, uint8_t *s2, uint8_t *s3, int n);

#endif

SCHRO_END_DECLS

#endif

