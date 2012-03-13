
#ifndef __SCHRO_SSIM_H__
#define __SCHRO_SSIM_H__

#include <schroedinger/schroframe.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

double schro_frame_ssim (SchroFrame *a, SchroFrame *b);

#endif

SCHRO_END_DECLS

#endif

