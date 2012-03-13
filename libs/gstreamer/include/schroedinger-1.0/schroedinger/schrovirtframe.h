
#ifndef __SCHRO_VIRT_FRAME_H__
#define __SCHRO_VIRT_FRAME_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schroframe.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

SchroFrame *schro_frame_new_virtual (SchroMemoryDomain *domain,
    SchroFrameFormat format, int width, int height);

void *schro_virt_frame_get_line (SchroFrame *frame, int component, int i);
void *schro_virt_frame_get_line_unrendered (SchroFrame *frame, int component, int i);
void schro_virt_frame_set_line_rendered (SchroFrame *frame, int component, int i);
void schro_virt_frame_render_line (SchroFrame *frame, void *dest,
    int component, int i);

void schro_virt_frame_render (SchroFrame *frame, SchroFrame *dest);

SchroFrame *schro_virt_frame_new_horiz_downsample (SchroFrame *vf, int cosite);
SchroFrame *schro_virt_frame_new_vert_downsample (SchroFrame *vf, int cosite);
SchroFrame *schro_virt_frame_new_vert_resample (SchroFrame *vf, int height);
SchroFrame *schro_virt_frame_new_horiz_resample (SchroFrame *vf, int width);
SchroFrame *schro_virt_frame_new_unpack (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_pack_YUY2 (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_pack_UYVY (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_pack_AYUV (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_pack_v216 (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_pack_v210 (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_pack_RGB (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_color_matrix (SchroFrame *vf);
SchroFrame *schro_virt_frame_new_subsample (SchroFrame *vf, SchroFrameFormat format);

SchroFrame * schro_virt_frame_new_convert_u8 (SchroFrame *vf);
SchroFrame * schro_virt_frame_new_convert_s16 (SchroFrame *vf);
SchroFrame * schro_virt_frame_new_crop (SchroFrame *vf, int width, int height);
SchroFrame * schro_virt_frame_new_edgeextend (SchroFrame *vf, int width, int height);
SchroFrame * schro_virt_frame_new_interlace (SchroFrame *vf_top, SchroFrame *vf_bottom);

#endif

SCHRO_END_DECLS

#endif

