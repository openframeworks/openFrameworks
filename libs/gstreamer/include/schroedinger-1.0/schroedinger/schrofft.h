
#ifndef __SCHRO_SCHRO_FFT_H__
#define __SCHRO_SCHRO_FFT_H__

#include <schroedinger/schroutils.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

void schro_fft_generate_tables_f32 (float *costable, float *sintable, int shift);

void schro_fft_fwd_f32 (float *d_real, float *d_imag, const float *s_real,
    const float *s_imag, const float *costable, const float *sintable,
    int shift);
void schro_fft_rev_f32 (float *d_real, float *d_imag, const float *s_real,
    const float *s_imag, const float *costable, const float *sintable,
    int shift);

#endif

SCHRO_END_DECLS

#endif

