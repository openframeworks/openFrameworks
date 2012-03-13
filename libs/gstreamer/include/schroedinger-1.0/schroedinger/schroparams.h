
#ifndef __SCHRO_PARAMS_H__
#define __SCHRO_PARAMS_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schrobitstream.h>
#include <schroedinger/schroframe.h>
#include <schroedinger/schrolimits.h>
#include <schroedinger/schrovideoformat.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroParams SchroParams;
typedef struct _SchroGlobalMotion SchroGlobalMotion;

#ifdef SCHRO_ENABLE_UNSTABLE_API

struct _SchroGlobalMotion {
  int b0;
  int b1;
  int a_exp;
  int a00;
  int a01;
  int a10;
  int a11;
  int c_exp;
  int c0;
  int c1;
};

struct _SchroParams {
  /*< private >*/
  SchroVideoFormat *video_format;
  int is_noarith;

  /* transform parameters */
  int wavelet_filter_index;
  int transform_depth;
  int horiz_codeblocks[SCHRO_LIMIT_TRANSFORM_DEPTH + 1];
  int vert_codeblocks[SCHRO_LIMIT_TRANSFORM_DEPTH + 1];
  int codeblock_mode_index;

  /* motion prediction parameters */
  int num_refs;
  int have_global_motion; /* using_global_motion */
  int xblen_luma;
  int yblen_luma;
  int xbsep_luma;
  int ybsep_luma;
  int mv_precision;
  SchroGlobalMotion global_motion[2];
  int picture_pred_mode;
  int picture_weight_bits;
  int picture_weight_1;
  int picture_weight_2;

  /* DiracPro parameters */
  int is_lowdelay;
  int n_horiz_slices; /* slices_x */
  int n_vert_slices; /* slices_y */
  int slice_bytes_num;
  int slice_bytes_denom;
  int quant_matrix[3*SCHRO_LIMIT_TRANSFORM_DEPTH+1];

  /* calculated sizes */
  int iwt_chroma_width;
  int iwt_chroma_height;
  int iwt_luma_width;
  int iwt_luma_height;
  int x_num_blocks;
  int y_num_blocks;
  int x_offset;
  int y_offset;
};

#define SCHRO_SUBBAND_IS_HORIZONTALLY_ORIENTED(position) (((position)&3) == 2)
#define SCHRO_SUBBAND_IS_VERTICALLY_ORIENTED(position) (((position)&3) == 1)
#define SCHRO_SUBBAND_IS_DIAGONALLY_ORIENTED(position) (((position)&3) == 3)
#define SCHRO_SUBBAND_SHIFT(position) ((position)>>2)

extern const int schro_tables_lowdelay_quants[7][4][9];

void schro_params_init (SchroParams *params, int video_format);

void schro_params_calculate_iwt_sizes (SchroParams *params);
void schro_params_calculate_mc_sizes (SchroParams *params);

int schro_params_set_block_params (SchroParams *params, int index);
int schro_params_get_block_params (SchroParams *params);
int schro_params_verify_block_params (SchroParams *params);

void schro_params_set_default_codeblock (SchroParams *params);
schro_bool schro_params_is_default_codeblock (SchroParams *params);

void schro_subband_get_frame_data (SchroFrameData *dest,
    SchroFrame *frame, int component, int position, SchroParams *params);
int schro_subband_get_position (int index);
void schro_params_set_default_quant_matrix (SchroParams *params);
schro_bool schro_params_is_default_quant_matrix (SchroParams *params);

/* FIXME should be SchroFrameFormat */
int schro_params_get_frame_format (int depth,
    SchroChromaFormat chroma_format);

/* FIXME should be moved */
void schro_frame_iwt_transform (SchroFrame *frame, SchroParams *params);
void schro_frame_inverse_iwt_transform (SchroFrame *frame, SchroParams *params);

#endif

SCHRO_END_DECLS

#endif

