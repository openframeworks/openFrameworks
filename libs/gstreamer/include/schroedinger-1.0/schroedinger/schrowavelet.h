
#ifndef _SCHRO_WAVELET_H_
#define _SCHRO_WAVELET_H_

#include <schroedinger/schroutils.h>
#include <schroedinger/schroframe.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

void schro_wavelet_transform_2d (SchroFrameData *fd, int type, int16_t *tmp);
void schro_wavelet_inverse_transform_2d (SchroFrameData *fd, int type, int16_t *tmp);

void schro_iwt_desl_9_3 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_5_3 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_13_5 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_haar0 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_haar1 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_haar2 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_fidelity (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iwt_daub_9_7 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);

void schro_iiwt_desl_9_3 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_5_3 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_13_5 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_haar0 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_haar1 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_haar2 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_fidelity (int16_t *data, int stride, int width, int height,
    int16_t *tmp);
void schro_iiwt_daub_9_7 (int16_t *data, int stride, int width, int height,
    int16_t *tmp);

#endif

SCHRO_END_DECLS

#endif

