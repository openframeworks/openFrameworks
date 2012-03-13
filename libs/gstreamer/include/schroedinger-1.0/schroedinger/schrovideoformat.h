
#ifndef __SCHRO_VIDEO_FORMAT_H__
#define __SCHRO_VIDEO_FORMAT_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schrobitstream.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroVideoFormat SchroVideoFormat;

struct _SchroVideoFormat {
  SchroVideoFormatEnum index;
  int width;
  int height;
  SchroChromaFormat chroma_format;

  schro_bool interlaced;
  schro_bool top_field_first;

  int frame_rate_numerator;
  int frame_rate_denominator;
  int aspect_ratio_numerator;
  int aspect_ratio_denominator;

  int clean_width;
  int clean_height;
  int left_offset;
  int top_offset;

  int luma_offset;
  int luma_excursion;
  int chroma_offset;
  int chroma_excursion;

  SchroColourPrimaries colour_primaries;
  SchroColourMatrix colour_matrix;
  SchroTransferFunction transfer_function;

  int interlaced_coding;

  int unused0;
  int unused1;
  int unused2;
};

int schro_video_format_validate (SchroVideoFormat *format);

void schro_video_format_set_std_video_format (SchroVideoFormat *format,
    SchroVideoFormatEnum index);
SchroVideoFormatEnum schro_video_format_get_std_video_format (SchroVideoFormat *format);
void schro_video_format_set_std_frame_rate (SchroVideoFormat *format, int index);
int schro_video_format_get_std_frame_rate (SchroVideoFormat *format);
void schro_video_format_set_std_aspect_ratio (SchroVideoFormat *format, int index);
int schro_video_format_get_std_aspect_ratio (SchroVideoFormat *format);
void schro_video_format_set_std_signal_range (SchroVideoFormat *format,
    SchroSignalRange index);
SchroSignalRange schro_video_format_get_std_signal_range (SchroVideoFormat *format);
void schro_video_format_set_std_colour_spec (SchroVideoFormat *format,
    SchroColourSpec index);
SchroColourSpec schro_video_format_get_std_colour_spec (SchroVideoFormat *format);

#ifdef SCHRO_ENABLE_UNSTABLE_API

int schro_video_format_get_picture_height (SchroVideoFormat *format);
void schro_video_format_get_picture_luma_size (SchroVideoFormat *format,
    int *picture_luma_width, int *picture_luma_height);
void schro_video_format_get_picture_chroma_size (SchroVideoFormat *format,
    int *picture_chroma_width, int *picture_chroma_height);
void schro_video_format_get_iwt_alloc_size (SchroVideoFormat *format,
    int *width, int *height, int transform_depth);
int schro_video_format_get_bit_depth (SchroVideoFormat *format);
schro_bool schro_video_format_check_MP_DL (SchroVideoFormat *format);
schro_bool schro_video_format_check_VC2_DL (SchroVideoFormat *format);
int schro_video_format_compare_new_sequence (SchroVideoFormat *a, SchroVideoFormat *b);
int schro_video_format_compare (SchroVideoFormat *a, SchroVideoFormat *b);

#endif

SCHRO_END_DECLS

#endif

